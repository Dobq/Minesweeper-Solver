
#include <testing\catch2\catch.hpp>

#include "alias\ad.h"
#include "minesweeper\game\game.h"
#include "minesweeper\table\table.h"

namespace ad {

TEST_CASE("MinesweeperGame win", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field_1 = {
    {0, 0, 0},
    {0, 0, 1},
  };

  MinesweeperTable minesweeper_table_1 = MinesweeperTable::FromBombField(bomb_field_1);

  MinesweeperGame minesweeper_game_1(minesweeper_table_1);

  static constexpr const U8 &unrevealed_tile = MinesweeperGame::UnrevealedTile();

  CHECK(unrevealed_tile > U8(8));
  CHECK(minesweeper_game_1.Height() == 2ull);
  CHECK(minesweeper_game_1.Width() == 3ull);
  CHECK(minesweeper_game_1.BombNum() == 1ull);
  CHECK(minesweeper_game_1.VisibilityCount() == 1ull);
  CHECK(minesweeper_game_1.Status() == during);
  CHECK(minesweeper_game_1.Energy() == 4ull);
  CHECK((minesweeper_game_1.Tile(0, 0) == U8(0)) ^ (minesweeper_game_1.Tile(1, 0) == U8(0)));
  CHECK(minesweeper_game_1.Tile(0, 1) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(1, 1) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(0, 2) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(1, 2) == unrevealed_tile);
  
  minesweeper_game_1
    .CheckForWin() // Should just do nothing
    .Reveal(0, 0)
    .Reveal(1, 0)
    .Reveal(0, 1)
    .Reveal(1, 1)
    .Reveal(0, 2)
    .CheckForWin()
  ;
    
  CHECK(minesweeper_game_1.Tile(0, 0) == U8(0));
  CHECK(minesweeper_game_1.Tile(1, 0) == U8(0));
  CHECK(minesweeper_game_1.Tile(0, 1) == U8(1));
  CHECK(minesweeper_game_1.Tile(1, 1) == U8(1));
  CHECK(minesweeper_game_1.Tile(0, 2) == U8(1));
  CHECK(minesweeper_game_1.Tile(1, 2) == unrevealed_tile);
  CHECK(minesweeper_game_1.VisibilityCount() == 5ull);
  CHECK(minesweeper_game_1.Status() == win);
  CHECK(minesweeper_game_1.Energy() == 0ull);
  
}

TEST_CASE("MinesweeperGame loss", "[minesweeper]") {

  const std::vector<std::vector<bool>> bomb_field_1 = {
    {0, 0, 1},
    {0, 0, 0},
    {0, 1, 1},
  };

  MinesweeperTable minesweeper_table_1 = MinesweeperTable::FromBombField(bomb_field_1);

  MinesweeperGame minesweeper_game_1(minesweeper_table_1);

  static constexpr const U8 &unrevealed_tile = MinesweeperGame::UnrevealedTile();

  CHECK(unrevealed_tile > U8(8));
  CHECK(minesweeper_game_1.Height() == 3ull);
  CHECK(minesweeper_game_1.Width() == 3ull);
  CHECK(minesweeper_game_1.BombNum() == 3ull);
  CHECK(minesweeper_game_1.VisibilityCount() == 1ull);
  CHECK(minesweeper_game_1.Status() == during);
  CHECK(minesweeper_game_1.Energy() == 5ull);
  CHECK(minesweeper_game_1.Tile(0, 0) == U8(0));
  CHECK(minesweeper_game_1.Tile(1, 0) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(2, 0) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(0, 1) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(1, 1) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(2, 1) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(0, 2) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(1, 2) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(2, 2) == unrevealed_tile);
  
  minesweeper_game_1
    .Reveal(0, 1)
    .Reveal(0, 2) // Boom!
    .Reveal(1, 0) // Should do nothing as game should be lost.
    .CheckForWin() // Nope.
  ;
  
  CHECK(minesweeper_game_1.Tile(0, 0) == U8(0));
  CHECK(minesweeper_game_1.Tile(0, 1) == U8(1));
  CHECK(minesweeper_game_1.Tile(0, 2) == unrevealed_tile);
  CHECK(minesweeper_game_1.Tile(1, 0) == unrevealed_tile);
  CHECK(minesweeper_game_1.VisibilityCount() == 2ull);
  CHECK(minesweeper_game_1.Status() == loss);
  CHECK(minesweeper_game_1.Energy() == 4ull);
  
}

}
