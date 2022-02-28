#include <cstdio>

#include "paqFe/paqFe.hpp"
#include "paqFe/models/Group.hpp"
#include "paqFe/Mixer.hpp"

using namespace paqFe::internal;

FILE* gfout;

int mixer_id = 0;

template<int nfeature, size_t N = 80>
class MixerWrapped : Mixer<nfeature, N> {
  using Parent = Mixer<nfeature, N>;
  int id = -1;
  Prob Ps[nfeature];
public:

  MixerWrapped() : Parent() {
    id = mixer_id++;
  }

  void predict(const Prob* P, Context ctx, Prob *pp) {
    for(int i=0;i<nfeature;i++)
      Ps[i] = P[i];
    Parent::predict(P, ctx, pp);
  }

  void update(uint8_t bit) {
    Parent::update(bit);
    output(bit);
  }

  void update(uint8_t bit, Prob p) {
    Parent::update(bit, p);
    output(bit);
  }

private:
  void output(uint8_t bit) {
    fprintf(gfout, "%d,", id);
    for(int i=0;i<nfeature;i++)
      fprintf(gfout, "%d,", Ps[i]);
    fprintf(gfout, "%d,%d,%d,%d\n", Parent::prev_ctx, bit, Parent::prev_prob, dot(Xs, Ws[ctx], nfeature) >> 16);
  }
};

using IPredictor = Predictor<8, paqFe::paqFeFile::Model, MixerWrapped<paqFe::paqFeFile::Model::nProb>>;

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
