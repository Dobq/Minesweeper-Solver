
#pragma once

#include <cassert>
#include <limits>
#include <random>
#include <vector>

#include "alias\ad.h"
#include "random\engine\xorshift_64.h"

namespace ad {

template<std::uniform_random_bit_generator Engine = Xorshift64<>>
class RNG {

  // For simplicity reasons we are using U64 here, 
  // despite we could use ay integral type compatible with engine.
  static_assert(Engine::min() == std::numeric_limits<U64>::min());
  static_assert(Engine::max() == std::numeric_limits<U64>::max());

  Engine engine_;

 public:

  constexpr static U64 DefaultSeed() {return Engine::DefaultSeed();}

  // Returns U64 smaller than given one,
  // fast implementation with negligible inexactness for small (< 10^13) arguments.
  U64 FastRandomU64(const U64 &max) {
    return engine_() % max;
  }

  // Returns U64 lying in [min, max),
  // fast implementation with negligible inexactness for small (< 10^13) arguments.
  U64 FastRandomU64(const U64 &min, const U64 &max) {
    assert(min < max);
    return min + engine_() % (max - min);
  }

  // Shuffles vector in place, "Fast" in name stands for FastRandomU64 being used,
  // causing function is suitable only for short (< 10^13 elements) vectors, 
  // other than that implementation is exact.
  template<typename T>
  void FastShuffle(std::vector<T> &vec) {
    for (U64 i = 2; i <= vec.size(); i++) {
      std::swap(vec[0], vec[FastRandomU64(i)]);
    }
  }

  // Returns U64 smaller than given one, exact implementation,
  // suitable for single number generation,
  // in case e.g. vector of random numbers needs to be generated,
  // other function should be used.
  U64 ExactRandomU64(const U64 &max) {
    std::uniform_int_distribution<U64> distribution(0, max - 1);
    return distribution(engine_);
  }

  // Returns U64 lying in [min, max), exact implementation,
  // suitable for single number generation,
  // in case e.g. vector of random numbers needs to be generated,
  // other function should be used.
  U64 ExactRandomU64(const U64 &min, const U64 &max) {
    std::uniform_int_distribution<U64> distribution(min, max - 1);
    return distribution(engine_);
  }

  RNG(const U64 &seed = DefaultSeed()):
  engine_(seed) {}

};

}
