#include "hidden_triples.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int hidden_triples(SudokuBoard *p_board)
{
    int counter = 0;
    HiddenTriples hidden_triples[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_triples(p_board->p_rows[i], hidden_triples, &counter);
        find_hidden_triples(p_board->p_cols[i], hidden_triples, &counter);
        find_hidden_triples(p_board->p_boxes[i], hidden_triples, &counter);
    }

    unset_hidden_triple_values(hidden_triples, &counter);

    // free hidden_triple_values
    for (int i = 0; i < counter; i++)
    {
        free(hidden_triples[i].arr);
    }

    return counter;
}

void find_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples,
                         int *p_counter)
{
    HiddenTriples temp;

    int hidden_triple_values[BOARD_SIZE] = {0};
    int hidden_triple_value_counter = find_hidden_triple_values(p_cells, hidden_triple_values);

    if (hidden_triple_value_counter < 3) // not enough hidden triple values
    {
        return;
    }

    for (int first = 0; first < hidden_triple_value_counter - 2; first++)
    {
        for (int second = first + 1; second < hidden_triple_value_counter - 1; second++)
        {
            for (int third = second + 1; third < hidden_triple_value_counter; third++)
            {
                int *triple = malloc(sizeof(int) * 3);
                triple[0] = hidden_triple_values[first];
                triple[1] = hidden_triple_values[second];
                triple[2] = hidden_triple_values[third];

                int arr_counter = 0; // address next cell in arr
                int counter = 0;     // number of cells with hidden triple values
                int count_naked = 0; // number of cells with naked triple values
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    if (p_cells[i]->num_candidates == 1 || p_cells[i]->fixed)
                    {
                        continue;
                    }

                    int *candidates = get_candidates(p_cells[i]);

                    int count_same = 0;
                    for (int j = 0; j < p_cells[i]->num_candidates; j++)
                    {
                        if (candidates[j] == triple[0] || candidates[j] == triple[1] || candidates[j] == triple[2])
                        {
                            {
                                count_same++;
                            }
                        }
                    }
                    free(candidates);

                    if (count_same == p_cells[i]->num_candidates)  // all candidates are hidden triple values --> not hidden --> skip
                    {
                        count_naked++;
                    }

                    if (count_same == 3 || count_same == 2)
                    {
                        switch (arr_counter)
                        {
                        case 0:
                            temp.p_cell1 = p_cells[i];
                            break;
                        case 1:
                            temp.p_cell2 = p_cells[i];
                            break;
                        case 2:
                            temp.p_cell3 = p_cells[i];
                            break;
                        }

                        arr_counter++;
                        counter++;
                    }
                }

                temp.arr = triple; // save hidden triple values in temp struct to be copied to p_hidden_triples later
                if (counter != 3)  // not enough cells with hidden triple values
                {
                    free(temp.arr);
                    break;
                }

                bool flagged = false;
                // check if hidden triple values appear in other cells except the 3 cells
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    if (p_cells[i] == temp.p_cell1 || p_cells[i] == temp.p_cell2 || p_cells[i] == temp.p_cell3)
                    {
                        continue;
                    }

                    if (p_cells[i]->fixed || p_cells[i]->num_candidates == 1)
                    {
                        continue;
                    }

                    int *candidates = get_candidates(p_cells[i]);
                    for (int j = 0; j < p_cells[i]->num_candidates; j++)
                    {
                        if (candidates[j] == triple[0] || candidates[j] == triple[1] || candidates[j] == triple[2])
                        {
                            flagged = true;
                        }
                    }
                    free(candidates);
                }

                if (count_naked == 3) // hidden triple values are also naked triple values
                {
                    flagged = true;
                }

                if (flagged) // hidden triple values are not only in 3 cells
                {
                    free(temp.arr);
                    return;
                }

                p_hidden_triples[*p_counter] = temp; // BUG: this is not a deep copy. Values in p_hidden_triples[*p_counter].arr will be overwritten in next iteration

                // free(hidden_triple_values);
                // free(temp.arr);

                *p_counter += 1;
            }
        }
    }
}

int find_hidden_triple_values(Cell **p_cells, int *hidden_triple_values)
{
    int hidden_triple_values_counter = 0;
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
            candidate_counter[candidates[j] - 1]++;
        }

        free(candidates);
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (candidate_counter[i] == 3 || candidate_counter[i] == 2)
        {
            hidden_triple_values[hidden_triple_values_counter++] = i + 1;
        }
    }

    return hidden_triple_values_counter;
}

void unset_hidden_triple_values(HiddenTriples *p_hidden_triples, int *num_hidden_triples)
{
    for (int i = 0; i < *num_hidden_triples; i++)
    {
        int *candidates_1 = get_candidates(p_hidden_triples[i].p_cell1);
        int *candidates_2 = get_candidates(p_hidden_triples[i].p_cell2);
        int *candidates_3 = get_candidates(p_hidden_triples[i].p_cell3);

        int temp[BOARD_SIZE] = {0};
        for (int j = 0; j < p_hidden_triples[i].p_cell1->num_candidates; j++)
        {
            if (candidates_1[j] != p_hidden_triples[i].arr[0] && candidates_1[j] != p_hidden_triples[i].arr[1] && candidates_1[j] != p_hidden_triples[i].arr[2])
            {
                temp[candidates_1[j] - 1]++; // save candidate to be unset later
            }
        }
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (temp[j] != 0)
            {
                unset_candidate(p_hidden_triples[i].p_cell1, j + 1);
            }
        }

        memset(temp, 0, sizeof(int) * BOARD_SIZE);

        for (int j = 0; j < p_hidden_triples[i].p_cell2->num_candidates; j++)
        {
            if (candidates_2[j] != p_hidden_triples[i].arr[0] && candidates_2[j] != p_hidden_triples[i].arr[1] && candidates_2[j] != p_hidden_triples[i].arr[2])
            {
                temp[candidates_2[j] - 1]++; // save candidate to be unset later
            }
        }
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (temp[j] != 0)
            {
                unset_candidate(p_hidden_triples[i].p_cell2, j + 1);
            }
        }

        memset(temp, 0, sizeof(int) * BOARD_SIZE);

        for (int j = 0; j < p_hidden_triples[i].p_cell3->num_candidates; j++)
        {
            if (candidates_3[j] != p_hidden_triples[i].arr[0] && candidates_3[j] != p_hidden_triples[i].arr[1] && candidates_3[j] != p_hidden_triples[i].arr[2])
            {
                temp[candidates_3[j] - 1]++; // save candidate to be unset later
            }
        }
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (temp[j] != 0)
            {
                unset_candidate(p_hidden_triples[i].p_cell3, j + 1);
            }
        }

        free(candidates_1);
        free(candidates_2);
        free(candidates_3);
    }
}