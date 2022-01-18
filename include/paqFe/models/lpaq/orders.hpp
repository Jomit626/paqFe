#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <cstdint>

#include "../../types.hpp"
#include "../StateMap.hpp"

namespace paqFe::internal::lpaq {

typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;

static const U8 State_table[256][2]={
{  1,  2},{  3,  5},{  4,  6},{  7, 10},{  8, 12},{  9, 13},{ 11, 14}, // 0
{ 15, 19},{ 16, 23},{ 17, 24},{ 18, 25},{ 20, 27},{ 21, 28},{ 22, 29}, // 7
{ 26, 30},{ 31, 33},{ 32, 35},{ 32, 35},{ 32, 35},{ 32, 35},{ 34, 37}, // 14
{ 34, 37},{ 34, 37},{ 34, 37},{ 34, 37},{ 34, 37},{ 36, 39},{ 36, 39}, // 21
{ 36, 39},{ 36, 39},{ 38, 40},{ 41, 43},{ 42, 45},{ 42, 45},{ 44, 47}, // 28
{ 44, 47},{ 46, 49},{ 46, 49},{ 48, 51},{ 48, 51},{ 50, 52},{ 53, 43}, // 35
{ 54, 57},{ 54, 57},{ 56, 59},{ 56, 59},{ 58, 61},{ 58, 61},{ 60, 63}, // 42
{ 60, 63},{ 62, 65},{ 62, 65},{ 50, 66},{ 67, 55},{ 68, 57},{ 68, 57}, // 49
{ 70, 73},{ 70, 73},{ 72, 75},{ 72, 75},{ 74, 77},{ 74, 77},{ 76, 79}, // 56
{ 76, 79},{ 62, 81},{ 62, 81},{ 64, 82},{ 83, 69},{ 84, 71},{ 84, 71}, // 63
{ 86, 73},{ 86, 73},{ 44, 59},{ 44, 59},{ 58, 61},{ 58, 61},{ 60, 49}, // 70
{ 60, 49},{ 76, 89},{ 76, 89},{ 78, 91},{ 78, 91},{ 80, 92},{ 93, 69}, // 77
{ 94, 87},{ 94, 87},{ 96, 45},{ 96, 45},{ 48, 99},{ 48, 99},{ 88,101}, // 84
{ 88,101},{ 80,102},{103, 69},{104, 87},{104, 87},{106, 57},{106, 57}, // 91
{ 62,109},{ 62,109},{ 88,111},{ 88,111},{ 80,112},{113, 85},{114, 87}, // 98
{114, 87},{116, 57},{116, 57},{ 62,119},{ 62,119},{ 88,121},{ 88,121}, // 105
{ 90,122},{123, 85},{124, 97},{124, 97},{126, 57},{126, 57},{ 62,129}, // 112
{ 62,129},{ 98,131},{ 98,131},{ 90,132},{133, 85},{134, 97},{134, 97}, // 119
{136, 57},{136, 57},{ 62,139},{ 62,139},{ 98,141},{ 98,141},{ 90,142}, // 126
{143, 95},{144, 97},{144, 97},{ 68, 57},{ 68, 57},{ 62, 81},{ 62, 81}, // 133
{ 98,147},{ 98,147},{100,148},{149, 95},{150,107},{150,107},{108,151}, // 140
{108,151},{100,152},{153, 95},{154,107},{108,155},{100,156},{157, 95}, // 147
{158,107},{108,159},{100,160},{161,105},{162,107},{108,163},{110,164}, // 154
{165,105},{166,117},{118,167},{110,168},{169,105},{170,117},{118,171}, // 161
{110,172},{173,105},{174,117},{118,175},{110,176},{177,105},{178,117}, // 168
{118,179},{110,180},{181,115},{182,117},{118,183},{120,184},{185,115}, // 175
{186,127},{128,187},{120,188},{189,115},{190,127},{128,191},{120,192}, // 182
{193,115},{194,127},{128,195},{120,196},{197,115},{198,127},{128,199}, // 189
{120,200},{201,115},{202,127},{128,203},{120,204},{205,115},{206,127}, // 196
{128,207},{120,208},{209,125},{210,127},{128,211},{130,212},{213,125}, // 203
{214,137},{138,215},{130,216},{217,125},{218,137},{138,219},{130,220}, // 210
{221,125},{222,137},{138,223},{130,224},{225,125},{226,137},{138,227}, // 217
{130,228},{229,125},{230,137},{138,231},{130,232},{233,125},{234,137}, // 224
{138,235},{130,236},{237,125},{238,137},{138,239},{130,240},{241,125}, // 231
{242,137},{138,243},{130,244},{245,135},{246,137},{138,247},{140,248}, // 238
{249,135},{250, 69},{ 80,251},{140,252},{249,135},{250, 69},{ 80,251}, // 245
{140,252},{  0,  0},{  0,  0},{  0,  0}};  // 252
#define nex(state,sel) State_table[state][sel]

template< size_t O2Size = 1ul << 20,
          size_t O3Size = 1ul << 20,
          size_t O4Size = 1ul << 21>
class Orders {
  static constexpr size_t O1Size = 1ul << 16;
#define DECARE_SIZE(name) \
  static constexpr size_t name##SizeMask = name##Size - 1;  \
  static_assert(isPow2(name##Size)); 

  DECARE_SIZE(O1)
  DECARE_SIZE(O2)
  DECARE_SIZE(O3)
  DECARE_SIZE(O4)

#undef DECARE_SIZE

  uint8_t table0[0x10000];
  uint8_t table1[O2Size];
  uint8_t table2[O3Size];
  uint8_t table3[O4Size];

  int c0=1;
  int c4=0;
  uint8_t *cp[6];
  uint32_t h[6] = {0};
  int bcount=0;

public:
  static constexpr int nProb = 4;
  static constexpr int nCtx = 1;

  Prob p[nProb];
  Context ctx[nCtx];
  Orders() {
    memset(table0, 0, sizeof(table0));
    memset(table1, 0, sizeof(table1));
    memset(table2, 0, sizeof(table2));
    memset(table3, 0, sizeof(table3));

    for(int i=0;i<6;i++)
      cp[i] = table0;
    for(int i=0;i<nProb;i++)
      p[i] = ProbEven;
    for(int i=0;i<nCtx;i++)
      ctx[i] = 0;
  }

  void predict(uint8_t y, Prob* pp, Context *pctx) {
    *cp[0]=nex(*cp[0], y);
    *cp[1]=nex(*cp[1], y);
    *cp[2]=nex(*cp[2], y);
    *cp[3]=nex(*cp[3], y);
    // update context
    ++bcount;
    c0+=c0+y;
    if (c0>=256) {
      c0-=256;
      c4=c4<<8|c0;
      h[0]=c0<<8;  // order 1
      h[1]=(c4&0xffff)<<5|0x57000000;  // order 2
      h[2]=(c4<<8)*3;  // order 3
      h[3]=c4*5;  // order 4
      h[4]=h[4]*(11<<5)+c0*13&0x3fffffff;  // order 6
      if (c0>=65 && c0<=90) c0+=32;  // lowercase unigram word order
      if (c0>=97 && c0<=122) h[5]=(h[5]+c0)*(7<<3);
      else h[5]=0;
      cp[1]= table_find(table1, O2SizeMask, h[1]) + 1;
      cp[2]= table_find(table2, O3SizeMask, h[2]) + 1;
      cp[3]= table_find(table3, O4SizeMask, h[3]) + 1;
      c0=1;
      bcount=0;
    }
    if (bcount==4) {
      cp[1]= table_find(table1, O2SizeMask, h[1] + c0) + 1;
      cp[2]= table_find(table2, O3SizeMask, h[2] + c0) + 1;
      cp[3]= table_find(table3, O4SizeMask, h[3] + c0) + 1;
    }
    else if (bcount>0) {
      int j=y+1<<(bcount&3)-1;
      cp[1]+=j;
      cp[2]+=j;
      cp[3]+=j;
    }
    cp[0]=table0+h[0]+c0;
    
    pp[0] = StaticStateMap::map[*cp[0]];
    pp[1] = StaticStateMap::map[*cp[1]];
    pp[2] = StaticStateMap::map[*cp[2]];
    pp[3] = StaticStateMap::map[*cp[3]];

    *pctx = 0;
  } 

  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t pstride, size_t ctxstride) {
    for(int j=0;j<8;j++) {
      for(int i=0;i<nProb;i++)
        pp[i] = p[i];

      for(int i=0;i<nCtx;i++)
        pctx[i] = ctx[i];

      uint8_t bit = (byte >> (7-j)) & 0x1;
      predict(bit, p, ctx);

      pp += pstride;
      pctx += ctxstride;
    }
  }
  
private:
  uint8_t* table_find(uint8_t *table, size_t Mask, uint32_t val) {
    uint32_t hash = val;
    hash*=123456791;
    hash=hash<<16|hash>>16;
    hash*=234567891;
    int chk=val>>24;
    hash=(hash*16)&Mask;
    if (table[hash]==chk) return &table[hash];
    //if (t[i^B]==chk) return t+(i^B);
    //if (t[i^B*2]==chk) return t+(i^B*2);
    //if (t[i+1]>t[i+1^B] || t[i+1]>t[i+1^B*2]) i^=B;
    //if (t[i+1]>t[i+1^B^B*2]) i^=B^B*2;
    memset(&table[hash], 0, 16);
    table[hash]=chk;
    return &table[hash];
  }
};

}