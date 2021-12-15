#pragma once

#include "../types.hpp"

namespace paqFe::internal {

class Model {
public:

  // how many outputs for one bit
  static constexpr int OutputCnt = 0;

  // init
  Model() {};

  /* bitwise prediction for decode
   * given a bit predict the prob of next bit being 1
   * the prob of very first bit is assumed to be [[ProbEven]]
  */
  void predict(uint8_t bit, Prob *pp) {};

  /* bytewise prediction matches RTL model
   * given a byte, predict the prob of the bits being 1 of that byte
   * !!!
   * there is a one bit offset between 
   * @example:
   * predict_byte(byte, pp) will return with:
   * pp[0] = p0
   * pp[1] = p1
   * ...
   * 
   * for(int j=7;j>=0;j++)
   *  predict((byte >> j) & 0x1, pp[7 - j])
   * will cause
   * pp[0] = p1
   * pp[1] = p2
   * ...
   * !!!
  */
  void predict_byte(uint8_t byte, Prob *pp) {};

  // batch mode for batter cache locality
  void predict_byte_batch(uint8_t *data, size_t size, Prob* pp) {};
};

}