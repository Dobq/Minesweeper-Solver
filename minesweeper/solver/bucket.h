
#pragma once

#include <array>
#include <set>
#include <vector>

#include "alias\ad.h"
#include "bucket_problem\solver.h"
#include "minesweeper\solver\basic.h"

namespace ad {

class BucketMinesweeperSolver: public BasicMinesweeperSolver {
  
  using Base = BasicMinesweeperSolver;
  
  friend Base;
  
  using Bucket = Bucket<std::array<U64, 2>, std::array<U64, 2>>;
  using BucketProblemSolver = BucketProblemSolver<std::array<U64, 2>, std::array<U64, 2>>;
  
  const Bucket BucketFrom(const std::array<U64, 2> &tile_coords) const {
    U8 tile = game_->Tile(tile_coords);
    U64 balls_num = tile;
    std::vector<std::array<U64, 2>> slots;
    for (const std::array<U64, 2> &neighbour_coords : Neighbourhood(tile_coords)) {
      if (bomb_marked_tiles_coords_.contains(neighbour_coords)) {
        balls_num--;
      } else if (game_->Tile(neighbour_coords) == game_->UnrevealedTile()) {
        slots.push_back(neighbour_coords);
      }
    }
    return Bucket(tile_coords, balls_num, slots);
  }
  
  // Returns true iff tile on given coords is separated, that means iff it is unrevealed
  // and all its neighbours are unrevealed (it is ok if they are marked as bombs).
  bool IsSeparated(const std::array<U64, 2> &tile_coords) const {
    if (game_->Tile(tile_coords) != game_->UnrevealedTile()) {return false;}
    for (const std::array<U64, 2> &neighbour_coords : Neighbourhood(tile_coords)) {
      if (game_->Tile(neighbour_coords) != game_->UnrevealedTile()) {return false;}
    }
    return true;
  }
  
  void TryToSolveCurrentSituationViaBucketProblemSolver() {
    BucketProblemSolver solver;
    // Order of processed buckets may have big impact on performance for bigger instances.
    // Moreover for big instances it would make sense to split problem into few separate
    // connected instances if possible.
    for (const std::array<U64, 2> &tile_coords : focused_tiles_coords_) {
      solver.Process(BucketFrom(tile_coords));
    }
    std::vector<std::array<U64, 2>> concluded_false = solver.ArrangementSlotsConcludedAs(0);
    std::vector<std::array<U64, 2>> concluded_true = solver.ArrangementSlotsConcludedAs(1);
    // Isn't if below overkill a bit? Maybe it's not.
    if (concluded_false.empty() && concluded_true.empty()) {
      std::vector<std::array<U64, 2>> separate_tiles_coords;
      // There is faster way to create this vector.
      for (U64 i = 0; i < game_->Height(); i++) {
        for (U64 j = 0; j < game_->Width(); j++) {
          if (IsSeparated({i, j})) {
            separate_tiles_coords.push_back({i, j});
          }
        }
      }
      solver.Process(game_->BombNum() - bomb_marked_tiles_coords_.size(), separate_tiles_coords);
    }
    concluded_false = solver.ArrangementSlotsConcludedAs(0);
    concluded_true = solver.ArrangementSlotsConcludedAs(1);
    for (const std::array<U64, 2> &tile_coords : concluded_false) {
      game_->Reveal(tile_coords);
      Focus(tile_coords);
    }
    for (const std::array<U64, 2> &tile_coords : concluded_true) {
      MarkAsBomb(tile_coords);
    }
  }
  
 protected:
  
  // TODO: Consider avoiding use of virtual functions.
  // TryToSolveCurrentSituationViaBucketProblemSolver() method can do everything 
  // two basic methods do, but we keep basic methods here because it is more effective
  // than using a sledgehammer to crack a nut in each cycle.
  void virtual Cycle() override {
    U64 initial_unknown_tiles_count = UnknownTilesCount();
    // First basic computationally cheap method.
    for (U64 i = 0; i < focused_tiles_coords_.size(); i++) {
      RevealAndFocusUnrevealedAndNotMarkedAsBombsNeigboursIfIsBombSaturated(focused_tiles_coords_[i]);
    }
    // Second basic computationally cheap method.
    for (U64 i = 0; i < focused_tiles_coords_.size(); i++) {
      MarkUnrevealedNeigboursAsBombsIfIsUnrevealedSaturated(focused_tiles_coords_[i]);
    }
    CleanFocusedTiles();
    // In case basic methods do not help, use BucketProblemSolver to find if there is any good next step.
    if (initial_unknown_tiles_count == UnknownTilesCount()) {
      TryToSolveCurrentSituationViaBucketProblemSolver();
      CleanFocusedTiles();
    }
    // In case even BucketProblemSolver did not help, pass.
    if (initial_unknown_tiles_count == UnknownTilesCount()) {
      game_->Pass();
    } else {
      PrintGameField(bomb_marked_tiles_coords_);
    }
  }
  
 public:
  
  BucketMinesweeperSolver(MinesweeperGame &game, const bool &print_cycles = false):
  Base(game, print_cycles)
  {}
  
};

}
