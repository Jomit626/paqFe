#pragma once

#include "Mixer.hpp"
#include "models/PresudoModel.hpp"

#include <type_traits>

namespace paqFe {

template<typename ... Models>
class PredictorWithMixer : public Mixer<Models...> {

};

template<typename ... Models>
class Predictor : public std::conditional< sizeof...(Models)>=2, Mixer<Models...>, Models...>::type {

};

typedef Predictor<PresudoModel<ProbEven>> DefaultPredictor;

}