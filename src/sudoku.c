#include "sudoku.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "hidden_singles.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <sudoku string>\n", argv[0]);
        return 1;
    }

    SudokuBoard *board = malloc(sizeof(SudokuBoard));
    init_sudoku(board);
    load_sudoku(board, argv[1]);
    print_board(board);

    int loop_count = 0;
    Cell **p_solved_cells = board->solved_cells;
    int solved_counter = board->solved_counter;
    while (board->solved_counter < BOARD_SIZE * BOARD_SIZE)
    {
        solved_counter = check_solved_cells(board, &p_solved_cells);
        printf("check_solved_cells %d\n", solved_counter);
        
        int count = hidden_singles(board);
        printf("Found %d hidden singles\n.", count);

        // solved_counter = hidden_singles(board);
        // if (solved_counter)
        // {
        //     printf("hidden_singles %d\n", solved_counter);
        //     continue;
        // }
        print_board(board);
        loop_count++;
    }
    printf("%d\n", loop_count);

    // clean up
    free_sudoku(board);
    free(board);
    return 0;
}