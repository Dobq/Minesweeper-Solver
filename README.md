
# Minesweeper Solver

## Description

Minesweeper Solver is a program that solves popular [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game)) game to the best possible extent without taking risk, in other words it reveals as much tiles as possible without taking risk of triggering the bomb.

## Compiling

C++20 compiler is required, I used GCC 14.2.0.

In order to compile tests and `main.exe` file, check and run batch files in `testing` folder, of course they work only on Windows, on Linux similar bash files should do the job.

Compiling tests requires Catch2 single-header library [file](https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp) and it is the only dependency of this repository, detailed info is included in aforementioned batch files.

## Running

After running batch scripts `main.exe` binary appears in `testing\binaries` folder, when in this folder, run for example following line:

```
.\main.exe ..\..\exemplary_inputs\input_0.txt
```

Colorful step-by-step solution should be printed in the terminal.

There are eight exemplary input files in this folder.

## Some technical details

The main goal was to create a program like it is described in the Description section: capable of revealing as many tiles as it is possible without risk of triggering the bomb.

Talking in simple words algorithm repeats two computationally cheap things:

1. If some revealed tile has its all neighbouring bombs marked, reveal its neighbours that are not marked as bombs.
2. If some revealed tile has unrevealed neighbours number equal to its neigbouring bombs number, mark all unrevealed neighbours as bombs.

If that is not enough to proceed, do following thing:

3. Check all possible bomb configurations within unrevealed tiles that have revealed neigbour; if there are tiles found to always contain a bomb or always be bombfree, mark them as bombs or reveal them. Take total bomb count into account, find if status of other tiles can be concluded.

That third step, if done naively, has exponential time-and-space complexity, in order to avoid that, possible configurations of some small area (unrevealed neighbours of specific revealed tile) are found first, and then similary small areas are appended (and possible bomb configurations with them are found) one by one, this can reduce complexity *hugely*, sometimes down to linear complexity. If you are checking the code, that "small areas" correspond to buckets in code of `minesweeper\solver\bucket.h` file.

For sure there are some micro opts that can be done. Three possibly more meaningful opts (especially for some very big input instances) I can think of are:

1. Adding aforementioned "small areas" in a way that newly added ones have nonempty crossection with ones that are there already (this would guarantee more effective complexity reduction), currently order is somewhat pseudorandom.
2. Consider each connected sum of "small areas" separately; if there are `n` connected sums of `a_1` to `a_n` tiles each, this would reduce space complexity from `O(a_1) * ... * O(a_n)` to `O(a_1) + ... + O(a_n)` and computational complexity would be reduced similarily.
3. Instead of doing aforementioned third step, redesign `BucketProblemSolver` of `bucket_problem\solver.h` to be capable of more dynamic work (simply speaking, so the current output of specific instance of `BucketProblemSolver` can be a new input for the same instance and it will work in sensible way, let's say old data will be deleted when no longer useful etc.), this way the whole computations would be enclosed within single a instance of `BucketProblemSolver` and some redundant recomputations would be avoided.
