
#include "minesweeper\solver\test.h"

#include <iostream>

#include "alias\ad.h"
#include "minesweeper\solver\basic.h"

namespace ad {

TEST_CASE("BasicMinesweeperSolver density = 1 / 24", "[minesweeper]") {

  static const U64 height = 10;
  static const U64 width = 12;
  static const U64 density_reciprocal = 24;
  static const U64 instances_num = 400;
  static const U64 required_perfectly_solved_instances_num = 380;
  static const U64 total_tolernace_reciprocal = 60;
  
  SolverRandomInstancesSolvedWellTest<
    BasicMinesweeperSolver,
    height,
    width,
    density_reciprocal,
    instances_num,
    required_perfectly_solved_instances_num,
    total_tolernace_reciprocal
  >();
  
}

TEST_CASE("BasicMinesweeperSolver density = 1 / 12", "[minesweeper]") {

  static const U64 height = 10;
  static const U64 width = 12;
  static const U64 density_reciprocal = 12;
  static const U64 instances_num = 400;
  static const U64 required_perfectly_solved_instances_num = 340; 
  static const U64 total_tolernace_reciprocal = 10;
  
  SolverRandomInstancesSolvedWellTest<
    BasicMinesweeperSolver,
    height,
    width,
    density_reciprocal,
    instances_num,
    required_perfectly_solved_instances_num,
    total_tolernace_reciprocal
  >();
  
}

TEST_CASE("BasicMinesweeperSolver density = 1 / 8", "[minesweeper]") {

  static const U64 height = 10;
  static const U64 width = 12;
  static const U64 density_reciprocal = 8;
  static const U64 instances_num = 400;
  static const U64 required_perfectly_solved_instances_num = 200;
  static const U64 total_tolernace_reciprocal = 3;
  
  SolverRandomInstancesSolvedWellTest<
    BasicMinesweeperSolver,
    height,
    width,
    density_reciprocal,
    instances_num,
    required_perfectly_solved_instances_num,
    total_tolernace_reciprocal
  >();
  
}

// It is not tricky at all;
// it is an simple example of case that requires both methods to solve.
TEST_CASE("BasicMinesweeperSolver tricky case 0", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1},
  };

  SolverSingleInstanceSolvedPerfectlyTest<
    BasicMinesweeperSolver
  >(bomb_field);
  
}

}
