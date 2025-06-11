
#pragma once

#include <array>
#include <iostream>
#include <set>
#include <sstream>

#include "alias/ad.h"

namespace ad {

template<class Field>
void PrintField(
  const Field &field,
  std::stringstream &ss,
  const std::set<std::array<U64, 2>> &special_coords = {}
) {
  for (U64 i = 0; i < field.Height(); i++) {
    ss << "\n";
    for (U64 j = 0; j < field.Width(); j++) {
      const U8 tile = field.Tile(i, j);
      if (special_coords.contains({i, j})) {
        ss << "\033[1;35mX\033[0m"; // Orange "X".
      } else if (tile == U8(-1)) {
        ss << "\033[1;31mX\033[0m"; // Red "X".
      } else if (tile == U8(0)) {
        ss << "\033[1;32m" << U16(tile) << "\033[0m"; // Green "0".
      } else if (tile == U8(1)) {
        ss << "\033[1;34m" << U16(tile) << "\033[0m"; // Blue "1".
      } else if (tile <= U8(3)) {
        ss << "\033[1;36m" << U16(tile) << "\033[0m"; // Cyan "2" or "3".
      } else if (tile <= U8(8)) {
        ss << "\033[1;33m" << U16(tile) << "\033[0m"; // Yellow "4", "5", "6", "7" or "8".
      } else {
        ss << "\033[1;30m" << tile << "\033[0m"; // Black block.
      }
      ss << " ";
    }
  }
  ss << std::endl;
}

template<class Field>
void PrintField(
  const Field &field,
  const std::set<std::array<U64, 2>> &special_coords = {}
) {
  std::stringstream ss;
  PrintField<Field>(field, ss, special_coords);
  std::cout << ss.rdbuf();
}

}
