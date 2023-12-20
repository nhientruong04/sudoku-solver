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
int find_nakedpairs(SudokuBoard *p_board, NakedPairs **naked_pairs_list, Cell **registered_cells);

// check if cell has naked pairs
bool has_naked_pairs(Cell *cell);

// get the corresponding cell having the same pair of candidates
Cell *get_counterpart(SudokuBoard *p_board, int *value_pair, Cell *cell, int row_index, int col_index, int box_index);

// find the pair cell in the same group (col, row, box)
Cell *find_counterpart_in_group(Cell **group, Cell *cell, int *value_pair);

// find and remove duplicate candidates at other cells compared to found naked pairs cells
void apply_naked_pairs_effect(SudokuBoard *p_board, NakedPairs *pair);

// find the matching type of the two cells
void get_match_type(Cell *first_cell, Cell *second_cell, int *match_positions);

// unset candidates at other cells
void unset_candidates_naked_pairs(NakedPairs *pair, Cell **group);

bool is_registered(Cell *cell, Cell **registered_cells, int size);
void get_value_pair(Cell *cell, int *value_pair);
