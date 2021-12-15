#include <cstdio>

#include <paqFe/types.hpp>
#include <paqFe/Mixer.hpp>

using namespace paqFe::internal;
using namespace paqFe;

constexpr size_t n = 500000;

class PattenedModel {
  int counter = 2048;
public:
  static constexpr int n_output = 1;

  void predict(uint8_t bit, Prob *pp) {
    update(1);
    *pp = counter;
  }

  void predict_byte(uint8_t byte, Prob *pp, size_t stride = 1) {
    for(int i=0;i<8;i++) {
      pp[i * stride] = counter;
      update(1);
    }
  }

  Context getContext() {
    return 0;
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
  int counter = 2000;
public:
  static constexpr int n_output = 1;

  void predict(uint8_t bit, Prob *pp) {
    update(1);
    *pp = counter;
  }

  void predict_byte(uint8_t byte, Prob *pp, size_t stride = 1) {
    for(int i=0;i<8;i++) {
      pp[i * stride] = counter;
      update(1);
    }
  }

  Context getContext() {
    return 0;
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
  Mixer<8, PattenedModel> m_ref;
  Mixer<8, PattenedModel> m;

  Prob* pp = new Prob[n * 8];
  Prob* pp_test = new Prob[n * 8];

  for(int i=0;i<n;i++) {
    uint8_t byte = i;
    for(int j=7;j>=0;j--) {
      uint8_t bit = (byte >> j) & 0x1;

      m_ref.predict(bit, &pp[i*8 + 7 - j]);
    }
  }

  for(int i=0;i<n;i++) {
    uint8_t byte = i;
    m.predict_byte(byte, &pp_test[8*i]);
  }

  for(int i=0;i<n*8 - 1;i++) {
    printf("%d,%d,%d,%d\n",i/8,i%8, pp[i], pp_test[i + 1]);
    if(pp[i] != pp_test[i + 1])
      return 1;
  }
  return 0;
}