#include "naked_pairs.h"
#include <stdio.h>

int naked_pairs(SudokuBoard *p_board)
{
    // init NakedPairs array
    int array_size = (BOARD_SIZE*BOARD_SIZE - p_board->solved_counter) /2;
    // save all pairs
    NakedPairs **naked_pairs_list = malloc(array_size * sizeof(NakedPairs *));
    // save all cells that are already paired
    Cell **registered_cells = malloc(array_size * 2 * sizeof(Cell*));

    // detect nakedpairs and register the pairs to naked_pairs_list
    int pairs_count = find_nakedpairs(p_board, naked_pairs_list, registered_cells);

    // apply naked pairs effect
    for (int i=0; i<pairs_count; i++)
    {
        apply_naked_pairs_effect(p_board, naked_pairs_list[i]);
    }

    // free all ptrs
    for (int i=0; i<pairs_count; i++)
    {
        free(naked_pairs_list[i]);
    }
    free(registered_cells);
    free(naked_pairs_list);

    return pairs_count;
}

// void print_naked_pairs(NakedPairs **naked_pairs_list)
// {

// }

int find_nakedpairs(SudokuBoard *p_board, NakedPairs **naked_pairs_list, Cell **registered_cells)
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

                if (!is_registered(second_cell, registered_cells, pairs_count*2))
                {
                    NakedPairs *pair = malloc(sizeof(NakedPairs));
                    pair->cells_pair[0] = first_cell;
                    pair->cells_pair[1] = second_cell;
                    pair->value_pair[0] = value_pair[0];
                    pair->value_pair[1] = value_pair[1];

                    // register cells ptrs
                    registered_cells[pairs_count * 2] = first_cell;
                    registered_cells[pairs_count * 2 + 1] = second_cell;

                    naked_pairs_list[pairs_count++] = pair;
                }
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

        // check if has constraint of naked pairs
        if (!has_naked_pairs(p_cell))
        {
            continue;
        }
        
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

bool is_registered(Cell *cell, Cell **registered_cells, int size) 
{
    bool ret = false;

    for (int i=0; i<size; i++)
    {
        if (cell == registered_cells[i])
        {
            ret = true;
            break;
        }
    }

    return ret;
}

void apply_naked_pairs_effect(SudokuBoard *p_board, NakedPairs *pair)
{
    // [row, col, box]
    int match_positions[3] = {0, 0, 0};
    get_match_type(pair->cells_pair[0], pair->cells_pair[1],match_positions);

    for (int i=0; i<3; i++)
    {
        if (match_positions[i] == 0)
        {
            continue;
        }

        if (i == 0)
        {
            unset_candidates_naked_pairs(pair, p_board->p_rows[pair->cells_pair[0]->row_index]);
        }

        if (i == 1)
        {
            unset_candidates_naked_pairs(pair, p_board->p_cols[pair->cells_pair[0]->col_index]);
        }

        if (i == 2)
        {
            unset_candidates_naked_pairs(pair, p_board->p_boxes[pair->cells_pair[0]->box_index]);
        }
    }
}

void get_match_type(Cell *first_cell, Cell *second_cell, int *match_positions)
{
    if (first_cell->row_index == second_cell->row_index)
    {
        match_positions[0] = 1;
    }

    if (first_cell->col_index == second_cell->col_index)
    {
        match_positions[1] = 1;
    }

    if (first_cell->box_index == second_cell->box_index)
    {
        match_positions[2] = 1;
    }
}

void unset_candidates_naked_pairs(NakedPairs *pair, Cell **group)
{
    int candidate1 = pair->value_pair[0];
    int candidate2 = pair->value_pair[1];

    for (int i=0; i<BOARD_SIZE; i++)
    {
        Cell *cell = group[i];

        if (cell->value > 0)
        {
            continue;
        }

        if (cell==pair->cells_pair[0] || cell==pair->cells_pair[1])
        {
            continue;
        }

        if (cell->candidates[candidate1])
        {
            unset_candidate(cell, candidate1 + 1);
        }

        if (cell->candidates[candidate2])
        {
            unset_candidate(cell, candidate2 + 1);
        }
    }
}