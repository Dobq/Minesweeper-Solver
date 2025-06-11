
#include <fstream>
#include <sstream>

#include "alias\ad.h"
#include "minesweeper\solver\bucket.h"
#include "minesweeper\game\game.h"
#include "minesweeper\table\from.h"
#include "minesweeper\table\table.h"

int main(int argc, char *argv[]) {
  
  using namespace ad;
  
  static const bool print_cycles = true;
  
  std::stringstream input;
  
  input << std::ifstream(argv[1]).rdbuf();
  
  U64 starting_y;
  input >> starting_y;
  U64 starting_x;
  input >> starting_x;
  
  const MinesweeperTable table = MinesweeperTableFrom(input);
  
  MinesweeperGame game(table, {starting_y, starting_x});
  
  BucketMinesweeperSolver solver(game, print_cycles);
  
  std::cout << "\033[1;32mTotal bomb count is " << game.BombNum() << ".\033[0m" << std::endl;
  
  solver.Solve();
  
  return 0;
  
}
