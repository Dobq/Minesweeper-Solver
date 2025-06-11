
#pragma once

#include <testing\catch2\catch.hpp>

#include <array>
#include <iostream>
#include <vector>

#include "alias\ad.h"
#include "minesweeper\game\game.h"
#include "minesweeper\table\table.h"

namespace ad {

template<
  class Solver,
  U64 height,
  U64 width,
  U64 density_reciprocal,
  U64 instances_num,
  U64 required_perfectly_solved_instances_num, 
  U64 total_tolernace_reciprocal,
  bool print_info = false
>
void SolverRandomInstancesSolvedWellTest() {
  U64 total_energy = 0;
  U64 total_perfectly_solved_instances_num = 0;
  for (U64 i = 0; i < instances_num; i++) {
    MinesweeperTable minesweeper_table = MinesweeperTable::FromSeed(
      height,
      width,
      height * width / density_reciprocal,
      -1ull / (i + 1)
    );
    MinesweeperGame minesweeper_game = MinesweeperGame(minesweeper_table);
    Solver minesweeper_solver = Solver(minesweeper_game);
    minesweeper_solver.Solve();
    CHECK(minesweeper_game.Status() != loss);
    CHECK(minesweeper_game.Status() != during);
    total_perfectly_solved_instances_num += minesweeper_game.Energy() == 0;
    total_energy += minesweeper_game.Energy();
  }
  if (print_info) {
    std::cout << total_perfectly_solved_instances_num << std::endl;
    std::cout << total_energy << std::endl;
    std::cout << std::endl;
  }
  CHECK(total_perfectly_solved_instances_num >= required_perfectly_solved_instances_num);
  CHECK(total_energy <= instances_num * height * width / total_tolernace_reciprocal);
}

template<
  class Solver,
  std::array<U64, 2> starting_tile_coords = std::array<U64, 2> {0, 0},
  bool print_cycles = false
>
void SolverSingleInstanceSolvedPerfectlyTest(
  std::vector<std::vector<bool>> bomb_field // Can not be template parameter.
) {
  MinesweeperTable minesweeper_table = MinesweeperTable::FromBombField(bomb_field);
  MinesweeperGame minesweeper_game(minesweeper_table, starting_tile_coords);
  Solver minesweeper_solver = Solver(minesweeper_game, print_cycles);
  minesweeper_solver.Solve();
  CHECK(minesweeper_game.Status() == win);
}

}
