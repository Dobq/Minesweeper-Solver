
#include <testing\catch2\catch.hpp>

#include <algorithm>
#include <set>
#include <vector>

#include "alias\ad.h"
#include "random\rng\rng.h"

namespace ad {

// Checks if min <= val && val < max.
void CheckThatLiesWithinRange(U64 min, U64 max, U64 val) {
  CHECK(min <= val);
  CHECK(val < max);
}

TEST_CASE("RNG random U64 stays within range", "[random]") {

  RNG<> rng;

  for (int i = 1; i <= 20; i++) {
    CheckThatLiesWithinRange(0, i, rng.FastRandomU64(i));
    CheckThatLiesWithinRange(i, 28, rng.FastRandomU64(i, 28));
    CheckThatLiesWithinRange(42, 42 + i, rng.FastRandomU64(42, 42 + i));
    CheckThatLiesWithinRange(0, i, rng.ExactRandomU64(i));
    CheckThatLiesWithinRange(i, 28, rng.ExactRandomU64(i, 28));
    CheckThatLiesWithinRange(42, 42 + i, rng.ExactRandomU64(42, 42 + i));
  }

}

TEST_CASE("RNG random U64 generates everything within range", "[random]") {
  
  RNG<> rng(28);

  std::set<U64> set_1, set_2, set_3, set_4;

  for (int i = 0; i < 128; i++) {
    set_1.insert(rng.FastRandomU64(8));
    set_2.insert(rng.FastRandomU64(8, 16));
    set_3.insert(rng.ExactRandomU64(16, 24));
    set_4.insert(rng.ExactRandomU64(24, 32));
  }

  // Some of checks below may be violated for about 1 seed in 10^7.
  CHECK(set_1.size() == 8);
  CHECK(set_2.size() == 8);
  CHECK(set_3.size() == 8);
  CHECK(set_4.size() == 8);

}

TEST_CASE("RNG shuffle", "[random]") {

  RNG<> rng(6);

  const std::vector<U64> vec_ref = {0, 1, 2, 3, 12, 13, 14, 15};
  std::vector<U64> vec_test = vec_ref;

  U64 diff_count = 0;

  for (int i = 1; i <= 10; i++) {
    rng.FastShuffle<U64>(vec_test);
    for (U64 j = 0; j < vec_ref.size(); j++) {
      diff_count += vec_ref[j] != vec_test[j];
    }
    std::sort(vec_test.begin(), vec_test.end());
    CHECK(vec_ref == vec_test);
  }

  // Some check if vector gets shuffled "well",
  // CHECK below may be violated for very rare seeds.
  CHECK(diff_count >= 25);

}

}
