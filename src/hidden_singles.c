#include "hidden_singles.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int hidden_singles(SudokuBoard *p_board)
{
    HiddenSingle **hiddens_found_list = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(HiddenSingle *));

    show_possible_hs(p_board);
    
    int count = 0;

    // Check every cell
    for(int i=0; i<BOARD_SIZE; i++)
    {
        for (int j=0; j<BOARD_SIZE; j++)
        {
            // If cell has not been solved
            if (p_board->data[i][j].value == 0)
            {
                printf("Checking cell [%d][%d]\n", i, j);
                int hidden_num = find_hidden_single_values(p_board, &p_board->data[i][j], hiddens_found_list);

                if (hidden_num != 0)
                {
                    HiddenSingle found_cell;
                    found_cell.p_cell = &p_board->data[i][j];
                    found_cell.value = hidden_num;

                    hiddens_found_list[count] = &found_cell;

                    printf("Found a hidden single with value %d\n at cell [%d][%d]\n", found_cell.value, i, j);

                    count++;
                }
            }
        }
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

void show_possible_hs(SudokuBoard *p_board)
{
    for(int i=0; i<BOARD_SIZE; i++)
    {
        for(int j=0; j<BOARD_SIZE; j++)
        {
            Cell *cell = &p_board->data[i][j];
            if(cell->value)
            {
                continue;
            }

            find_hidden_single(cell, p_board->p_rows[i]);
            find_hidden_single(cell, p_board->p_cols[j]);
            find_hidden_single(cell, p_board->p_boxes[(i / 3) * 3 + j / 3]);
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
