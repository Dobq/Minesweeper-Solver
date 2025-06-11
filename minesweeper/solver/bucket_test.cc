
#include "minesweeper\solver\test.h"

#include "alias\ad.h"
#include "minesweeper\solver\bucket.h"

namespace ad {

TEST_CASE("BucketMinesweeperSolver density = 1 / 24", "[minesweeper]") {

  static const U64 height = 10;
  static const U64 width = 12;
  static const U64 density_reciprocal = 24;
  static const U64 instances_num = 400;
  static const U64 required_perfectly_solved_instances_num = 395; // 399 on average.
  static const U64 total_tolernace_reciprocal = 240;
  
  SolverRandomInstancesSolvedWellTest<
    BucketMinesweeperSolver,
    height,
    width,
    density_reciprocal,
    instances_num,
    required_perfectly_solved_instances_num,
    total_tolernace_reciprocal
  >();
  
}

TEST_CASE("BucketMinesweeperSolver density = 1 / 12", "[minesweeper]") {

  static const U64 height = 10;
  static const U64 width = 12;
  static const U64 density_reciprocal = 12;
  static const U64 instances_num = 400;
  static const U64 required_perfectly_solved_instances_num = 380; // 390 on average.
  static const U64 total_tolernace_reciprocal = 30;
  
  SolverRandomInstancesSolvedWellTest<
    BucketMinesweeperSolver,
    height,
    width,
    density_reciprocal,
    instances_num,
    required_perfectly_solved_instances_num,
    total_tolernace_reciprocal
  >();
  
}

TEST_CASE("BucketMinesweeperSolver density = 1 / 8", "[minesweeper]") {

  static const U64 height = 10;
  static const U64 width = 12;
  static const U64 density_reciprocal = 8;
  static const U64 instances_num = 400;
  static const U64 required_perfectly_solved_instances_num = 335; // 354 on average.
  static const U64 total_tolernace_reciprocal = 12;
  
  SolverRandomInstancesSolvedWellTest<
    BucketMinesweeperSolver,
    height,
    width,
    density_reciprocal,
    instances_num,
    required_perfectly_solved_instances_num,
    total_tolernace_reciprocal
  >();
  
}

TEST_CASE("BucketMinesweeperSolver density = 1 / 6", "[minesweeper]") {

  static const U64 height = 10;
  static const U64 width = 12;
  static const U64 density_reciprocal = 6;
  static const U64 instances_num = 400;
  static const U64 required_perfectly_solved_instances_num = 236; // 256 on average.
  static const U64 total_tolernace_reciprocal = 5;
  
  SolverRandomInstancesSolvedWellTest<
    BucketMinesweeperSolver,
    height,
    width,
    density_reciprocal,
    instances_num,
    required_perfectly_solved_instances_num,
    total_tolernace_reciprocal
  >();
  
}

TEST_CASE("BucketMinesweeperSolver density = 1 / 5", "[minesweeper]") {

  static const U64 height = 10;
  static const U64 width = 12;
  static const U64 density_reciprocal = 5;
  static const U64 instances_num = 400;
  static const U64 required_perfectly_solved_instances_num = 92; // 139 on average.
  static const U64 total_tolernace_reciprocal = 3;
  
  SolverRandomInstancesSolvedWellTest<
    BucketMinesweeperSolver,
    height,
    width,
    density_reciprocal,
    instances_num,
    required_perfectly_solved_instances_num,
    total_tolernace_reciprocal
  >();
  
}

TEST_CASE("BucketMinesweeperSolver tricky case 0", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field = {
    {0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 1, 1},
  };

  SolverSingleInstanceSolvedPerfectlyTest<
    BucketMinesweeperSolver
  >(bomb_field);
  
}

TEST_CASE("BucketMinesweeperSolver tricky case 1", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field = {
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
  };

  SolverSingleInstanceSolvedPerfectlyTest<
    BucketMinesweeperSolver
  >(bomb_field);
  
}

TEST_CASE("BucketMinesweeperSolver tricky case 2", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0},
  };

  SolverSingleInstanceSolvedPerfectlyTest<
    BucketMinesweeperSolver
  >(bomb_field);
  
}

TEST_CASE("BucketMinesweeperSolver tricky case 3", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 0, 1, 0, 1, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0},
  };

  SolverSingleInstanceSolvedPerfectlyTest<
    BucketMinesweeperSolver
  >(bomb_field);
  
}

TEST_CASE("BucketMinesweeperSolver tricky case 4", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field = {
    {0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {1, 0, 0, 0, 0, 0, 0},
  };

  SolverSingleInstanceSolvedPerfectlyTest<
    BucketMinesweeperSolver
  >(bomb_field);
  
}

TEST_CASE("BucketMinesweeperSolver tricky case 5", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0},
  };

  SolverSingleInstanceSolvedPerfectlyTest<
    BucketMinesweeperSolver
  >(bomb_field);
  
}

TEST_CASE("BucketMinesweeperSolver tricky case 6", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0},
  };

  SolverSingleInstanceSolvedPerfectlyTest<
    BucketMinesweeperSolver
  >(bomb_field);
  
}

}
