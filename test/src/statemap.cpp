#include <cstdio>

#include <paqFe/models/StateMap.hpp>

int main() {
  paqFe::internal::StaticStateMap sm;

  int cnt = 0;
  printf("val map_lut : Vec[UInt] = VecInit ( \n\t");

  for(int i=0;i<256;i++) {
    printf("0x%04X.U, ", sm.predict(i));
    if(i % 8 == 7) {
      putchar('\n');
      putchar('\t');
    }
  }
  printf(")\n\n");

  printf("val shift_lut : Vec[UInt] = VecInit ( \n\t");
  for(int i=0;i<512;i++) {
    printf("0x%02X.U, ", paqFe::internal::StateTable[i >> 1][i & 0x1]);
    if(i % 8 == 7) {
      putchar('\n');
      putchar('\t');
    }
  }
   printf(")\n\n");
  return 0;
}