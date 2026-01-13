
cmake_minimum_required(VERSION 3.15)
project(MinesweeperSolver CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -static-libstdc++")

enable_testing()

add_executable(main main.cc)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(catch2_main OBJECT testing/catch2/catch_main.cc)
target_include_directories(catch2_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set(
    TEST_FILES
    minesweeper/game/game_test.cc
    minesweeper/print/field_test.cc
    minesweeper/solver/basic_test.cc
    minesweeper/solver/bucket_test.cc
    minesweeper/solver/dummy_test.cc
    minesweeper/table/from_test.cc
    minesweeper/table/table_test.cc
    bucket_problem/solver_test.cc
    random/engine/xorshift_64_test.cc
    random/rng/rng_test.cc
)

# Separate executable for each test, altough takes slightly more disk space,
# makes usage of some VSCode CMake extensions more convenient, than single executable for all tests.
foreach(TEST_FILE ${TEST_FILES})
    # To create test name we use full path to ensure collisions are avoided.
    string(REPLACE "/" "-" TEST_NAME ${TEST_FILE})
    string(REPLACE "_test.cc" "" TEST_NAME ${TEST_NAME})
    
    add_executable(${TEST_NAME} ${TEST_FILE} $<TARGET_OBJECTS:catch2_main>)    
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_source_files_properties(${TEST_FILE} PROPERTIES COMPILE_OPTIONS "-D_GLIBCXX_DEBUG")
    
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
