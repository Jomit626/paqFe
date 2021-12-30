#include <cstdio>
#include <cmath>
#include <paqFe/models/Orders.hpp>

using namespace paqFe::internal;

template<size_t N>
void eval(FILE* fin, FILE* fout) {
  fseek(fin, SEEK_SET, 0);
  Orders<N,N,N> &model = *(new Orders<N,N,N>);

  double error[model.OutputCnt] = {0.0};
  int cnt = 0;

  uint8_t buf[128];
  Prob p[8 * model.OutputCnt];
  Context ctx[8 * model.OutputCnt];
  size_t n = fread(buf, 1, sizeof(buf), fin);
  while(n > 0) {

    for(int i=0;i<n;i++) {
      uint8_t byte = buf[i];
      model.predict_byte(byte, p, ctx);
      for(int j=0;j<8;j++) {
        int bit = (byte >> (7 - j)) & 0x1;
        for(int k=0;k<model.OutputCnt;k++) {
          Prob ps = p[j * model.OutputCnt + k];

          double pd = ps / 4096.0;
          pd = bit ? (1.0 - pd) : pd;
          error[k] += pd;
        }
      }
    }

    cnt += 8 * n;
    n = fread(buf, 1, sizeof(buf), fin);
  }

  for(int k=0;k<model.OutputCnt;k++)
    error[k] /= cnt;
  
  fprintf(fout, "%d,\t%lf,\t%lf,\t%lf,\t%lf\n", int(log2(double(N))), error[0], error[1], error[2], error[3]);
  delete &model;
}

void start(FILE* fin, FILE* fout) {
  fprintf(fout, "size,order1,order2,order3,order4\n");
  eval<1ul << 12>(fin, fout);
  eval<1ul << 13>(fin, fout);
  eval<1ul << 14>(fin, fout);
  eval<1ul << 15>(fin, fout);
  eval<1ul << 16>(fin, fout);
  eval<1ul << 17>(fin, fout);
  eval<1ul << 18>(fin, fout);
  eval<1ul << 19>(fin, fout);
  eval<1ul << 20>(fin, fout);
  eval<1ul << 21>(fin, fout);
  eval<1ul << 22>(fin, fout);
  eval<1ul << 23>(fin, fout);
  eval<1ul << 24>(fin, fout);
  eval<1ul << 25>(fin, fout);
  eval<1ul << 26>(fin, fout);
  eval<1ul << 27>(fin, fout);
}