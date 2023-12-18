#pragma once

#include <stdbool.h>
#include "sudoku.h"

typedef struct HiddenSingle_impl
{
    Cell *p_cell;
    int value;
} HiddenSingle;

// find hidden single values (the value that is unique among all the candidates for a row, a collumn, or a box)
int find_hidden_single_values(SudokuBoard *p_board, Cell *cell, HiddenSingle **hiddens_found_list);

// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(Cell *p_cell, Cell **find_target);

// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board);

void show_possible_hs(SudokuBoard *p_board);

bool check_constraint(Cell *p_cell, Cell **find_target, int value);