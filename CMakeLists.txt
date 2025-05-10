cmake_minimum_required(VERSION 3.30)
project(SudokuInC C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
        main.c
        play.c
        settings.c
        game.c
        board.c
        sudokuGenerator.c
        solver.c
)

add_executable(SudokuInC ${SOURCE_FILES})

target_link_libraries(SudokuInC m)