#pragma once

#include "Mixer.hpp"
#include "models/PresudoModel.hpp"
#include "models/MatchModel.hpp"
#include "models/StateModel.hpp"
#include "models/Group.hpp"

#include <type_traits>

namespace paqFe::internal {

template<int N, typename ... Models>
class Predictor {
  using Model = ModelGroup<Models...>;

  Model m;
  Mixer<Model::OutputCnt> mixers[N];

  int mixer_duty = 0;

  bool first = true;

public:
  Predictor() {

  }

  void predict(uint8_t bit, Prob *pp) {
    mixers[mixer_duty].update(bit);
    mixer_duty = (mixer_duty + 1) % N;

    Prob P[Model::OutputCnt];
    Context Ctx[Model::OutputCnt];

    m.predict(bit, P, Ctx);
    Context ctx = contextSum(Ctx, Model::OutputCnt);

    mixers[mixer_duty].predict(P, ctx, pp);
  };

  void predict_byte(uint8_t byte, Prob *pp) {
    Prob Ps[8][Model::OutputCnt];
    Context Ctxs[8][Model::OutputCnt];

    m.predict_byte(byte, &Ps[0][0], &Ctxs[0][0]);

    for(int i=0;i<8;i++) {
      uint8_t bit = (byte >> (7 - i)) & 0x1;

      Context ctx = contextSum(Ctxs[i], Model::OutputCnt);
    
      mixers[(mixer_duty + i) % N].predict(Ps[i], ctx, &pp[i]);
    }

    if(first) {
      pp[0] = ProbEven;
      first = false;
    }

    for(int i=0;i<8;i++) {
      uint8_t bit = (byte >> (7 - i)) & 0x1;
      mixers[(mixer_duty + i) % N].update(bit, pp[i]);
    }
    mixer_duty = (mixer_duty + 8) % N;
  };

private:
  Context contextSum(const Context *pctx, int n) {
    Context s = 0;
    return 0;
    for(int i=0;i<n;i++) {
      s += pctx[i];
    }

    return s;
  }
};

}