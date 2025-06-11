
:: This script creates catch2\catch_main.o file.

:: catch2\catch.hpp file must be provided, it can be obtained under following link:
:: https://github.com/catchorg/Catch2/releases/v2.13.10,
:: in case this link does not work somewhen in future: you basically need single-header catch.hpp,
:: catch version 2.13.10 (this is one I used, although most of versions will work I guess).

:: repository_path variable needs to be set accordingly.

@SET repository_path=C:\minesweeper_solver
@SET compile=g++ -Wall -static-libstdc++ -std=c++20 -I"%repository_path%"
@SET compile_catch=%compile% %repository_path%\testing\catch2\catch_main.cc -o catch2\catch_main.o -c

%compile_catch%
