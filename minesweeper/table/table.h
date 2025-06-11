
#pragma once

#include <cassert>
#include <numeric>
#include <vector>

#include "alias\ad.h"
#include "random\rng\rng.h"

namespace ad {

class MinesweeperTable {
  
  // info_field_[i][j] == number of bombs in neighbourhood, 255 if there is bomb,
  // U8 is used as it is smallest type that is big enough.
  const std::vector<std::vector<U8>> info_field_;
  const U64 height_;
  const U64 width_;
  const U64 bomb_num_;
  
  MinesweeperTable(
    const std::vector<std::vector<U8>> &info_field,
    const U64 &height,
    const U64 &width,
    const U64 &bomb_num
  ):
  info_field_(info_field),
  height_(height),
  width_(width),
  bomb_num_(bomb_num) {}
  
 public:
  
  const std::vector<std::vector<U8>> &InfoField() const {return info_field_;}
  const U8 &Tile(const U64 &i, const U64 &j) const {return info_field_[i][j];}
  const U64 &Height() const {return height_;}
  const U64 &Width() const {return width_;}
  const U64 &BombNum() const {return bomb_num_;}
  
  static MinesweeperTable FromBombField(
    const std::vector<std::vector<bool>> &bomb_field
  ) {
    const U64 height = bomb_field.size();
    const U64 width = bomb_field[0].size();
    std::vector<std::vector<U8>> info_field(height, std::vector<U8>(width, 0));
    U64 bomb_num = 0;
    for (const std::vector<bool> &i : bomb_field) {
      assert(i.size() == width);
    }
    auto update_info_field_and_bomb_num = [&](const U64 &i, const U64 &j) {
      if (bomb_field[i][j]) {
        info_field[i][j] = -1;
        bomb_num++;
        for (U64 ii = std::max<I64>(0, i - 1); ii < U64(std::min<I64>(i + 2, height)); ii++) {
          for (U64 jj = std::max<I64>(0, j - 1); jj < U64(std::min<I64>(j + 2, width)); jj++) {
            if (info_field[ii][jj] != U8(-1)) {
              info_field[ii][jj]++;
            }
          }
        }
      }
    };
    for (U64 i = 0; i < height; i++) {
      for (U64 j = 0; j < width; j++) {
        update_info_field_and_bomb_num(i, j);
      }
    }
    return MinesweeperTable(info_field, height, width, bomb_num);
  }

  static MinesweeperTable FromSeed(
    const U64 &height, 
    const U64 &width, 
    const U64 &bomb_num, 
    const U64 &seed
  ) {
    assert(bomb_num <= height * width);
    RNG rng(seed);
    std::vector<std::vector<bool>> bomb_field(height, std::vector<bool>(width, false));
    U64 i = 0;
    while (i < bomb_num) {
      U64 x = rng.ExactRandomU64(height);
      U64 y = rng.ExactRandomU64(width);
      if (!bomb_field[x][y]) {
        bomb_field[x][y] = true;
        i++;
      }
    }
    return MinesweeperTable::FromBombField(bomb_field);
  }
  
};

}
