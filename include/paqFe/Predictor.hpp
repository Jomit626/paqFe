#pragma once

#include "Mixer.hpp"
#include "APM.hpp"
#include "models/PresudoModel.hpp"
#include "models/MatchModel.hpp"
#include "models/StateModel.hpp"
#include "models/Group.hpp"

#include <type_traits>

namespace paqFe::internal {

template<typename Model, int sel = 0>
class PassThroughtPredictor {
  Model m;
  static_assert(sel >= 0 && sel < Model::nProb);

  bool first = true;
public:
  void predict(uint8_t bit, Prob *pp) {
    Prob ps[Model::nProb];
    Context c[Model::nCtx];
    m.predict(bit, ps, c);

    *pp = ps[sel];
  }
  void predict_byte(uint8_t byte, Prob *pp) {
    Prob ps[8][Model::nProb];
    Context ctxs[8][Model::nCtx];

    m.predict_byte(byte, &ps[0][0], &ctxs[0][0], Model::nProb, Model::nCtx);
    for(int i=0;i<8;i++)
      pp[i] = ps[i][sel];
    if(first) {
      pp[0] = ProbEven;
      first = false;
    }
  }
};

template<int N, typename ... Models>
class Predictor {
  static_assert(N >= 8);

  using Model = ModelGroup<Models...>;

  Model m;
  Mixer<Model::nProb> mixers[N];

  APM<1024 * 24> apms[N];
  APM<4096> apms2[N];

  int mixer_duty = 0;

  bool first = true;

public:
  Predictor() {

  }

  void predict(uint8_t bit, Prob *pp) {
    Prob px, p1;
    if(first) {
      apms[0].predict(0, ProbEven, &px);
      first = false;
    }
    mixers[mixer_duty].update(bit);
    apms[mixer_duty].update(bit);

    mixer_duty = (mixer_duty + 1) % N;

    Prob P[Model::nProb];
    Context Ctx[Model::nCtx];

    m.predict(bit, P, Ctx);
    Context ctx = contextSum(Ctx, Model::nCtx);

    mixers[mixer_duty].predict(P, ctx, &px);
    apms[mixer_duty].predict(ctx, px, &p1);

    *pp = (px + p1 * 3) / 4;
  };

  void predict_byte(uint8_t byte, Prob *pp) {
    Prob Ps[8][Model::nProb];
    Prob Px[8];
    Prob P1[8];
    Context Ctxs[8][Model::nCtx];
    Context ctx[8];

    m.predict_byte(byte, &Ps[0][0], &Ctxs[0][0]);

    for(int i=0;i<8;i++) {
      uint8_t bit = (byte >> (7 - i)) & 0x1;

      ctx[i] = contextSum(Ctxs[i], Model::nCtx);
    
      mixers[(mixer_duty + i) % N].predict(Ps[i], ctx[i], &Px[i]);
    }

    if(first) {
      Px[0] = ProbEven;
      ctx[0] = 0;
    }

    for(int i=0;i<8;i++) {
      uint8_t bit = (byte >> (7 - i)) & 0x1;
      mixers[(mixer_duty + i) % N].update(bit, Px[i]);

      apms[(mixer_duty + i) % N].predict(ctx[i], Px[i], &P1[i]);
      apms[(mixer_duty + i) % N].update(bit);
    }
    mixer_duty = (mixer_duty + 8) % N;

    for(int i=0;i<8;i++) {
      pp[i] = (Px[i] + P1[i] * 3) / 4;
    }

    if(first) {
      pp[0] = ProbEven;
      first = false;
    }
  };

private:
  Context contextSum(const Context *pctx, int n) {
    Context s = 0;
    for(int i=0;i<n;i++) {
      s += pctx[i];
    }
    return s;
  }
};

}