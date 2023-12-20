#pragma once

#include "sudoku.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

int naked_triples(SudokuBoard *p_board);

struct NakedTriples_Impl
{
    Cell *cells_triples[3];
    int value_triples[3];
    int match_positions[3];
    int positions[3];
};

typedef struct NakedTriples_Impl NakedTriples;

struct NakedTriplesPossibleMembers
{
    Cell *cell;
    int *value_triples;
    int value_size;
    bool used;
};

typedef struct NakedTriplesPossibleMembers PossibleMembers;

// find all possible naked triples members
int find_nakedTriples_members(SudokuBoard *p_board, PossibleMembers **possible_member_cells);

// check if cell has naked triples
// bool has_naked_triple(Cell *cell);

// find conventional triples
bool find_convention_triples(NakedTriples *triple, PossibleMembers **possible_member_cells, NakedTriples **naked_triples_list, int size);
bool find_convention_triples_step2(NakedTriples *triple, PossibleMembers *member2, PossibleMembers **possible_member_cells, NakedTriples **naked_triples_list, int size);

// find special triples
bool find_special_triples(NakedTriples *triple, PossibleMembers **possible_member_cells, NakedTriples **naked_triples_list, int size);
bool find_special_triples_step2(NakedTriples *triple, PossibleMembers *member2, PossibleMembers **possible_member_cells, NakedTriples **naked_triples_list, int size, int odd_candidate);

// find and unset duplicate candidates at other cells
void apply_naked_triples_effect(SudokuBoard *p_board, NakedTriples *triple);

// check if the original cell and the potential cell is in the same house
bool is_same_house(int *match_positions, PossibleMembers *second_cell, int row_index, int col_index, int box_index);

void unset_candidates_naked_triples(NakedTriples *triple, Cell **group);
void get_value(Cell *cell, int *value_triples, int num_candidates);