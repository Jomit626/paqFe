#include <cstdio>

#include <paqFe/types.hpp>
#include <paqFe/Mixer.hpp>

using namespace paqFe::internal;
using namespace paqFe;

constexpr size_t n = 500000;

class PattenedModel {
  bool first = true;
  int counter = 2048;
public:
  static constexpr int OutputCnt = 1;

  void predict(uint8_t bit, Prob *pp, Context *pctx) {
    update(1);
    *pp = counter;
    *pctx = counter & 1;
  }

  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t stride = OutputCnt) {
    for(int i=0;i<8;i++) {
      pp[i * stride] = counter;
      pctx[i * stride] = counter & 1;
      update(1);
    }
    if(first) {
      pp[0] = ProbEven;
      pctx[0] = 0;
      first = false;
    }
  }

protected:
  void update(int n) {
    counter += n;
    if(counter > 4095) {
      counter = 0;
    }
  }
};

class PattenedModel2 {
  int counter = 2001;
  bool first = true;
public:
  static constexpr int OutputCnt = 2;

  void predict(uint8_t bit, Prob *pp, Context *pctx) {
    update(1);
    pp[0] = counter;
    pctx[0] = counter & 1;

    pp[1] = 4096 - counter;;
    pctx[1] = counter & 1;
  }

  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t stride = OutputCnt) {
    for(int i=0;i<8;i++) {
      pp[i * stride] = counter;
      pctx[i * stride] = counter & 1;
      pp[i * stride + 1] = 4096 - counter;
      pctx[i * stride + 1] = ~(counter & 1);
      update(1);
    }
    if(first) {
      pp[0] = ProbEven;
      pctx[0] = 0;
      first = false;
    }
  }

protected:
  void update(int n) {
    counter += n;
    if(counter > 4095) {
      counter = 0;
    }
  }
};


int main() {
  Mixer<8, PattenedModel,PattenedModel2> m_ref;
  Mixer<8, PattenedModel,PattenedModel2> m;

  Prob* pp = new Prob[n * 8];
  Prob* pp_test = new Prob[n * 8];

  pp[0] = 2048;
  for(int i=0;i<n;i++) {
    uint8_t byte = i;
    for(int j=7;j>=0;j--) {
      uint8_t bit = (byte >> j) & 0x1;

      m_ref.predict(bit, &pp[i*8 + 7 - j + 1]);
    }
  }


  for(int i=0;i<n;i++) {
    uint8_t byte = i;
    m.predict_byte(byte, &pp_test[8*i]);
  }

  for(int i=0;i<n*8 - 1;i++) {
    printf("%d,%d,%d,%d\n",i/8,i%8, pp[i], pp_test[i]);
    if(pp[i] != pp_test[i]) {
      return 1;
    }
  }
  return 0;
}