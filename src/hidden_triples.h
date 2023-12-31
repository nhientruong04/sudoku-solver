#pragma once

#include "sudoku.h"

typedef struct hidden_triples_impl
{
    Cell *p_cell1;
    Cell *p_cell2;
    Cell *p_cell3;
    int *arr;  // array of values
} HiddenTriples;


int hidden_triples(SudokuBoard *p_board);

void find_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples,
                         int *p_counter);

int find_hidden_triple_values(Cell **p_cells, int *hidden_triple_values);

void unset_hidden_triple_values(HiddenTriples *p_hidden_triples, int *num_hidden_triples);