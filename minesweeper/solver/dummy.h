
#pragma once

#include <cassert>

#include "alias\ad.h"
#include "minesweeper\solver\solver.h"

namespace ad {

// This class is for tests only.
class DummyMinesweeperSolver: public MinesweeperSolver<DummyMinesweeperSolver> {
  
  using Base = MinesweeperSolver<DummyMinesweeperSolver>;
  
  friend Base;
  
  U64 i_;
  
  void BeginSolving() {}
  
  void Cycle() {
    if (i_ >= game_->Height()) {return;} 
    for (U64 j = 0; j < game_->Width(); j++) {
      // Little test for this method.
      assert(Neighbourhood(i_, j).size() > 2);
      game_->Reveal(i_, j);
    }
    i_++;
    return;
  }
  
 public:
  
  DummyMinesweeperSolver(MinesweeperGame &game):
  Base(game),
  i_(0)
  {}
  
};

}
