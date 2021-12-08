#include <cstdio>

#include <paqFe/models/StateMap.hpp>

class StateMap {
protected:
  const int N;  // Number of contexts
  int cxt;      // Context of last prediction
  uint32_t *t;       // cxt -> prediction in high 22 bits, count in low 10 bits
  static int dt[1024];  // i -> 16K/(i+3)
  void update(int y, int limit) {
    int n=t[cxt]&1023, p=t[cxt]>>10;  // count, prediction
    if (n<limit) ++t[cxt];
    else t[cxt]=t[cxt]&0xfffffc00|limit;
    t[cxt]+=(((y<<22)-p)>>3)*dt[n]&0xfffffc00;
  }
public:
  StateMap(int n=256);

  // update bit y (0..1), predict next bit in context cx
  int p(int y, int cx, int limit=1023) {
    update(y, limit);
    return t[cxt=cx]>>20;
  }
};

int StateMap::dt[1024]={0};

StateMap::StateMap(int n): N(n), cxt(0) {
  t = new uint32_t[N];
  for (int i=0; i<N; ++i)
    t[i]=1<<31;
  if (dt[0]==0)
    for (int i=0; i<1024; ++i)
      dt[i]=16384/(i+i+3);
}

int main() {
  StateMap sm_lpaq1;
  paqFe::internal::StateMap<256> sm_paqfe;

  int ctx = 1;
  for(int i=0;i<1024;i++) {
    int bit = (i >> 2) & 0x1;
    int p = sm_lpaq1.p(bit ,ctx);
    int p1 = sm_paqfe.predict(bit ,ctx);
    printf("%d, %d, %d\n", i, p, p1);

    int delta = p - p1;
    if(delta < 0)
      delta = -delta;
    
    if(delta > 2)
      return 1;
  }
  return 0;
}