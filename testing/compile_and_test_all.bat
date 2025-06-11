
:: This is some primitive script to recompile and execute
:: all tests (and main binary) in the repository.

:: Compiling tests requires catch2\catch_main.o file to exist.

:: Every test name and test path must be added here;
:: repository_path variable needs to be set accordingly.

@SET repository_path=C:\minesweeper_solver
@SET compile=g++ -Wall -static-libstdc++ -std=c++20 -I"%repository_path%"
@SET compile_test=%compile% -D_GLIBCXX_DEBUG %repository_path%\testing\catch2\catch_main.o -o tests\
@SET compile_binary=%compile% -o binaries\

:: Creating folders if not present.
@echo off
if not exist "tests" mkdir tests
if not exist "binaries" mkdir binaries
@echo on

:: %compile_test%[test_name] [test_path]
%compile_test%solver_test %repository_path%\bucket_problem\solver_test.cc
%compile_test%game_test %repository_path%\minesweeper\game\game_test.cc
%compile_test%field_test %repository_path%\minesweeper\print\field_test.cc
%compile_test%basic_test %repository_path%\minesweeper\solver\basic_test.cc
%compile_test%bucket_test %repository_path%\minesweeper\solver\bucket_test.cc
%compile_test%dummy_test %repository_path%\minesweeper\solver\dummy_test.cc
%compile_test%from_test %repository_path%\minesweeper\table\from_test.cc
%compile_test%table_test %repository_path%\minesweeper\table\table_test.cc
%compile_test%xorshift_64_test %repository_path%\random\engine\xorshift_64_test.cc
%compile_test%rng_test %repository_path%\random\rng\rng_test.cc

:: %compile%[binary_name] [binary_path]
%compile_binary%main %repository_path%\main.cc

@cd tests

:: [test_name]
solver_test
game_test
field_test
basic_test
bucket_test
dummy_test
from_test
table_test
xorshift_64_test
rng_test

@cd ..
