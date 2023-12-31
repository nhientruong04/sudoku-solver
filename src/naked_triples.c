#include "naked_triples.h"
#include <stdio.h>
#include <stdlib.h>

int naked_triples(SudokuBoard *p_board)
{
    // find naked triples in rows
    int naked_triples_counter = 0;
    NakedTriples naked_triples[BOARD_SIZE * BOARD_SIZE];

    // find and unset
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_triples(p_board->p_rows[i], naked_triples, &naked_triples_counter);
        find_naked_triples(p_board->p_cols[i], naked_triples, &naked_triples_counter);
        find_naked_triples(p_board->p_boxes[i], naked_triples, &naked_triples_counter);
    }

    return naked_triples_counter;
}

void unset_triples(Cell **p_cells, int value1, int value2, int value3, Cell *p_cell1, Cell *p_cell2, Cell *p_cell3)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i] != p_cell1 && p_cells[i] != p_cell2 && p_cells[i] != p_cell3)
        {
            unset_candidate(p_cells[i], value1);
            unset_candidate(p_cells[i], value2);
            unset_candidate(p_cells[i], value3);
        }
    }
}

bool is_naked_triples(Cell **p_cells, int i, int j, int k, int *p_naked_triples_candidates)
{
    int *candidates_i = get_candidates(p_cells[i]);
    int *candidates_j = get_candidates(p_cells[j]);
    int *candidates_k = get_candidates(p_cells[k]);

    for (int l = 0; l < p_cells[i]->num_candidates; l++)
    {
        p_naked_triples_candidates[candidates_i[l]-1] += 1;
    }

    for (int l = 0; l < p_cells[j]->num_candidates; l++)
    {
        p_naked_triples_candidates[candidates_j[l]-1] += 1;
    }

    for (int l = 0; l < p_cells[k]->num_candidates; l++)
    {
        p_naked_triples_candidates[candidates_k[l]-1] += 1;
    }

    free(candidates_i);
    free(candidates_j);
    free(candidates_k);

    int count = 0;
    for (int l = 0; l < BOARD_SIZE; l++)
    {
        if (p_naked_triples_candidates[l] >= 1)
        {
            count++;
        }
        if (count > 3)
        {
            return false;
        }
    }


    return true;
}

void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter)
{
    for (int i = 0; i < BOARD_SIZE-2; i++)
    {
        if (p_cells[i]->fixed || p_cells[i]->num_candidates > 3 || p_cells[i]->num_candidates < 2)
        {
            continue;
        }

        for (int j = i+1; j < BOARD_SIZE-1; j++)
        {
            if (p_cells[j]->fixed || p_cells[j]->num_candidates > 3 || p_cells[j]->num_candidates < 2)
            {
                continue;
            }

            for (int k = j+1; k < BOARD_SIZE; k++)
            {
                if (p_cells[k]->fixed || p_cells[k]->num_candidates > 3 || p_cells[k]->num_candidates < 2)
                {
                    continue;
                }

                int naked_triples_candidates[BOARD_SIZE] = {0};
                if (is_naked_triples(p_cells, i, j, k, naked_triples_candidates))
                {
                    // unset candidates for other cells in the same row/col/box
                    for (int l = 0; l < BOARD_SIZE; l++)
                    {
                        if (naked_triples_candidates[l] >= 1)
                        {
                            for (int m = 0; m < BOARD_SIZE; m++)
                            {
                                if ((p_cells[m]->num_candidates > 1) && (p_cells[m] != p_cells[i]) && (p_cells[m] != p_cells[j]) && (p_cells[m] != p_cells[k]))
                                {
                                    for (int n = 0; n < BOARD_SIZE; n++)
                                    {
                                        if ((p_cells[m]->candidates[n] == 1) && (n == l))
                                        {
                                            unset_candidate(p_cells[m], l+1);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Check if the pair is already in the list
                    bool duplicate = false;
                    for (int l = 0; l < *p_counter; l++)
                    {
                        if (p_naked_triples[l].p_cell1 == p_cells[i] && p_naked_triples[l].p_cell2 == p_cells[j] && p_naked_triples[l].p_cell3 == p_cells[k])
                        {
                            duplicate = true;
                            break;
                        }
                    }

                    if (!duplicate)
                    {
                        p_naked_triples[*p_counter].p_cell1 = p_cells[i];
                        p_naked_triples[*p_counter].p_cell2 = p_cells[j];
                        p_naked_triples[*p_counter].p_cell3 = p_cells[k];
                        *p_counter += 1;
                    }
                }
            }
        }
    }
}