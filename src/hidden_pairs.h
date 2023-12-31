#pragma once

#include "sudoku.h"

typedef struct HiddenPairs_impl
{
    Cell *p_cell1;
    Cell *p_cell2;
    int *arr;  // array of values
} HiddenPairs;

int hidden_pairs(SudokuBoard *p_board);

void find_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs,
                       int *p_counter);

int find_hidden_pair_values(Cell **p_cells, int *hidden_pair_values);
