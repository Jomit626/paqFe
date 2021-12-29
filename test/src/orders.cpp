#include <cstdio>

#include <paqFe/types.hpp>
#include <paqFe/models/Orders.hpp>

using namespace paqFe::internal;
using namespace paqFe;

constexpr size_t n = 100000;

int main() {
  Orders a;
  Orders b;

  constexpr int cols = a.OutputCnt;
  constexpr int rows = n * 8;

  Prob* pp = new Prob[cols * rows];
  Context* pctx = new Context[cols * rows];
  Prob* pp_test = new Prob[cols * rows];
  Context* pctx_test = new Context[cols * rows];

  pp[0] = pp[1] = pp[2] = pp[3] = 2048;
  pctx[0] = pctx[1] = pctx[2] = pctx[3] = 0;
  for(int i=0;i<n;i++) {
    uint8_t byte = i;
    for(int j=7;j>=0;j--) {
      uint8_t bit = (byte >> j) & 0x1;
      int row = i*8 + 7 - j + 1;  // +1 for first row is assumed to be 2048
      a.predict(bit, &pp[row * cols], &pctx[row * cols]);
    }
  }

  for(int i=0;i<n;i++) {
    uint8_t byte = i;
    int row = i*8;
    b.predict_byte(byte, &pp_test[row * cols], &pctx_test[row * cols], cols);
  }

  for(int i=0;i<rows;i++) {
    for(int j=0;j<cols;j++) {
      //printf("i:%d,j:%d,exp:%d,real:%d\n", i, j,pctx[i * cols + j],pctx_test[i * cols + j]);
      if(pp[i * cols + j] != pp_test[i * cols + j]){
        return 1;
      }
      if(pctx[i * cols + j] != pctx_test[i * cols + j]){
        return 1;
      }
    }
  }
  return 0;
}