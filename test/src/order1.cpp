#include <cstdio>

#include <paqFe/types.hpp>
#include <paqFe/models/Orders.hpp>

using namespace paqFe::internal;
using namespace paqFe;

constexpr size_t n = 100000;

int main() {
  Order1 a;
  Order1 b;

  Prob* pp = new Prob[n * 8];
  Prob* pp_test = new Prob[n * 8];

  for(int i=0;i<n;i++) {
    uint8_t byte = i;
    for(int j=7;j>=0;j--) {
      uint8_t bit = (byte >> j) & 0x1;

      a.predict(bit, &pp[i*8 + 7 - j]);
    }
  }

  for(int i=0;i<n;i++) {
    uint8_t byte = i;
    b.predict_byte(byte, &pp_test[8*i]);
  }

  for(int i=0;i<n*8 - 1;i++) {
    printf("%d,%d,%d,%d\n",i/8,i%8, pp[i], pp_test[i + 1]);
    if(pp[i] != pp_test[i + 1])
      return 1;
  }
  return 0;
}