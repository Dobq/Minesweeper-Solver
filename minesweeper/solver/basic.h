
#pragma once

#include <array>
#include <set>
#include <vector>

#include "alias\ad.h"
#include "minesweeper\solver\solver.h"

namespace ad {

class BasicMinesweeperSolver: public MinesweeperSolver<BasicMinesweeperSolver> {
  
  using Base = MinesweeperSolver<BasicMinesweeperSolver>;
  
  friend Base;
  
 protected:
  
  // Tile is considered to be focused if it is revealed and
  // in surrounding 3 by 3 square there is
  // at least one tile that is unrevealed and not marked as bomb tile.
  // This vector is going to be updated each cycle.
  std::vector<std::array<U64, 2>> focused_tiles_coords_;
  
  std::set<std::array<U64, 2>> bomb_marked_tiles_coords_;
  
  void Focus(const std::array<U64, 2> &tile_coords) {
    focused_tiles_coords_.push_back(tile_coords);
  }
  
  // Quickly removes index-th thing from focused_tiles_coords_.
  void Unfocus(const U64 &index) {
    std::swap(focused_tiles_coords_[index], focused_tiles_coords_.back());
    focused_tiles_coords_.pop_back();
  }
  
  bool ShouldBeUnfocused(const std::array<U64, 2> &tile_coords) const {
    for (const std::array<U64, 2> &neighbour_coords : Neighbourhood<true>(tile_coords)) {
      if (game_->Tile(neighbour_coords) == game_->UnrevealedTile() && !bomb_marked_tiles_coords_.contains(neighbour_coords)) {
        return false;
      }
    }
    return true;
  }
  
  // Should be used only at the begging.
  void FocusAllRevealedTiles() {
    for (U64 i = 0; i < game_->Height(); i++) {
      for (U64 j = 0; j < game_->Width(); j++) {
        if (game_->Tile(i, j) <= U8(8)) {
          Focus({i, j});
        }
      }
    }
  }
  
  // Unfocuses all tiles that do not need to be focused longer.
  void CleanFocusedTiles() {
    for (U64 i = focused_tiles_coords_.size() - 1; i != -1ull; i--) {
      if (ShouldBeUnfocused(focused_tiles_coords_[i])) {
        Unfocus(i);
      }
    }
  }
  
  void MarkAsBomb(const std::array<U64, 2> &tile_coords) {
    bomb_marked_tiles_coords_.insert(tile_coords);
  }
  
  // Tile is meant to be unrevealed saturated if it has number of unrevealed
  // neighbours equal to its number.
  void MarkUnrevealedNeigboursAsBombsIfIsUnrevealedSaturated(
    const std::array<U64, 2> &tile_coords
  ) {
    if (game_->Tile(tile_coords) > U8(8)) {return;}
    std::vector<std::array<U64, 2>> neighbourhood = Neighbourhood<>(tile_coords);
    std::vector<std::array<U64, 2>> unrevealed_neighbours_coords;
    for (const std::array<U64, 2> &neighbour_coords : neighbourhood) {
      if (game_->Tile(neighbour_coords) == U8(-2)) {
        unrevealed_neighbours_coords.push_back(neighbour_coords);
      }
    }
    if (U8(unrevealed_neighbours_coords.size()) == game_->Tile(tile_coords)) {
      for (const std::array<U64, 2> &unrevealed_neighbour_coords : unrevealed_neighbours_coords) {
        MarkAsBomb(unrevealed_neighbour_coords);
      }
    }
  }
  
  // Tile is meant to be bomb saturated if it has number of neighbouring 
  // bomb marked tiles equal to its number.
  void RevealAndFocusUnrevealedAndNotMarkedAsBombsNeigboursIfIsBombSaturated(
    const std::array<U64, 2> &tile_coords
  ) {
    if (game_->Tile(tile_coords) > U8(8)) {return;}
    std::vector<std::array<U64, 2>> neigbourhood = Neighbourhood<>(tile_coords);
    U8 marked_as_bombs_neighbours_count = 0;
    std::vector<std::array<U64, 2>> unrevealed_and_not_marked_as_bombs_neighbours_coords;
    for (const std::array<U64, 2> &neighbour_coords : neigbourhood) {
      if (bomb_marked_tiles_coords_.contains(neighbour_coords)) {
        marked_as_bombs_neighbours_count++;
      } else if (game_->Tile(neighbour_coords) == U8(-2)) {
        unrevealed_and_not_marked_as_bombs_neighbours_coords.push_back(neighbour_coords);
      }
    }
    if (marked_as_bombs_neighbours_count == game_->Tile(tile_coords)) {
      for (
        const std::array<U64, 2> &unrevealed_and_not_marked_as_bomb_neighbour_coords :
        unrevealed_and_not_marked_as_bombs_neighbours_coords
      ) {
        game_->Reveal(unrevealed_and_not_marked_as_bomb_neighbour_coords);
        Focus(unrevealed_and_not_marked_as_bomb_neighbour_coords);
      }
    }
  }
  
  U64 UnknownTilesCount() const {
    return game_->Energy() - bomb_marked_tiles_coords_.size();
  }
  
  void BeginSolving() {FocusAllRevealedTiles();}
  
  // TODO: Consider avoiding use of virtual functions.
  void virtual Cycle() {
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
    // In case basic methods do not help, pass.
    if (initial_unknown_tiles_count == UnknownTilesCount()) {
      game_->Pass();
    } else {
      PrintGameField(bomb_marked_tiles_coords_);
    }
  }
  
 public:
  
  BasicMinesweeperSolver(MinesweeperGame &game, const bool &print_cycles = false):
  Base(game, print_cycles),
  focused_tiles_coords_({}),
  bomb_marked_tiles_coords_({})
  {}
  
};

}
