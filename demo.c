#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "src/sudoku.h"

int main(int agrc, char **argv) {
    SudokuBoard *p_board = malloc(sizeof(SudokuBoard));

    init_sudoku(p_board);
    load_sudoku(p_board, argv[1]);
    print_board(p_board);

    return 0;
}