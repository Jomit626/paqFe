#include <cstdio>

#include "paqFe/paqFe.hpp"
#include "paqFe/models/Group.hpp"
#include "paqFe/Mixer.hpp"

using namespace paqFe::internal;

FILE* gfout;

int mixer_id = 0;

template<int nFeature, int nHidden>
class MixerWrapped : Mixer<nFeature, nHidden> {
  using Parent = Mixer<nFeature, nHidden>;

public:
  MixerWrapped() : Parent() {
  }

  void predict(const Prob* P, const Context *pctx, Prob *pp) {
    Parent::predict(P, pctx, pp);
  }

  void update(uint8_t bit) {
    pre_output();
    Parent::update(bit);
    output(bit);
  }

  void update(uint8_t bit, Prob p) {
    pre_output();
    Parent::update(bit, p);
    output(bit);
  }
private:
  int32_t Ws[nHidden][nFeature];
  void pre_output() {
    for(int j=0;j<nHidden;j++)
      for(int i=0;i<nFeature;i++)
        Ws[j][i] = Parent::W[j][Parent::prev_ctx[j]][i];
  }
  void output(uint8_t bit) {
    for(int j=0;j<nHidden;j++) {
      for(int i=0;i<nFeature;i++)
        fprintf(gfout, "%d,", Parent::X[i]);

      for(int i=0;i<nFeature;i++)
        fprintf(gfout, "%d,", Ws[j][i]);

      for(int i=0;i<nFeature;i++)
        fprintf(gfout, "%d,", Parent::W[j][Parent::prev_ctx[j]][i]);

      Prob p = LUT.squash(Parent::X1[j]);
      fprintf(gfout, "%d\n", Parent::lossCal(p, bit, Parent::Layer1LR));
    }
  }
};

using Model = paqFe::paqFeFile::Model;
using IPredictor = Predictor<8, Model, MixerWrapped<Model::nProb, Model::nCtx>>;

void generate_db(FILE* fin, FILE* fout) {
  gfout = fout;
  IPredictor &predictor = *(new IPredictor());

  uint8_t data[128];
  size_t n = 0;
  Prob prob = 2048;
  Prob p[8];
  while((n = fread(data, 1, 128, fin))) {
    for(int i=0;i<n;i++) {
      uint8_t byte = data[i];
      predictor.predict_byte(byte, &p[0]);
    }
  }

  delete &predictor;
}
