#include "hidden_pairs.h"
#include <stdlib.h>
#include <stdio.h>

int hidden_pairs(SudokuBoard *p_board)
{
    int counter = 0;
    HiddenPairs hidden_pairs[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_pairs(p_board->p_rows[i], hidden_pairs, &counter);
        find_hidden_pairs(p_board->p_cols[i], hidden_pairs, &counter);
        find_hidden_pairs(p_board->p_boxes[i], hidden_pairs, &counter);
    }

    for (int i = 0; i < counter; i++)
    {
        // printf("Hidden single: %d %d %d\n", hidden_singles[i].p_cell->row_index, hidden_singles[i].p_cell->col_index, hidden_singles[i].value);
        set_candidates(hidden_pairs[i].p_cell1, hidden_pairs[i].arr, 2);
        set_candidates(hidden_pairs[i].p_cell2, hidden_pairs[i].arr, 2);
    }

    for (int i = 0; i < counter; i++)
    {
        free(hidden_pairs[i].arr);
    }
    return counter;
}

void find_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs,
                       int *p_counter)
{
    int hidden_pair_values[BOARD_SIZE] = {0};
    int hidden_pair_value_counter = find_hidden_pair_values(p_cells, hidden_pair_values);

    for (int m = 0; m < hidden_pair_value_counter - 1; m++)
    {
        for (int n = m + 1; n < hidden_pair_value_counter; n++)
        {
            int *pair = malloc(sizeof(int) * 2);
            pair[0] = hidden_pair_values[m];
            pair[1] = hidden_pair_values[n];
            bool flagged = false;
            int counter = 0; // number of cells with hidden pair values
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                if (p_cells[i]->fixed || p_cells[i]->num_candidates <= 2)
                {
                    continue;
                }

                int *candidates = get_candidates(p_cells[i]);

                int count_same = 0;
                for (int j = 0; j < p_cells[i]->num_candidates; j++)
                {
                    if (candidates[j] == pair[0] || candidates[j] == pair[1])
                    {
                        count_same++;
                    }
                }

                if (count_same == 2)
                {
                    if (!flagged)
                    {
                        p_hidden_pairs[*p_counter].p_cell1 = p_cells[i];
                        flagged = true;
                    }
                    else
                    {
                        p_hidden_pairs[*p_counter].p_cell2 = p_cells[i];
                    }
                    counter++;
                }

                free(candidates);
            }

            if (counter != 2) // not enough cells with hidden pair values
            {
                free(pair);
                continue;
            }

            bool duplicate = false;
            for (int i = 0; i < *p_counter; i++)
            {
                if (p_hidden_pairs[i].p_cell1 == p_hidden_pairs[*p_counter].p_cell1 && p_hidden_pairs[i].p_cell2 == p_hidden_pairs[*p_counter].p_cell2)
                {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate)
            {
                p_hidden_pairs[*p_counter].arr = pair;
                *p_counter += 1;
            }
            else
            {
                free(pair);
            }
        }
    }
}

int find_hidden_pair_values(Cell **p_cells, int *hidden_pair_values)
{
    int hidden_pair_value_counter = 0;
    int candidate_counter[BOARD_SIZE] = {0};

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->fixed || p_cells[i]->num_candidates == 1)
        {
            continue;
        }

        int *candidates = get_candidates(p_cells[i]);
        for (int j = 0; j < p_cells[i]->num_candidates; j++)
        {
            candidate_counter[candidates[j] - 1] += 1;
        }
        free(candidates);
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (candidate_counter[i] == 2)
        {
            hidden_pair_values[hidden_pair_value_counter] = i + 1;
            hidden_pair_value_counter++;
        }
    }

    return hidden_pair_value_counter;
}
