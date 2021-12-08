#pragma once

#include "../types.hpp"

namespace paqFe::internal {

class Model {
public:

  // how many outputs for one bit
  static constexpr int OutputCnt = 0;

  // init
  Model() {};

  // bit wise prediction for decode
  void predict(uint8_t bit, Prob *pp) {};

  // byte wise prediction matches RTL model
  void predict_byte(uint8_t byte, Prob *pp) {};

  // batch mode for batter cache locality
  void predict_byte_batch(uint8_t *data, size_t size, Prob* pp) {};
};

}