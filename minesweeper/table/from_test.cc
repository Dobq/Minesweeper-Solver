
#include <testing\catch2\catch.hpp>

#include <sstream>
#include <vector>

#include "alias\ad.h"
#include "minesweeper\table\from.h"
#include "minesweeper\table\table.h"

namespace ad {

TEST_CASE("MinesweeperTableFrom", "[minesweeper]") {
  
  static const U8 x = -1;
  
  std::stringstream input_sstream_1( 
    "4 4    \n"
    "0 0 0 0\n"
    "0 0 0 0\n"
    "0 0 0 0\n"
    "0 0 0 0\n"
  );

  std::stringstream input_sstream_2(
    "4 4    \n"
    "0 0 0 0\n"
    "0 0 1 0\n"
    "0 0 0 0\n"
    "0 0 0 0\n"
  );

  std::stringstream input_sstream_3(
    "5 6        \n"
    "1 1 1 1 1 1\n"
    "0 0 0 1 0 1\n"
    "0 1 1 1 1 1\n"
    "0 0 0 0 0 0\n"
    "0 0 0 1 1 1\n"
  );

  MinesweeperTable minesweeper_table_1 = MinesweeperTableFrom(input_sstream_1);
  MinesweeperTable minesweeper_table_2 = MinesweeperTableFrom(input_sstream_2);
  MinesweeperTable minesweeper_table_3 = MinesweeperTableFrom(input_sstream_3);

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

}
