
#pragma once

#include <cassert>
#include <limits>
#include <random>

#include "alias\ad.h"

namespace ad {

// Implements 64-bit xorshift algorithm,
// default parameter values of 11, 31, 18 are best possible
// according to https://vigna.di.unimi.it/ftp/papers/xorshift.pdf (Page 6)
// and default t_default_seed == -1ull / 641 (what is prime integer btw)
// provides some bit of initial noise.
template<U64 a = 11, U64 b = 31, U64 c = 18, U64 t_default_seed = -1ull / 641>
class Xorshift64 {

  static_assert(a < 64);
  static_assert(b < 64);
  static_assert(c < 64);
  static_assert(t_default_seed > 0);

  U64 state_;

 public:

  using result_type = U64;

  // Smaller by 1 than factual minimum, provides better performance
  // at the cost of very minimal quality loss.
  // Performance is better because of value range being power of 2.
  constexpr static U64 min() {return std::numeric_limits<U64>::min();}

  constexpr static U64 max() {return std::numeric_limits<U64>::max();}

  constexpr static U64 DefaultSeed() {
    return t_default_seed;
  }

  U64 operator()() {
    state_ ^= state_ << a;
    state_ ^= state_ >> b;
    state_ ^= state_ << c;
    return state_;
  }

  Xorshift64(const U64 &seed = DefaultSeed()):
  state_(seed) {
    // seed == 0 would provide constant sequence of 0s.
    assert(state_ != 0);
  }

};

static_assert(std::uniform_random_bit_generator<Xorshift64<>>);  

}
