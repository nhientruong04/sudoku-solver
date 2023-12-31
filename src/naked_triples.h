#pragma once

#include "sudoku.h"

typedef struct NakedTriples_impl
{
    Cell *p_cell1;
    Cell *p_cell2;
    Cell *p_cell3;

    int value1;
    int value2;
    int value3;
} NakedTriples;

int naked_triples(SudokuBoard *p_board);

void unset_triples(Cell **p_cells, int value1, int value2, int value3, Cell *p_cell1, Cell *p_cell2, Cell *p_cell3);

void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter);

bool is_naked_triples(Cell **p_cells, int i, int j, int k, int *p_naked_triples_candidates);
