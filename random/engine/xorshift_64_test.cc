
#include <testing\catch2\catch.hpp>

#include "alias\ad.h"
#include "random\engine\xorshift_64.h"

namespace ad {

TEST_CASE("Xorshift64 calls", "[random]") {
  
  // Testing default and nondefault arguments. 
  Xorshift64<> rbg_1;
  Xorshift64<> rbg_2(42);
  Xorshift64<8, 29, 19> rbg_3;
  Xorshift64<8, 29, 19> rbg_4(42);

  // We consider generators are fine just if they 
  // are callable and return anything other than 0ull. 
  for (int i = 0; i < 10; i++) {
    REQUIRE(rbg_1());
    REQUIRE(rbg_2());
    REQUIRE(rbg_3());
    REQUIRE(rbg_4());
  }

}

}
