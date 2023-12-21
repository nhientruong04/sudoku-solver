#include "hidden_pairs.h"

int hidden_pairs(SudokuBoard *p_board)
{
    int pairs_found = 0;
    int array_size = BOARD_SIZE * BOARD_SIZE - p_board->solved_counter;

    Cell **registered_cells = malloc(array_size * sizeof(Cell *));
    HiddenPairs **hidden_pairs_found = malloc(array_size * sizeof(HiddenPairs *));
    for (int i=0; i<BOARD_SIZE; i++)
    {
        for (int j=0; j<BOARD_SIZE; j++)
        {
            Cell *p_cell = &p_board->data[i][j];
            if ((p_cell->num_candidates)<=2)
            {
                continue;
            }

            find_hidden_pairs(p_cell, p_board, registered_cells, hidden_pairs_found, &pairs_found);
        }
    }

    for (int i=0; i<pairs_found; i++)
    {
        HiddenPairs *pair = hidden_pairs_found[i];
        for (int j=0; j<BOARD_SIZE; j++)
        {
            if (j == pair->value_pair[0] || j == pair->value_pair[1])
            {
                pair->members_cell[0]->candidates[j] = 1;
                pair->members_cell[1]->candidates[j] = 1;
                continue;
            }

            pair->members_cell[0]->candidates[j] = 0;
            pair->members_cell[1]->candidates[j] = 0;
        }
    }

    // clean up
    for (int i=0; i<pairs_found; i++)
    {
        free(hidden_pairs_found[i]);
    }
    free(hidden_pairs_found);
    free(registered_cells);

    return pairs_found;
}

void find_hidden_pairs(Cell *p_cell, SudokuBoard *p_board, Cell **registered_cells, HiddenPairs **hidden_pairs_found, int *pairs_found)
{
    for (int i=0; i<BOARD_SIZE; i++)
    {
        for (int j=0; j<BOARD_SIZE; j++)
        {
            Cell *p_counterpart = &p_board->data[i][j];
            if (p_counterpart == p_cell || p_counterpart->num_candidates<=2)
            {
                continue;
            }

            if (registered_pair(p_cell, p_counterpart, hidden_pairs_found, *pairs_found))
            {
                continue;
            }

            int *matched_positions = malloc(3 * sizeof(int));
            matched_positions[0] = 1;
            matched_positions[1] = 1;
            matched_positions[2] = 1;

            int matched_values[9];
            bool same_house = is_same_house_hidden_pairs(matched_positions, p_cell, p_counterpart, p_cell->row_index, p_cell->col_index, p_cell->box_index);
            bool same_candidates = check_validity(p_cell, p_counterpart, matched_values);

            if (same_candidates && same_house)
            {
                bool not_unique = pair_is_not_unique(matched_values, matched_positions, p_cell, p_counterpart, p_board);
                if (not_unique)
                {
                    free(matched_positions);
                    continue;
                }

                HiddenPairs *pair = malloc(sizeof(HiddenPairs));
                pair->members_cell[0] = p_cell;
                pair->members_cell[1] = p_counterpart;
                get_value_from_matched_ones(matched_values, pair->value_pair);

                pair->matched_positions = malloc(3 * sizeof(int));
                pair->matched_positions[0] = matched_positions[0];
                pair->matched_positions[1] = matched_positions[1];
                pair->matched_positions[2] = matched_positions[2];

                hidden_pairs_found[(*pairs_found)++] = pair;
            }

            free(matched_positions);
        }
    }
}

bool registered_pair(Cell *mem1, Cell *mem2, HiddenPairs **hidden_pairs_found, int pairs_found)
{
    bool ret = false;

    for (int i=0; i<pairs_found; i++)
    {
        Cell *ptr_1 = hidden_pairs_found[i]->members_cell[0];
        Cell *ptr_2 = hidden_pairs_found[i]->members_cell[1];

        ret |= (ptr_1 == mem1) && (ptr_2 == mem2);
        ret |= (ptr_2 == mem1) && (ptr_1 == mem2);

        if (ret)
        {
            return ret;
        }
    }

    return ret;
}

bool check_validity(Cell *mem1, Cell *mem2, int *matched_values)
{
    bool ret = false;
    int *candidates_1 = mem1->candidates;
    int *candidates_2 = mem2->candidates;

    for (int i=0; i<BOARD_SIZE; i++)
    {
        matched_values[i] = candidates_1[i] * candidates_2[i];
    }

    int counter = 0;
    for (int i=0; i<BOARD_SIZE; i++)
    {
        counter += matched_values[i];
    }

    if (counter >= 2)
    {
        ret = true;
    }

    return ret;
}

void get_value_from_matched_ones(int *matched_values, int *res_value)
{
    int counter = 0;

    for (int i=0; i<BOARD_SIZE; i++)
    {
        if (matched_values[i])
        {
            res_value[counter++] = i;
        }
    }
}

bool is_same_house_hidden_pairs(int *match_positions, Cell *first_cell, Cell *second_cell, int row_index, int col_index, int box_index)
{
    bool ret = false;
    int draft_positions[3];
    for (int i=0; i<3; i++)
    {
        draft_positions[i] = match_positions[i];
    }

    int ori_positions[3] = {row_index, col_index, box_index};
    int compare_positions[3] = {second_cell->row_index, second_cell->col_index, second_cell->box_index};
    
    for (int i=0; i<3; i++)
    {
        draft_positions[i] *= (ori_positions[i] == compare_positions[i]);
        ret |= draft_positions[i];
    }

    if (ret)
    {
        for (int i=0; i<3; i++)
        {
            match_positions[i] = draft_positions[i];
        }

        return ret;
    }
    
    return ret;
}

bool pair_is_not_unique(int *matched_values, int *matched_positions, Cell *first_cell, Cell *second_cell, SudokuBoard *p_board)
{
    bool ret = false;
    int row_index = first_cell->row_index;
    int col_index = first_cell->col_index;
    int box_index = first_cell->box_index;

    for (int i=0; i<3; i++)
    {
        if (matched_positions[i])
        {
            if (i == 0)
            {
                ret |= check_for_each_group(first_cell, second_cell, matched_values, p_board->p_rows[row_index]);
            }

            if (i == 1)
            {
                ret |= check_for_each_group(first_cell, second_cell, matched_values, p_board->p_cols[col_index]);
            }

            if (i == 2)
            {
                ret |= check_for_each_group(first_cell, second_cell, matched_values, p_board->p_boxes[box_index]);
            }
        }
    }

    return ret;
}

bool check_for_each_group(Cell *first_cell, Cell *second_cell, int *matched_values, Cell **group)
{
    bool ret = false;

    for (int j=0; j<BOARD_SIZE; j++)
    {
        Cell *test_cell = group[j];
        if (test_cell == first_cell || test_cell == second_cell)
        {
            continue;
        }

        for (int t=0; t<BOARD_SIZE; t++)
        {
            matched_values[t] *= !(test_cell->candidates[t]);
        }
    }

    int value_counter = 0;
    for (int i=0; i<BOARD_SIZE; i++)
    {
        if (matched_values[i])
        {
            value_counter++;
        }        
    }

    if (value_counter != 2)
    {
        ret = true;
    }

    return ret;
}