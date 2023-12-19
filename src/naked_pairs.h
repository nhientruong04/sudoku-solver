#pragma once

#include "sudoku.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

struct NakedPairs_Impl
{
    Cell *cells_pair[2];
    int value_pair[2];
};

typedef struct NakedPairs_Impl NakedPairs;

int naked_pairs(SudokuBoard *p_board);

// find all naked pairs
int find_nakedpairs(SudokuBoard *p_board, NakedPairs **naked_pairs_list);

// check if cell has naked pairs
bool has_naked_pairs(Cell *cell);

// get the corresponding cell having the same pair of candidates
Cell *get_counterpart(SudokuBoard *p_board, int *value_pair, Cell *cell, int row_index, int col_index, int box_index);

// find the pair cell in the same group (col, row, box)
Cell *find_counterpart_in_group(Cell **group, Cell *cell, int *value_pair);

void get_value_pair(Cell *cell, int *value_pair);
