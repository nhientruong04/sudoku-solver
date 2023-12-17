#include <stdio.h>
#include <stdbool.h>

#define BOARD_SIZE 9

struct Cell_impl
{
    int row_index;
    int col_index;
    int box_index;
    int num_candidates;
    int candidates[BOARD_SIZE];
    int value; // solved or 0
    bool fixed;
};

typedef struct Cell_impl Cell;

int main() {
    
    return 0;
}