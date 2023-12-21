#pragma once

#include "sudoku.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

struct HiddenPairs_Impl
{
    Cell *members_cell[2];
    int value_pair[2];
    int *matched_positions;
};

typedef struct HiddenPairs_Impl HiddenPairs;

int hidden_pairs(SudokuBoard *p_board);

bool registered_pair(Cell *mem1, Cell *mem2, HiddenPairs **hidden_pairs_found, int pairs_found);
void find_hidden_pairs(Cell *p_cell, SudokuBoard *p_board, Cell **registered_cells, HiddenPairs **hidden_pairs_found, int *pairs_found);
void get_value_from_matched_ones(int *matched_values, int *res_value);
bool check_validity(Cell *mem1, Cell *mem2, int *matched_values);
bool is_same_house_hidden_pairs(int *match_positions, Cell *first_cell, Cell *second_cell, int row_index, int col_index, int box_index);
bool pair_is_not_unique(int *value_pair, int *matched_positions, Cell *first_cell, Cell *second_cell, SudokuBoard *p_board);
bool check_for_each_group(Cell *first_cell, Cell *second_cell, int *value_pair, Cell **group);