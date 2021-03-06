#include "types.hpp"
#include "Engine.hpp"
#include "models/Group.hpp"
#include "models/SimpleBytePredict.hpp"
#include "models/NormalModel.hpp"
#include "models/BiasModel.hpp"
#include "models/Orders.hpp"
#include "Mixer.hpp"

namespace paqFe {

class paqFeFile {
public:
  using Model = internal::ModelGroup<
                          internal::OrdersDefault
                          //internal::ModelSimpleBytePredict<internal::NormalModel<>>
                          //internal::ModelSimpleBytePredict<internal::Orders<12,16,16,17,17>>
                          >;
  using Mixer = internal::Mixer<Model::nProb, Model::nCtx>;
  using Predictor = internal::Predictor<8, Model, Mixer>;
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