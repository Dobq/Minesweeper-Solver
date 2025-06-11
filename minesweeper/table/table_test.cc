
#include <testing\catch2\catch.hpp>

#include <set>
#include <vector>

#include "alias\ad.h"
#include "minesweeper\table\table.h"

namespace ad {

TEST_CASE("MinesweeperTable::FromBombField", "[minesweeper]") {
  
  static const U8 x = -1;
  
  const std::vector<std::vector<bool>> bomb_field_1 = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  };

  const std::vector<std::vector<bool>> bomb_field_2 = {
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  };

  const std::vector<std::vector<bool>> bomb_field_3 = {
    {1, 1, 1, 1, 1, 1},
    {0, 0, 0, 1, 0, 1},
    {0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1},
  };

  MinesweeperTable minesweeper_table_1 = MinesweeperTable::FromBombField(bomb_field_1);
  MinesweeperTable minesweeper_table_2 = MinesweeperTable::FromBombField(bomb_field_2);
  MinesweeperTable minesweeper_table_3 = MinesweeperTable::FromBombField(bomb_field_3);

  const std::vector<std::vector<U8>> info_field_1 = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  };

  const std::vector<std::vector<U8>> info_field_2 = {
    {0, 1, 1, 1},
    {0, 1, x, 1},
    {0, 1, 1, 1},
    {0, 0, 0, 0},
  };

  const std::vector<std::vector<U8>> info_field_3 = {
    {x, x, x, x, x, x},
    {3, 5, 7, x, 8, x},
    {1, x, x, x, x, x},
    {1, 2, 4, 5, 6, 4},
    {0, 0, 1, x, x, x},
  };

  CHECK(minesweeper_table_1.InfoField() == info_field_1);
  CHECK(minesweeper_table_1.Height() == 4);
  CHECK(minesweeper_table_1.Width() == 4);
  CHECK(minesweeper_table_1.BombNum() == 0);
  
  CHECK(minesweeper_table_2.InfoField() == info_field_2);
  CHECK(minesweeper_table_2.Height() == 4);
  CHECK(minesweeper_table_2.Width() == 4);
  CHECK(minesweeper_table_2.BombNum() == 1);
  
  CHECK(minesweeper_table_3.InfoField() == info_field_3);
  CHECK(minesweeper_table_3.Height() == 5);
  CHECK(minesweeper_table_3.Width() == 6);
  CHECK(minesweeper_table_3.BombNum() == 16);

}

TEST_CASE("MinesweeperTable::FromSeed", "[minesweeper, random]") {

  MinesweeperTable minesweeper_table_1 = MinesweeperTable::FromSeed(16, 16, 32, 5);
  MinesweeperTable minesweeper_table_2 = MinesweeperTable::FromSeed(4, 16, 8, 6);
  MinesweeperTable minesweeper_table_3 = MinesweeperTable::FromSeed(13, 7, 16, 19);
  MinesweeperTable minesweeper_table_4 = MinesweeperTable::FromSeed(3, 3, 9, 3);

  CHECK(!minesweeper_table_1.InfoField().empty());
  CHECK(minesweeper_table_1.Height() == 16);
  CHECK(minesweeper_table_1.Width() == 16);
  CHECK(minesweeper_table_1.BombNum() == 32);
  
  CHECK(!minesweeper_table_2.InfoField().empty());
  CHECK(minesweeper_table_2.Height() == 4);
  CHECK(minesweeper_table_2.Width() == 16);
  CHECK(minesweeper_table_2.BombNum() == 8);
  
  CHECK(!minesweeper_table_3.InfoField().empty());
  CHECK(minesweeper_table_3.Height() == 13);
  CHECK(minesweeper_table_3.Width() == 7);
  CHECK(minesweeper_table_3.BombNum() == 16);
  
  CHECK(!minesweeper_table_4.InfoField().empty());
  CHECK(minesweeper_table_4.Height() == 3);
  CHECK(minesweeper_table_4.Width() == 3);
  CHECK(minesweeper_table_4.BombNum() == 9);

}

TEST_CASE("MinesweeperTable::FromSeed is seed-sensitive", "[minesweeper, random]") {

  U64 height = 6;
  U64 width = 8;
  U64 bomb_num = 8;
  U64 instances_num = 256;

  std::vector<MinesweeperTable> minesweeper_tables;

  for (U64 i = 1; i <= instances_num; i++) {
    minesweeper_tables.push_back(MinesweeperTable::FromSeed(height, width, bomb_num, i));
  }

  std::vector<std::vector<std::set<U8>>> tile_variations(height, std::vector<std::set<U8>>(width));
  
  U64 minimal_variativity = -1;
  U64 total_variativity = 0;
  U64 maximal_variativity = 0;
  for (U64 i = 0; i < height; i++) {
    for (U64 j = 0; j < width; j++) {
      std::set<U8> variations;
      for (U64 k = 0; k < instances_num; k++) {
        variations.insert(minesweeper_tables[k].Tile(i, j));
      }
      U64 variativity = variations.size();
      minimal_variativity = std::min(variativity, minimal_variativity);
      total_variativity += variativity;
      maximal_variativity = std::max(variativity, maximal_variativity);
    }
  }
  
  CHECK(minimal_variativity >= 4);
  CHECK(total_variativity >= height * width * 11 / 2); // Checks if mean variativity >= 5.5.
  CHECK(maximal_variativity >= 7);
  CHECK(maximal_variativity <= 10); // Higher should be impossible.
  
}

}
