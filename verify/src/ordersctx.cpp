#include <cstdio>

#include "paqFe/paqFe.hpp"
#include "paqFe/models/Orders.hpp"

using namespace paqFe::internal;

FILE* gfout;

template< size_t O2Size = 1ul << 20,
          size_t O3Size = 1ul << 20,
          size_t O4Size = 1ul << 21>
class OrdersWrapper : public Orders<O2Size, O3Size, O4Size> {
public:
  using Parent = Orders<O2Size, O3Size, O4Size>;

  uint64_t C = 0;
  uint64_t C1 = 0;
  uint64_t C2 = 0;
  uint64_t C3 = 0;
  uint64_t C4 = 0;

  uint32_t h1 = 0;
  uint32_t h2 = 0;
  uint32_t h3 = 0;
  uint32_t h4 = 0;

  bool updateContextNibble0(uint8_t nibble) {
    C = ((C << 4) | nibble);
    C1 = (C & 0xFF) << 5;
    C2 = (C & 0xFFFF) << 5;
    C3 = (C & 0xFFFFFF) << 5;
    C4 = (C & 0xFFFFFFFF) << 5;
    updateHash();

    return true;
  }

  bool updateContextNibble1(uint8_t nibble) {
    C = ((C << 4) | nibble);
    C1 = C1 + nibble + 16;
    C2 = C2 + nibble + 16;
    C3 = C3 + nibble + 16;
    C4 = C4 + nibble + 16;
    updateHash();

    return true;
  }

  void updateHash() {
    h1 = C1 & Parent::O1Mask;
    h2 = tab_hashing<21, 16>(O2HashTab, C2) & Parent::O2Mask;
    h3 = tab_hashing<29, 16>(O3HashTab, C3) & Parent::O3Mask;
    h4 = tab_hashing<34, 17>(O4HashTab, C4) & Parent::O4Mask;
  }


  OrdersWrapper() : Parent() {

  }

  void predict(uint8_t bit, Prob *pp, Context *pctx) {
    Parent::predict(bit, pp, pctx);
  }

  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t pstride, size_t ctxstride) {
    Parent::predict_byte(byte, pp, pctx, pstride, ctxstride);

    uint8_t nibble0 = (byte >> 4) & 0xF;
    uint8_t nibble1 = byte & 0xF;

    fprintf(gfout, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", nibble0, h1, h2, h3, h4, C1 & 0xFF, C2 & 0xFF, C3 & 0xFF, C4 & 0xFF);
    updateContextNibble1(nibble0);

    fprintf(gfout, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", nibble1, h1, h2, h3, h4, C1 & 0xFF, C2 & 0xFF, C3 & 0xFF, C4 & 0xFF);
    updateContextNibble0(nibble1);
  }
};

void generate_db(FILE* fin, FILE* fout) {
  gfout = fout;

  OrdersWrapper<> &model  = *(new OrdersWrapper<>());

  uint8_t data[128];
  size_t n = 0;
  Prob prob = 2048;
  Prob p[8][OrdersWrapper<>::nProb];
  Context ctx[8][OrdersWrapper<>::nCtx];
  while((n = fread(data, 1, 128, fin))) {
    for(int i=0;i<n;i++) {
      uint8_t byte = data[i];
      model.predict_byte(byte, &p[0][0], &ctx[0][0], OrdersWrapper<>::Parent::nProb, OrdersWrapper<>::Parent::nCtx);
    }
  }

  delete &model;
}

