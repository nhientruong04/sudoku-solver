#include "naked_pairs.h"
#include <stdio.h>

int naked_pairs(SudokuBoard *p_board)
{
    // init NakedPairs array
    int array_size = (BOARD_SIZE*BOARD_SIZE - p_board->solved_counter) /2;
    NakedPairs **naked_pairs_list = malloc(array_size * sizeof(NakedPairs *));

    int pairs_count = find_nakedpairs(p_board, naked_pairs_list);

    // for (int i=0; i<array_size; i++)
    // {
    //     free(naked_pairs_list[i]);
    // }
    free(naked_pairs_list);

    return pairs_count;
}

// void print_naked_pairs(NakedPairs **naked_pairs_list)
// {

// }

int find_nakedpairs(SudokuBoard *p_board, NakedPairs **naked_pairs_list)
{
    int pairs_count = 0;

    for (int i=0; i<BOARD_SIZE; i++)
    {
        for (int j=0; j<BOARD_SIZE; j++)
        {
            Cell cell = p_board->data[i][j];
            if (cell.value==0 && has_naked_pairs(&cell))
            {
                Cell *first_cell = &p_board->data[i][j];
                int value_pair[2];
                get_value_pair(first_cell, value_pair);

                Cell *second_cell = get_counterpart(p_board, value_pair, first_cell, first_cell->row_index, first_cell->col_index, first_cell->box_index);

                if (second_cell == NULL)
                {
                    continue;
                }

                NakedPairs *pair = malloc(sizeof(NakedPairs));
                pair->cells_pair[0] = first_cell;
                pair->cells_pair[1] = second_cell;
                pair->value_pair[0] = value_pair[0];
                pair->value_pair[1] = value_pair[1];

                naked_pairs_list[pairs_count++] = pair;
            }
        }
    }

    return pairs_count;
}

bool has_naked_pairs(Cell *cell)
{
    bool ret = false;

    if (cell->num_candidates == 2)
    {
        ret = true;
    }

    return ret;
}

Cell *get_counterpart(SudokuBoard *p_board, int *value_pair, Cell *cell, int row_index, int col_index, int box_index)
{
    Cell *counterpart_ptr = NULL;
    
    counterpart_ptr = find_counterpart_in_group(p_board->p_rows[row_index], cell, value_pair);
    if (counterpart_ptr != NULL)
    {
        return counterpart_ptr;
    }

    counterpart_ptr = find_counterpart_in_group(p_board->p_cols[col_index], cell, value_pair);
    if (counterpart_ptr != NULL)
    {
        return counterpart_ptr;
    }

    counterpart_ptr = find_counterpart_in_group(p_board->p_boxes[box_index], cell, value_pair);
    if (counterpart_ptr != NULL)
    {
        return counterpart_ptr;
    }

    return counterpart_ptr;
}

Cell *find_counterpart_in_group(Cell **group, Cell *cell, int *value_pair)
{
    Cell *ret_ptr = NULL;

    for (int i=0; i<BOARD_SIZE; i++)
    {
        Cell *p_cell = group[i];
        
        if (p_cell->candidates[value_pair[0]] && p_cell->candidates[value_pair[1]] && p_cell != cell)
        {
            ret_ptr = p_cell;
            break;
        }
    }

    return ret_ptr;
}

void get_value_pair(Cell *cell, int *value_pair)
{
    int *candidates = cell->candidates;
    int counter = 0;
    
    for (int i=0; i<BOARD_SIZE; i++)
    {
        if (candidates[i])
        {
            value_pair[counter++] = i;
        }

        if (counter == 2)
        {
            break;
        }
    }
}