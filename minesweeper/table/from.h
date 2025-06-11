
#pragma once

#include <sstream>

#include "alias\ad.h"
#include "minesweeper\table\table.h"

namespace ad {

MinesweeperTable MinesweeperTableFrom(std::stringstream &input) {
  U64 height;
  input >> height;
  U64 width;
  input >> width;
  // TODO: Use more std::vector::reserve() and std::vector::shrink_to_fit() if it makes any sense.
  std::vector<std::vector<bool>> bomb_field;
  for (U64 i = 0; i < height; i++) {
    bomb_field.push_back({});
    bomb_field[i].reserve(width);
    for (U64 j = 0; j < width; j++) {
      bool b;
      input >> b;
      bomb_field[i].push_back(b);
    }
  }
  return MinesweeperTable::FromBombField(bomb_field);
}

}
