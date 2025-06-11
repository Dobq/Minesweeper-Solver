
#include <testing\catch2\catch.hpp>

#include "alias\ad.h"
#include "minesweeper\game\game.h"
#include "minesweeper\solver\dummy.h"
#include "minesweeper\table\table.h"

namespace ad {

TEST_CASE("MinesweeperSolver win", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field_1 = {
    {0, 0, 0},
    {0, 0, 1},
  };

  MinesweeperTable minesweeper_table_1 = MinesweeperTable::FromBombField(bomb_field_1);

  MinesweeperGame minesweeper_game_1(minesweeper_table_1);

  CHECK(minesweeper_game_1.Status() == during);
  CHECK(minesweeper_game_1.Energy() == 4ull);

  DummyMinesweeperSolver dummy_minesweeper_solver_1(minesweeper_game_1);
  
  dummy_minesweeper_solver_1.Solve();
  
  CHECK(minesweeper_game_1.Status() == win);
  CHECK(minesweeper_game_1.Energy() == 0ull);
  
}

}
