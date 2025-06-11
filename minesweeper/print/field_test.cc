
#include <testing\catch2\catch.hpp>

#include <iostream>

#include "alias\ad.h"
#include "minesweeper\game\game.h"
#include "minesweeper\print\field.h"
#include "minesweeper\table\table.h"

namespace ad {

TEST_CASE("PrintField", "[minesweeper]") {

  std::stringstream ss;

  const std::vector<std::vector<bool>> bomb_field = {
    {1, 1, 1, 1, 1, 1},
    {0, 0, 0, 1, 0, 1},
    {0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1},
  };

  MinesweeperTable minesweeper_table = MinesweeperTable::FromBombField(bomb_field);

  PrintField<MinesweeperTable>(minesweeper_table, ss);

  MinesweeperGame minesweeper_game = MinesweeperGame(minesweeper_table);
    
  PrintField<MinesweeperGame>(minesweeper_game, ss);

  CHECK(ss.good());

}

}
