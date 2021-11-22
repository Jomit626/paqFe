#pragma once

#include "Mixer.hpp"
#include "models/PresudoModel.hpp"
#include "models/MatchModel.hpp"
#include "models/StateModel.hpp"

#include <type_traits>

namespace paqFe {

template<typename ... Models>
class PredictorWithOutMixer : public Models... {

};

template<typename ... Models>
class Predictor : public Mixer<Models...> {

};

}