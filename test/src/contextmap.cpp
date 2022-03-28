#include <cstdio>

#include <paqFe/types.hpp>
#include <paqFe/models/ContextMap.hpp>

using namespace paqFe::internal;
using namespace paqFe;

const char data[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaacca";
constexpr size_t n = sizeof(data);


size_t PassthroughHash(uint64_t val) {
  return val & ((1 << 12) - 1);
}

void display_prob(Prob* pp, size_t cols, size_t rows, int j) {
  for(int i=0;i<rows;i++) {
    char c = i % 8 == 7 ? '\n' : ' ';
    printf("%d,%c", pp[i * cols + j], c);
  }
}
void bits(uint8_t x) {
  for(int i=7;i>=0;i--) {
    uint8_t bit = (x >> i) & 0x1;
    if(bit)
      printf("1 ");
    else
      printf("0 ");
  }
  putchar('\n');
}
int main() {
  ContextMap<PassthroughHash,12> a;
  ContextMap<PassthroughHash,12> b;

  constexpr int cols = a.nProb;
  constexpr int rows = n * 8;
  bits('a');
  Prob* pp = new Prob[cols * rows];
  Context* pctx = new Context[cols * rows];
  Prob* pp_test = new Prob[cols * rows];
  Context* pctx_test = new Context[cols * rows];

  pp[0] = pp[1] = pp[2] = pp[3] = 2048;
  pctx[0] = pctx[1] = pctx[2] = pctx[3] = 0;
  uint64_t ctx = 0;
  for(int i=0;i<n;i++) {
    uint8_t byte = data[i];
    for(int j=7;j>=0;j--) {
      uint8_t bit = (byte >> j) & 0x1;
      int row = i*8 + 7 - j + 1;  // +1 for first row is assumed to be 2048
      a.predict(bit, ctx, &pp[row * cols]);
    }
    ctx = (ctx << 8 | byte) & 0xFF;
  }

  ctx = 0;
  for(int i=0;i<n;i++) {
    uint8_t byte = data[i];
    int row = i*8;
    b.predict_byte(byte, ctx, &pp_test[row * cols], cols);
    ctx = (ctx << 8 | byte) & 0xFF;
  }
  display_prob(pp, cols, rows, 0);
  bits('a');
  /*
  for(int i=0;i<rows;i++) {
    for(int j=0;j<cols;j++) {
      printf("i:%d,j:%d,exp:%d,real:%d\n", i / 8, i %8,pp[i * cols + j],pp_test[i * cols + j]);
      if(pp[i * cols + j] != pp_test[i * cols + j]){
        return 1;
      }
    }
  }*/
  return 0;
}