#include "types.hpp"
#include "Engine.hpp"

namespace paqFe {

class paqFeFile {
public:
  //using Predictor = internal::PassThroughtPredictor<internal::MatchModel<4096, 1 << 16, 16>>;
  //using Predictor = internal::PassThroughtPredictor<internal::Orders<>, 3>;
  using Predictor = internal::Predictor<8, internal::Orders<>,
                                           internal::MatchModel<4096, 1 << 16, 16>>;
  //using Predictor = internal::Predictor<8, internal::Orders<>>;
  using Engine = internal::CompressEngineNw<8, Predictor>;
private:
  Engine *engine = nullptr;

public:

  paqFeFile(const char* pathname, OpMode m) {
    engine = new Engine(pathname, m);
  }

  paqFeFile(paqFeFile&& other) {
    engine = other.engine;
    other.engine = nullptr;
  }

  size_t size() {
    return engine->size();
  }

  size_t read(void *dst, size_t n) {
    return engine->read((uint8_t*)(dst), n);
  }

  size_t write(const void *src, size_t n) {
    return engine->write((const uint8_t*)(src), n);
  }

  void close() {
    engine->close();
  }

  ~paqFeFile() {
    if(engine)
      delete engine;
  }
};

}