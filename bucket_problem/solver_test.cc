
#include <testing\catch2\catch.hpp>

#include <algorithm>
#include <string>
#include <vector>

#include "alias\ad.h"
#include "bucket_problem\solver.h"

// TODO: Think about reducing code duplication below.

namespace ad {

TEST_CASE("Bucket problem example 0", "[bucket_problem]") {

  // There are no balls in bucket that consists of slots a and b.
  //
  // Solver is meant to conclude that there is no slot 
  // with a ball in it.

  using Slot = std::string;
  using Identifier = std::string;

  using Bucket = Bucket<Slot, Identifier>;
  using BucketProblemSolver = BucketProblemSolver<Slot, Identifier>;

  Bucket bucket("bucket", 0, {"a", "b"});

  BucketProblemSolver solver;

  solver.Process(bucket);
  
  std::vector<Slot> concluded_false = solver.ArrangementSlotsConcludedAs(0);
  std::vector<Slot> concluded_true = solver.ArrangementSlotsConcludedAs(1);
  
  CHECK((
    concluded_false == std::vector<Slot> {"a", "b"} ||
    concluded_false == std::vector<Slot> {"b", "a"}
  ));
  CHECK(concluded_true == std::vector<Slot> {});

  // This is called just to check that the call changes nothing.
  solver.Process(0, {});

  concluded_false = solver.ArrangementSlotsConcludedAs(0);
  concluded_true = solver.ArrangementSlotsConcludedAs(1);
  
  CHECK((
    concluded_false == std::vector<Slot> {"a", "b"} ||
    concluded_false == std::vector<Slot> {"b", "a"}
  ));
  CHECK(concluded_true == std::vector<Slot> {});
  
}

TEST_CASE("Bucket problem example 1", "[bucket_problem]") {

  // There is a single ball in small_bucket that consists of slots a and b,
  // and there are two balls in big_bucket that consists of slots a, b and c.
  //
  // Solver is meant to conclude that there is no slot that is for sure empty,
  // and that for sure there is ball in slot c.

  using Slot = std::string;
  using Identifier = std::string;

  using Bucket = Bucket<Slot, Identifier>;
  using BucketProblemSolver = BucketProblemSolver<Slot, Identifier>;

  Bucket small_bucket("small_bucket", 1, {"a", "b"});
  Bucket big_bucket("big_bucket", 2, {"a", "b", "c"});

  BucketProblemSolver solver;

  solver.Process(small_bucket);
  solver.Process(big_bucket);
  
  std::vector<Slot> concluded_false = solver.ArrangementSlotsConcludedAs(0);
  std::vector<Slot> concluded_true = solver.ArrangementSlotsConcludedAs(1);
  
  CHECK(concluded_false == std::vector<Slot> {});
  CHECK(concluded_true == std::vector<Slot> {"c"});

  // This is called just to check that the call changes nothing.
  solver.Process(2, {});
  
  concluded_false = solver.ArrangementSlotsConcludedAs(0);
  concluded_true = solver.ArrangementSlotsConcludedAs(1);
  
  CHECK(concluded_false == std::vector<Slot> {});
  CHECK(concluded_true == std::vector<Slot> {"c"});
  
}

TEST_CASE("Bucket problem example 2", "[bucket_problem]") {

  // This test shows that if you start minesweeper game with eight 1s around,
  // then it is actually pretty fine start.
  //
  // All eight slots that can not be accesed by pawn horse standing in the center
  // (at least in single move) for sure are bombfree (or ballfree, whatever),
  // and Solver is meant to conclude it.

  using Slot = U64;
  using Identifier = std::string;

  using Bucket = Bucket<Slot, Identifier>;
  using BucketProblemSolver = BucketProblemSolver<Slot, Identifier>;

  std::vector<Bucket> buckets = {
    Bucket("bucket_1", 1, {0, 1, 2}),
    Bucket("bucket_2", 1, {1, 2, 3, 4, 5}),
    Bucket("bucket_3", 1, {4, 5, 6}),
    Bucket("bucket_4", 1, {5, 6, 7, 8, 9}),
    Bucket("bucket_5", 1, {8, 9, 10}),
    Bucket("bucket_6", 1, {9, 10, 11, 12, 13}),
    Bucket("bucket_7", 1, {12, 13, 14}),
    Bucket("bucket_8", 1, {13, 14, 15, 0, 1})
  };
  
  BucketProblemSolver solver;

  for (U64 i = 0; i != 8; i++) {
    solver.Process(buckets[i]);
  }
  
  std::vector<Slot> concluded_false = solver.ArrangementSlotsConcludedAs(0);
  std::vector<Slot> concluded_true = solver.ArrangementSlotsConcludedAs(1);
  
  std::sort(concluded_false.begin(), concluded_false.end());
  
  CHECK(concluded_false == std::vector<Slot> {1, 3, 5, 7, 9, 11, 13, 15});
  CHECK(concluded_true == std::vector<Slot> {});

  // This is called just to check that the call changes nothing.
  solver.Process(4, {});
  
  concluded_false = solver.ArrangementSlotsConcludedAs(0);
  concluded_true = solver.ArrangementSlotsConcludedAs(1);
  
  std::sort(concluded_false.begin(), concluded_false.end());
  
  CHECK(concluded_false == std::vector<Slot> {1, 3, 5, 7, 9, 11, 13, 15});
  CHECK(concluded_true == std::vector<Slot> {});
  
}

TEST_CASE("Bucket problem example 3", "[bucket_problem]") {

  // This test case checks if second overload of Process() function works properly
  // in a case that separate_slots argument is nonempty (unlike in previous testcases).
  //
  // Consider following situation in the minesweeper game (letters correspond to unrevealed tiles):
  //
  // 0 2 a d
  // 0 3 b e
  // 0 2 c f
  //
  // Addidtional info: there are 3 bombs in total.
  //
  // Solver is meant to conclude that tiles d, e and f are bombfree.
  
  using Slot = std::string;
  using Identifier = std::string;

  using Bucket = Bucket<Slot, Identifier>;
  using BucketProblemSolver = BucketProblemSolver<Slot, Identifier>;

  Bucket upper_bucket("upper_bucket", 2, {"a", "b"});
  Bucket middle_bucket("middle_bucket", 3, {"a", "b", "c"});
  Bucket lower_bucket("lower_bucket", 2, {"b", "c"});

  BucketProblemSolver solver;

  solver.Process(upper_bucket);
  solver.Process(middle_bucket);
  solver.Process(lower_bucket);
  
  std::vector<Slot> concluded_false = solver.ArrangementSlotsConcludedAs(0);
  std::vector<Slot> concluded_true = solver.ArrangementSlotsConcludedAs(1);
  
  std::sort(concluded_false.begin(), concluded_false.end());
  std::sort(concluded_true.begin(), concluded_true.end());
  
  CHECK(concluded_false == std::vector<Slot> {});
  CHECK(concluded_true == std::vector<Slot> {"a", "b", "c"});

  // Here information that there are 3 bombs in total and another 3 slots exist is processed.
  solver.Process(3, {"d", "e", "f"});
  
  concluded_false = solver.ArrangementSlotsConcludedAs(0);
  concluded_true = solver.ArrangementSlotsConcludedAs(1);
  
  std::sort(concluded_false.begin(), concluded_false.end());
  std::sort(concluded_true.begin(), concluded_true.end());
  
  CHECK(concluded_false == std::vector<Slot> {"d", "e", "f"}); // New finding!
  CHECK(concluded_true == std::vector<Slot> {"a", "b", "c"});
  
}

TEST_CASE("Bucket problem example 4", "[bucket_problem]") {

  // This is another test case that checks if second overload of Process() function works properly
  // in a case that separate_slots argument is nonempty.
  //
  // Consider following situation in the minesweeper game (x correspond to tile marked as bomb and
  // other letters correspond to unrevealed tiles):
  //
  // x 2 1
  // 2 b d
  // 1 p q
  //
  // Addidtional info: there are 3 bombs in total (except one already marked).
  //
  // Before additional info is processed solver can conclude nothing;
  // after additional info is processed solver is meant to conclude that there are bombs on slots
  // d, p and q, while slot b is bombfree.
  
  using Slot = std::string;
  using Identifier = std::string;

  using Bucket = Bucket<Slot, Identifier>;
  using BucketProblemSolver = BucketProblemSolver<Slot, Identifier>;

  Bucket left_lower("left_lower", 1, {"b", "p"});
  Bucket left_upper("left_upper", 1, {"b", "p"});
  Bucket upper_left("upper_left", 1, {"b", "d"});
  Bucket upper_right("upper_right", 1, {"b", "d"});
  
  BucketProblemSolver solver;

  solver.Process(left_lower);
  solver.Process(left_upper);
  solver.Process(upper_left);
  solver.Process(upper_right);
  
  std::vector<Slot> concluded_false = solver.ArrangementSlotsConcludedAs(0);
  std::vector<Slot> concluded_true = solver.ArrangementSlotsConcludedAs(1);
  
  CHECK(concluded_false == std::vector<Slot> {});
  CHECK(concluded_true == std::vector<Slot> {});

  // Here information that there are 3 bombs in total and q slot exist is processed.
  solver.Process(3, {"q"});
  
  concluded_false = solver.ArrangementSlotsConcludedAs(0);
  concluded_true = solver.ArrangementSlotsConcludedAs(1);
  
  std::sort(concluded_false.begin(), concluded_false.end());
  std::sort(concluded_true.begin(), concluded_true.end());
  
  CHECK(concluded_false == std::vector<Slot> {"b"}); // New finding!
  CHECK(concluded_true == std::vector<Slot> {"d", "p", "q"}); // New finding!
  
}

}
