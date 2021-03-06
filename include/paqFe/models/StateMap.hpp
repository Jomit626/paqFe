#pragma once

#include "../types.hpp"
#include <cassert>

namespace paqFe::internal {

static constexpr uint8_t StateTable[256][4] = {
  {  1,  2, 0, 0},{  3,  5, 1, 0},{  4,  6, 0, 1},{  7, 10, 2, 0}, // 0-3
  {  8, 12, 1, 1},{  9, 13, 1, 1},{ 11, 14, 0, 2},{ 15, 19, 3, 0}, // 4-7
  { 16, 23, 2, 1},{ 17, 24, 2, 1},{ 18, 25, 2, 1},{ 20, 27, 1, 2}, // 8-11
  { 21, 28, 1, 2},{ 22, 29, 1, 2},{ 26, 30, 0, 3},{ 31, 33, 4, 0}, // 12-15
  { 32, 35, 3, 1},{ 32, 35, 3, 1},{ 32, 35, 3, 1},{ 32, 35, 3, 1}, // 16-19
  { 34, 37, 2, 2},{ 34, 37, 2, 2},{ 34, 37, 2, 2},{ 34, 37, 2, 2}, // 20-23
  { 34, 37, 2, 2},{ 34, 37, 2, 2},{ 36, 39, 1, 3},{ 36, 39, 1, 3}, // 24-27
  { 36, 39, 1, 3},{ 36, 39, 1, 3},{ 38, 40, 0, 4},{ 41, 43, 5, 0}, // 28-31
  { 42, 45, 4, 1},{ 42, 45, 4, 1},{ 44, 47, 3, 2},{ 44, 47, 3, 2}, // 32-35
  { 46, 49, 2, 3},{ 46, 49, 2, 3},{ 48, 51, 1, 4},{ 48, 51, 1, 4}, // 36-39
  { 50, 52, 0, 5},{ 53, 43, 6, 0},{ 54, 57, 5, 1},{ 54, 57, 5, 1}, // 40-43
  { 56, 59, 4, 2},{ 56, 59, 4, 2},{ 58, 61, 3, 3},{ 58, 61, 3, 3}, // 44-47
  { 60, 63, 2, 4},{ 60, 63, 2, 4},{ 62, 65, 1, 5},{ 62, 65, 1, 5}, // 48-51
  { 50, 66, 0, 6},{ 67, 55, 7, 0},{ 68, 57, 6, 1},{ 68, 57, 6, 1}, // 52-55
  { 70, 73, 5, 2},{ 70, 73, 5, 2},{ 72, 75, 4, 3},{ 72, 75, 4, 3}, // 56-59
  { 74, 77, 3, 4},{ 74, 77, 3, 4},{ 76, 79, 2, 5},{ 76, 79, 2, 5}, // 60-63
  { 62, 81, 1, 6},{ 62, 81, 1, 6},{ 64, 82, 0, 7},{ 83, 69, 8, 0}, // 64-67
  { 84, 71, 7, 1},{ 84, 71, 7, 1},{ 86, 73, 6, 2},{ 86, 73, 6, 2}, // 68-71
  { 44, 59, 5, 3},{ 44, 59, 5, 3},{ 58, 61, 4, 4},{ 58, 61, 4, 4}, // 72-75
  { 60, 49, 3, 5},{ 60, 49, 3, 5},{ 76, 89, 2, 6},{ 76, 89, 2, 6}, // 76-79
  { 78, 91, 1, 7},{ 78, 91, 1, 7},{ 80, 92, 0, 8},{ 93, 69, 9, 0}, // 80-83
  { 94, 87, 8, 1},{ 94, 87, 8, 1},{ 96, 45, 7, 2},{ 96, 45, 7, 2}, // 84-87
  { 48, 99, 2, 7},{ 48, 99, 2, 7},{ 88,101, 1, 8},{ 88,101, 1, 8}, // 88-91
  { 80,102, 0, 9},{103, 69,10, 0},{104, 87, 9, 1},{104, 87, 9, 1}, // 92-95
  {106, 57, 8, 2},{106, 57, 8, 2},{ 62,109, 2, 8},{ 62,109, 2, 8}, // 96-99
  { 88,111, 1, 9},{ 88,111, 1, 9},{ 80,112, 0,10},{113, 85,11, 0}, // 100-103
  {114, 87,10, 1},{114, 87,10, 1},{116, 57, 9, 2},{116, 57, 9, 2}, // 104-107
  { 62,119, 2, 9},{ 62,119, 2, 9},{ 88,121, 1,10},{ 88,121, 1,10}, // 108-111
  { 90,122, 0,11},{123, 85,12, 0},{124, 97,11, 1},{124, 97,11, 1}, // 112-115
  {126, 57,10, 2},{126, 57,10, 2},{ 62,129, 2,10},{ 62,129, 2,10}, // 116-119
  { 98,131, 1,11},{ 98,131, 1,11},{ 90,132, 0,12},{133, 85,13, 0}, // 120-123
  {134, 97,12, 1},{134, 97,12, 1},{136, 57,11, 2},{136, 57,11, 2}, // 124-127
  { 62,139, 2,11},{ 62,139, 2,11},{ 98,141, 1,12},{ 98,141, 1,12}, // 128-131
  { 90,142, 0,13},{143, 95,14, 0},{144, 97,13, 1},{144, 97,13, 1}, // 132-135
  { 68, 57,12, 2},{ 68, 57,12, 2},{ 62, 81, 2,12},{ 62, 81, 2,12}, // 136-139
  { 98,147, 1,13},{ 98,147, 1,13},{100,148, 0,14},{149, 95,15, 0}, // 140-143
  {150,107,14, 1},{150,107,14, 1},{108,151, 1,14},{108,151, 1,14}, // 144-147
  {100,152, 0,15},
  // contexts representing strong trend of 0s or 1s start from here
  {153, 95,16, 0},{154, 69,15, 1},{ 80,155, 1,15},{100,156, 0,16}, // 149-152
  {157, 95,17, 0},{158, 69,16, 1},{ 80,159, 1,16},{100,160, 0,17}, // 153-156
  {161, 95,18, 0},{162, 69,17, 1},{ 80,163, 1,17},{100,164, 0,18}, // 157-160
  {165, 95,19, 0},{166, 69,18, 1},{ 80,167, 1,18},{100,168, 0,19}, // 161-164
  {169, 95,20, 0},{170, 69,19, 1},{ 80,171, 1,19},{100,172, 0,20}, // 165-168
  {173, 95,21, 0},{174, 69,20, 1},{ 80,175, 1,20},{100,176, 0,21}, // 169-172
  {177, 95,22, 0},{178, 69,21, 1},{ 80,179, 1,21},{100,180, 0,22}, // 173-176
  {181, 95,23, 0},{182, 69,22, 1},{ 80,183, 1,22},{100,184, 0,23}, // 177-180
  {185, 95,24, 0},{186, 69,23, 1},{ 80,187, 1,23},{100,188, 0,24}, // 181-184
  {189, 95,25, 0},{190, 69,24, 1},{ 80,191, 1,24},{100,192, 0,25}, // 185-188
  {193, 95,26, 0},{194, 69,25, 1},{ 80,195, 1,25},{100,196, 0,26}, // 189-192
  {197, 95,27, 0},{198, 69,26, 1},{ 80,199, 1,26},{100,200, 0,27}, // 193-196
  {201, 95,28, 0},{202, 69,27, 1},{ 80,203, 1,27},{100,204, 0,28}, // 197-200
  {205, 95,29, 0},{206, 69,28, 1},{ 80,207, 1,28},{100,208, 0,29}, // 201-204
  // contexts with incremental counting start from here
  {209, 95,30, 0},{210, 69,29, 1},{ 80,211, 1,29},{100,212, 0,30}, // 205-208
  {213, 95,31, 0},{214, 69,30, 1},{ 80,215, 1,30},{100,216, 0,31}, // 209-212
  {217, 95,32, 0},{218, 69,31, 1},{ 80,219, 1,31},{100,220, 0,32}, // 213-216
  {221, 95,33, 0},{222, 69,32, 1},{ 80,223, 1,32},{100,224, 0,33}, // 217-220
  {225, 95,34, 0},{226, 69,33, 1},{ 80,227, 1,33},{100,228, 0,34}, // 221-224
  {229, 95,35, 0},{230, 69,34, 1},{ 80,231, 1,34},{100,232, 0,35}, // 225-228
  {233, 95,36, 0},{234, 69,35, 1},{ 80,235, 1,35},{100,236, 0,36}, // 229-232
  {237, 95,37, 0},{238, 69,36, 1},{ 80,239, 1,36},{100,240, 0,37}, // 233-236
  {241, 95,38, 0},{242, 69,37, 1},{ 80,243, 1,37},{100,244, 0,38}, // 237-240
  {245, 95,39, 0},{246, 69,38, 1},{ 80,247, 1,38},{100,248, 0,39}, // 241-244
  {249, 95,40, 0},{250, 69,39, 1},{ 80,251, 1,39},{100,252, 0,40}, // 245-248
  {249, 95,41, 0},{250, 69,40, 1},{ 80,251, 1,40},{100,252, 0,41}, // 249-252
  {1,2, 0,0},{1,2, 0,0},{1,2, 0,0} // 253-255 are unused (and in case we reach such a state we'll need to restart)
};

static constexpr uint8_t StateGroup[256] = {
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14, // 0-14
  18,23,24,24,26,25,27,26,26,25,27,26,28,28,29,15, //15-30
  18,23,25,25,26,26,27,27,29,15, //31-40
  18,23,25,24,25,26,26,27,28,27,29,15, //41-52
  18,23,24,24,24,25,25,27,27,28,28,28,29,15, //53-66
  19,22,24,23,24,25,25,26,26,27,27,28,29,28,30,16, //67-82
  19,22,24,23,24,28,29,28,30,16, //83-92
  19,22,24,23,24,28,29,28,30,16, //93-102
  19,22,24,23,24,28,29,28,30,16, //103-112
  19,22,24,23,24,28,29,28,30,16, //113-122
  19,22,24,22,23,29,30,28,30,16, //123-132
  19,22,23,22,23,29,30,29,30,16, //133-142
  19,22, 0, 0,30,16, //143-148
  19,22,30,16, 19,22,30,16, 19,22,30,16, 19,22,30,16, 19,22,30,16, //149-152 153-156 157-160 161-164 165-168 
  19,21,31,16, 19,21,31,16, 19,21,31,16, 19,21,31,16, 19,21,31,16, //169-172 173-176 177-180 181-184 185-188 
  19,21,31,16, 19,21,31,16, 19,21,31,16, 19,21,31,16, 19,21,31,17, //189-192 193-196 197-200 201-204 205-208 
  20,21,31,17, 20,21,31,17, 20,21,31,17, 20,21,31,17, 20,21,31,17, //209-212 213-216 217-220 221-224 225-228
  20,21,31,17, 20,21,31,17, 20,21,31,17, 20,21,31,17, 20,21,31,17, //229-232 233-236 237-240 241-244 245-248
  20,21,31,17, 0,0,0, //249-252 253-255
};

class State {
  uint8_t s;
public:

  void next(uint8_t bit){
    s = StateTable[s][bit];
  }

  int zero_cnt() {
    return StateTable[s][2];
  }
  
  int one_cnt() {
    return StateTable[s][3];
  }

  uint8_t group() const {
    return StateGroup[s];
  }

  operator uint8_t() {
    return s;
  }
};


template<size_t size>
class StateMap {
public:
  struct StateInfo
  {
    int32_t prob;
    int32_t count;
  };
  
private:

  static_assert(isPow2(size));
  static constexpr size_t SizeMask = size - 1;

  uint32_t state_prev = 0;
  StateInfo infos[size];

public:
  StateMap() {
    for(int i=0;i<size;i++) {
      infos[i].prob = (1 << 21);
      infos[i].count = 0;
    }
  };

  void predict(uint32_t state, Prob *pp) {
    state_prev = state & SizeMask;
    *pp = infos[state_prev].prob >> 10;
  }

  void update(uint8_t bit) {
    StateInfo &info = infos[state_prev];
    info.prob += (((int32_t)bit << 22) - info.prob) * 2 / (2 * info.count + 3);

    if(info.count < 1023) {
      info.count = info.count + 1;
    }
  }

  /*
  ~StateMap() {
    printf("Prob state_map[256] = {\n\t");
    for(int i=0;i<256;i++) {
      printf("0X%X, ", infos[i].prob >> 10);
      if(((i + 1) % 8) == 0)
        printf("\n\t");
    }
    printf("};\n");
  }
  */

};

class StaticStateMap {
public:
  const static Prob map[256];
  Prob predict(uint8_t bit, uint32_t state) {
    assert(state < 256);
    return map[state];
  }
  
  Prob predict(uint32_t state) {
    assert(state < 256);
    return map[state];
  }

  Prob operator[](uint32_t state) {
    return map[state];
  }
};

const Prob StaticStateMap::map[256] = {
  0X78B, 0X231, 0XD08, 0X14D, 0X678, 0X8F3, 0XE4D, 0XC9, 
  0X473, 0X73E, 0X6E2, 0X7EC, 0XAF3, 0XA0F, 0XF22, 0XB0, 
  0X4A2, 0X2DF, 0X3C4, 0X678, 0X800, 0X91F, 0X88D, 0X800, 
  0X88D, 0X735, 0XAE0, 0XB64, 0XA76, 0XA55, 0XEF8, 0X95, 
  0X387, 0X308, 0X725, 0X68B, 0X874, 0XA0A, 0XB64, 0XBD1, 
  0XF6A, 0X70, 0X2B6, 0X419, 0X5B7, 0X500, 0X835, 0X6FB, 
  0XB4A, 0XA22, 0XD2E, 0XCF2, 0XF47, 0X50, 0X2E2, 0X3E2, 
  0X51F, 0X489, 0X6FC, 0X696, 0X971, 0X86E, 0XC12, 0XB0A, 
  0XC53, 0XD93, 0XF82, 0X3F, 0X281, 0X1A9, 0X488, 0X402, 
  0X662, 0X5BB, 0X81B, 0X771, 0XAA1, 0X95B, 0XD40, 0XBDC, 
  0XE9B, 0XDB8, 0XFAF, 0X3C, 0X1F8, 0XA2, 0X4BC, 0X3C2, 
  0XD75, 0XC3B, 0XB59, 0XDC6, 0XF96, 0X2F, 0X215, 0XBA, 
  0X3C8, 0X33C, 0XD1C, 0XCA2, 0XE53, 0XDB6, 0XFC6, 0X57, 
  0X1ED, 0X46, 0X3A8, 0X29B, 0XD34, 0XD37, 0XC25, 0XDC7, 
  0XFB3, 0X18, 0X1BC, 0X124, 0X31E, 0X274, 0XE21, 0XD81, 
  0XFA6, 0XE15, 0XFB9, 0X40, 0X1D4, 0X114, 0X307, 0X232, 
  0XDCB, 0XDE2, 0XD1E, 0XE3A, 0XFDA, 0X3D, 0X191, 0XA7, 
  0X2B7, 0X1D7, 0XE37, 0XDB2, 0XE0C, 0XE59, 0XFB3, 0X20, 
  0X157, 0X800, 0X800, 0XE8C, 0XFC3, 0X2E, 0X164, 0XE75, 
  0XFCC, 0X21, 0X130, 0XEAE, 0XFDE, 0XE, 0XF6, 0XECB, 
  0XFD3, 0X15, 0X15A, 0XEE2, 0XFE7, 0XF, 0XF8, 0XEE2, 
  0XFF2, 0X1D, 0XD2, 0XEE1, 0XFD0, 0X1D, 0XEA, 0XEFD, 
  0XFE6, 0X25, 0X104, 0XEF0, 0XFE5, 0X10, 0X116, 0XEE9, 
  0XFE4, 0X9, 0XB5, 0XF0D, 0XFF0, 0X10, 0XD5, 0XF1A, 
  0XFCB, 0X20, 0X11C, 0XF03, 0XFF0, 0X2, 0XE4, 0XF57, 
  0XFFC, 0X19, 0XB4, 0XF37, 0XFEF, 0X2, 0XCF, 0XF50, 
  0XFEF, 0X1A, 0X84, 0XF27, 0XFD3, 0XA, 0XBC, 0XF59, 
  0XFDF, 0XA, 0X8F, 0XF69, 0XFED, 0X13, 0XC6, 0XF13, 
  0XFED, 0XA, 0X8E, 0XF59, 0XFDE, 0X13, 0X9B, 0XF4B, 
  0XFFC, 0X13, 0X91, 0XF98, 0XFEC, 0X40, 0X6E, 0XF77, 
  0XFEC, 0X2, 0X82, 0XF62, 0XFFB, 0X27, 0X7E, 0XF64, 
  0XFFB, 0X1, 0X28, 0XFDD, 0XFF8, 0X800, 0X800, 0X800, 
};

}