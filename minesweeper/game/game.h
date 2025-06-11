
#pragma once

#include <array>
#include <cassert>
#include <vector>

#include "alias\ad.h"
#include "minesweeper\table\table.h"
#include "random\rng\rng.h"

namespace ad {

enum GameStatus {
  during,
  win,
  loss,
  pass
};

class MinesweeperGame {
  
  static constexpr U8 unrevealed_tile_ = -2;
  
  const MinesweeperTable *const table_;
  
  std::vector<std::vector<bool>> visibilty_table_;
  
  U64 visibility_count_;
  
  const U64 max_energy_ = table_->Height() * table_->Width() - table_->BombNum();

  GameStatus status_;
  
 public:
  
  static constexpr const U8 UnrevealedTile() {return unrevealed_tile_;}
  
  const U64 &Height() const {return table_->Height();}
  const U64 &Width() const {return table_->Width();}
  const U64 &BombNum() const {return table_->BombNum();}
  
  const U64 &VisibilityCount() const {return visibility_count_;}
  
  const GameStatus &Status() const {return status_;}
  
  const U8 &Tile(const U64 &i, const U64 &j) const {
    if (visibilty_table_[i][j]) {
      return table_->Tile(i, j);
    } else {
      return unrevealed_tile_;
    }
  }
  
  const U8 &Tile(const std::array<U64, 2> &tile_coords) const {
    return Tile(tile_coords[0], tile_coords[1]);
  }
  
  U64 Energy() const {
    return max_energy_ - visibility_count_;
  }
  
  // Maybe make winning automatic?
  MinesweeperGame &CheckForWin() {
    if (!Energy()) {status_ = win;}
    return *this;
  }
  
  MinesweeperGame &Pass() {
    status_ = pass;
    return *this;
  }
  
  MinesweeperGame &Reveal(const U64 &i, const U64 &j) {
    if (status_ == during) {
      if (table_->Tile(i, j) == U8(-1)) {
        status_ = loss;
      } else {
        visibility_count_ += !visibilty_table_[i][j];
        visibilty_table_[i][j] = true;
      }
    }
    return *this;
  }
  
  MinesweeperGame &Reveal(const std::array<U64, 2> &tile_coords) {
    return Reveal(tile_coords[0], tile_coords[1]);
  }
  
  MinesweeperGame(
    const MinesweeperTable &table,
    const bool &reveal_random_zero = true // Asserts there exists one.
  ):
  table_(&table),
  visibilty_table_(table.Height(), std::vector<bool>(table.Width(), false)),
  visibility_count_(0),
  status_(during) {
    if (reveal_random_zero) {
      // Seed for rng, going to be modified in a hacky way to reduce bias.
      U64 seed = RNG<>::DefaultSeed();
      std::vector<std::array<U64, 2>> zeros_coords;
      for (U64 i = 0; i < table.Height(); i++) {
        for (U64 j = 0; j < table.Width(); j++) {
          const U8 &tile = table.Tile(i, j);
          if (tile == U8(0)) {
            zeros_coords.push_back({i, j});
          } else if (tile == U8(-1)) {
            // Even numbers would be dangerous (would make seed == 0).
            seed *= (2 * i + 1) * (2 * j + 1);
          }
        }
      }
      assert(!zeros_coords.empty());
      RNG rng(seed);
      std::array<U64, 2> zero_coords = zeros_coords[rng.ExactRandomU64(zeros_coords.size())];
      Reveal(zero_coords[0], zero_coords[1]);
    }
  }
  
  MinesweeperGame(
    const MinesweeperTable &table,
    const std::array<U64, 2> &tile_coords_to_be_revealed
  ):
  table_(&table),
  visibilty_table_(table.Height(), std::vector<bool>(table.Width(), false)),
  visibility_count_(0),
  status_(during) {
    Reveal(tile_coords_to_be_revealed);
  }
  
};

}
