
#pragma once

#include <array>
#include <set>
#include <vector>

#include "alias\ad.h"
#include "minesweeper\game\game.h"
#include "minesweeper\print\field.h"

namespace ad {

template<class Derived>
class MinesweeperSolver {
  
  U64 cycles_;
  
  bool print_cycles_;
  
  bool NextCycle() {
    static_cast<Derived*>(this)->Cycle();
    cycles_++;
    game_->CheckForWin();
    return game_->Status() == during;
  }
  
 protected:
  
  MinesweeperGame *const game_;
  
  template<bool inclusive = false>
  std::vector<std::array<U64, 2>> Neighbourhood(
    const U64 &i,
    const U64 &j
  ) const {
    std::vector<std::array<U64, 2>> neighbourhood;
    neighbourhood.reserve(8 + inclusive);
    for (U64 ii = std::max<I64>(0, i - 1); ii < U64(std::min<I64>(i + 2, game_->Height())); ii++) {
      for (U64 jj = std::max<I64>(0, j - 1); jj < U64(std::min<I64>(j + 2, game_->Width())); jj++) {
        if (!inclusive || !(i == ii && j == jj)) {
          neighbourhood.push_back({ii, jj});
        }
      }
    }
    return neighbourhood;
  }
  
  template<bool inclusive = false>
  std::vector<std::array<U64, 2>> Neighbourhood(
    const std::array<U64, 2> &tile_coords
  ) const {
    return Neighbourhood<inclusive>(tile_coords[0], tile_coords[1]);
  }
  
 public:
  
  void PrintGameField(const std::set<std::array<U64, 2>> &special_coords = {}) const {
    if (print_cycles_) {
      PrintField<MinesweeperGame>(*game_, special_coords);
    }
  }
  
  const MinesweeperGame &Game() {return *game_;}
  
  void Solve() {
    static_cast<Derived*>(this)->BeginSolving();
    while (NextCycle());
  }
  
  MinesweeperSolver(MinesweeperGame &game, const bool &print_cycles = false):
  cycles_(0),
  print_cycles_(print_cycles),
  game_(&game)
  {}
  
};

}
