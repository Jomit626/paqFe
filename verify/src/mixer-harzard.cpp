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
  int id = -1;
  int prev_ctx[nHidden] = {};
  int harzard_cnt[nHidden] = {-1}; // 'cause first input 0 is compred with NULL prev_ctx
  int cnt = 0;

  int counter = 0;
public:

  MixerWrapped() : Parent() {
    id = mixer_id++;
  }

  ~MixerWrapped() {
    for(int i=0;i<nHidden;i++) {
      printf("%d, %d, %d, %d, %lf%\n", id, i, harzard_cnt[i], cnt, (double)harzard_cnt[i]/cnt * 100.0);
    }
  }

  void predict(const Prob* P, const Context *pctx, Prob *pp) {
    harzard_dectet(pctx);
    Parent::predict(P, pctx, pp);
  }

  void update(uint8_t bit) {
    Parent::update(bit);
  }

  void update(uint8_t bit, Prob p) {
    Parent::update(bit, p);
  }

private:
  void harzard_dectet(const Context *pctx) {
    cnt ++;
    for(int i=0;i<nHidden;i++) {
      if(prev_ctx[i] == (pctx[i] & 0xFF)) {
        harzard_cnt[i] ++;
      }
      prev_ctx[i] = (pctx[i] & 0xFF);
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
