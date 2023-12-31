#include "hidden_singles.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int hidden_singles(SudokuBoard *p_board)
{
    HiddenSingle **hiddens_found_list = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(HiddenSingle *));
    
    int count = 0;

    // Check every cell to find hidden values
    for(int i=0; i<BOARD_SIZE; i++)
    {
        for (int j=0; j<BOARD_SIZE; j++)
        {
            // If cell has not been solved
            if (p_board->data[i][j].value == 0)
            {
                int hidden_num = find_hidden_single_values(p_board, &p_board->data[i][j], hiddens_found_list);

                if (hidden_num != 0)
                {
                    HiddenSingle *found_cell = malloc(sizeof(HiddenSingle));
                    found_cell->p_cell = &p_board->data[i][j];
                    found_cell->value = hidden_num;

                    hiddens_found_list[count] = found_cell;

                    count++;
                }
            }
        }
    }

    int set_cell_count = 0;
    while (set_cell_count < count)
    {
        HiddenSingle *hs_cell = hiddens_found_list[set_cell_count];
        int candidates[] = {hs_cell->value};
        set_candidates(hs_cell->p_cell, candidates, 1);

        free(hiddens_found_list[set_cell_count]);
        set_cell_count++;
    }
   
    free(hiddens_found_list);

    return count;
}

void find_hidden_single(Cell *p_cell, Cell **find_target)
{
    for(int i=0; i<BOARD_SIZE; i++)
    {
        int value = find_target[i]->value;
        if(value && p_cell->candidates[value-1])
        {
            unset_candidate(p_cell, value);
        }
    }
}

bool check_constraint(Cell *p_cell, Cell **find_target, int value)
{
    bool is_unique = true;

    for(int target_cell=0; target_cell<BOARD_SIZE; target_cell++)
    {
        if(find_target[target_cell]->candidates[value] && find_target[target_cell] != p_cell)
        {
            is_unique = false;
            break;
        }
    }

    return is_unique;
}

int find_hidden_single_values(SudokuBoard *p_board, Cell *cell, HiddenSingle **hiddens_found_list)
{
    int hidden_value = 0;
    
    // Check every candidate
    for (int num=0; num<BOARD_SIZE; num++)
    {
        if (cell->candidates[num])
        {
            bool check_value = false;

            if (cell->num_candidates == 1) 
            {
                check_value = true;
            }

            int box_index = cell->box_index;
            int row_index = cell->row_index;
            int col_index = cell->col_index;

            check_value |= check_constraint(cell, p_board->p_boxes[box_index], num);
            check_value |= check_constraint(cell, p_board->p_cols[col_index], num);
            check_value |= check_constraint(cell, p_board->p_rows[row_index], num);

            if(check_value)
            {
                hidden_value = num+1;
                break;
            }
        }
    }

    return hidden_value;
}