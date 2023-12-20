#include "naked_triples.h"
#include <stdio.h>

int naked_triples(SudokuBoard *p_board)
{
    int triples_counter = 0;
    int a;
    for (int i=0; i<BOARD_SIZE; i++)
    {
        for (int j=0; j<BOARD_SIZE; j++)
        {
            Cell *cell = &p_board->data[i][j];
            if (cell->row_index==8 && cell->col_index==0)
            {
                a = 1+1;
            }

            if (cell->row_index==8 && cell->col_index==3)
            {
                a = 1+1;
            }

            if (cell->row_index==8 && cell->col_index==7)
            {
                a = 1+1;
            }
        }
    }

    // save all cells that are already paired
    int arraysize = BOARD_SIZE*BOARD_SIZE - p_board->solved_counter;
    PossibleMembers **possible_member_cells = malloc(arraysize * sizeof(PossibleMembers*));

    int triples_members_num = find_nakedTriples_members(p_board, possible_member_cells);

    // save all pairs
    NakedTriples **naked_triples_list = malloc((triples_members_num) / 3 * sizeof(NakedTriples *));
    // Cell **registered_cells = malloc(triples_members_num * 3 * sizeof(Cell*));

    // target the conventional case
    for (int i=0; i<triples_members_num; i++)
    {
        PossibleMembers *member1 = possible_member_cells[i];
        if (member1->value_size == 2)
        {
            continue;
        }

        NakedTriples *triple = malloc(sizeof(NakedTriples));
        triple->cells_triples[0] = member1->cell;
        
        triple->value_triples[0] = member1->value_triples[0];
        triple->value_triples[1] = member1->value_triples[1];
        triple->value_triples[2] = member1->value_triples[2];

        triple->positions[0] = member1->cell->row_index;
        triple->positions[1] = member1->cell->col_index;
        triple->positions[2] = member1->cell->box_index;

        triple->match_positions[0] = 1;
        triple->match_positions[1] = 1;
        triple->match_positions[2] = 1;

        bool triple_valid = find_convention_triples(triple, possible_member_cells, naked_triples_list, triples_members_num);
    
        if (!triple_valid)
        {
            free(triple);
            continue;
        }

        member1->used = true;
        naked_triples_list[triples_counter++] = triple;
    }

    // target the special case
    for (int i=0; i<triples_members_num; i++)
    {
        PossibleMembers *member1 = possible_member_cells[i];
        if (member1->value_size == 3 || member1->used)
        {
            continue;
        }

        NakedTriples *triple = malloc(sizeof(NakedTriples));
        triple->cells_triples[0] = member1->cell;
        
        triple->value_triples[0] = member1->value_triples[0];
        triple->value_triples[1] = member1->value_triples[1];

        triple->positions[0] = member1->cell->row_index;
        triple->positions[1] = member1->cell->col_index;
        triple->positions[2] = member1->cell->box_index;

        triple->match_positions[0] = 1;
        triple->match_positions[1] = 1;
        triple->match_positions[2] = 1;

        bool triple_valid = find_special_triples(triple, possible_member_cells, naked_triples_list, triples_members_num);
    
        if (!triple_valid)
        {
            free(triple);
            continue;
        }

        member1->used = true;
        naked_triples_list[triples_counter++] = triple;
    }

    // apply effect
    for (int i=0; i<triples_counter; i++)
    {
        apply_naked_triples_effect(p_board, naked_triples_list[i]);
    }
    
    for (int i=0; i<triples_counter; i++)
    {
        printf("Group %d: \n", i+1);
        for (int j=0; j<3; j++)
        {
            printf("Found cell [%d][%d]\n", naked_triples_list[i]->cells_triples[j]->row_index, naked_triples_list[i]->cells_triples[j]->col_index);
        }
    }

    // clean up
    for (int i=0; i<triples_members_num; i++)
    {
        free(possible_member_cells[i]->value_triples);
        free(possible_member_cells[i]);
    }
    free(possible_member_cells);

    for (int i=0; i<triples_counter; i++)
    {
        free(naked_triples_list[i]);
    }
    free(naked_triples_list);

    return triples_counter;
}

int find_nakedTriples_members(SudokuBoard *p_board, PossibleMembers **possible_member_cells)
{
    int members_count = 0;

    for (int i=0; i<BOARD_SIZE; i++)
    {
        for (int j=0; j<BOARD_SIZE; j++)
        {
            Cell *cell = &p_board->data[i][j];
            int num_candidates = cell->num_candidates;
            if (num_candidates == 2 || num_candidates == 3)
            {
                PossibleMembers *member = malloc(sizeof(PossibleMembers));
                member->cell = cell;
                member->value_triples = malloc(num_candidates * sizeof(int));
                member->value_size = num_candidates;
                member->used = false;

                get_value(member->cell, member->value_triples, num_candidates);

                possible_member_cells[members_count++] = member;
            }
        }
    }

    return members_count;
}

void get_value(Cell *cell, int *value_triples, int num_candidates)
{
    int *candidates = cell->candidates;
    int counter = 0;
    
    for (int i=0; i<BOARD_SIZE; i++)
    {
        if (candidates[i])
        {
            value_triples[counter++] = i;
        }

        if (counter == num_candidates)
        {
            break;
        }
    }
}

bool find_convention_triples(NakedTriples *triple, PossibleMembers **possible_member_cells, NakedTriples **naked_triples_list, int size)
{
    bool ret = false;

    for (int i=0; i<size; i++)
    {
        if (possible_member_cells[i]->used)
        {
            continue;
        }

        if (possible_member_cells[i]->cell == triple->cells_triples[0])
        {
            continue;
        }

        int counter_validity = 0;
        for (int j=0; j<3; j++)
        {
            if (possible_member_cells[i]->cell->candidates[triple->value_triples[j]])
            {
                counter_validity++;
            }
        }

        if (counter_validity != possible_member_cells[i]->cell->num_candidates)
        {
            continue;
        }

        bool house_valid = is_same_house(triple->match_positions, possible_member_cells[i], triple->positions[0], triple->positions[1], triple->positions[2]);

        if (house_valid)
        {
            ret = find_convention_triples_step2(triple, possible_member_cells[i], possible_member_cells, naked_triples_list, size);

            if (ret)
            {
                possible_member_cells[i]->used = true;
                triple->cells_triples[2] = possible_member_cells[i]->cell;
                ret = true;

                return ret;
            } else
            {
                triple->match_positions[0] = 1;
                triple->match_positions[1] = 1;
                triple->match_positions[2] = 1;
                continue;
            }
        }
    }

    return ret;
}

bool find_convention_triples_step2(NakedTriples *triple, PossibleMembers *member2, PossibleMembers **possible_member_cells, NakedTriples **naked_triples_list, int size)
{
    bool ret = false;

    for (int i=0; i<size; i++)
    {
        if (possible_member_cells[i]->used)
        {
            continue;
        }

        if (possible_member_cells[i]->cell == triple->cells_triples[0] || possible_member_cells[i]->cell == member2->cell)
        {
            continue;
        }

        int counter_validity = 0;
        for (int j=0; j<3; j++)
        {
            if (possible_member_cells[i]->cell->candidates[triple->value_triples[j]])
            {
                counter_validity++;
            }
        }

        if (counter_validity != possible_member_cells[i]->cell->num_candidates)
        {
            continue;
        }

        bool house_valid = is_same_house(triple->match_positions, possible_member_cells[i], triple->positions[0], triple->positions[1], triple->positions[2]);

        if (house_valid)
        {
            ret = true;
            possible_member_cells[i]->used = true;
            triple->cells_triples[1] = possible_member_cells[i]->cell;
            break;
        }
    }

    return ret;
}

bool find_special_triples(NakedTriples *triple, PossibleMembers **possible_member_cells, NakedTriples **naked_triples_list, int size)
{
    bool ret = false;

    for (int i=0; i<size; i++)
    {
        if (possible_member_cells[i]->used || possible_member_cells[i]->cell->num_candidates == 3)
        {
            continue;
        }

        if (possible_member_cells[i]->cell == triple->cells_triples[0])
        {
            continue;
        }

        int counter_validity = 0;
        for (int j=0; j<2; j++)
        {
            if (possible_member_cells[i]->cell->candidates[triple->value_triples[j]])
            {
                counter_validity++;
            }
        }

        if (counter_validity != 1)
        {
            continue;
        }

        int odd_candidate;
        for (int j=0; j<2; j++)
        {
            if (!triple->cells_triples[0]->candidates[possible_member_cells[i]->value_triples[j]])
            {
                odd_candidate = possible_member_cells[i]->value_triples[j];
            }
        }

        bool house_valid = is_same_house(triple->match_positions, possible_member_cells[i], triple->positions[0], triple->positions[1], triple->positions[2]);

        if (house_valid)
        {
            ret = find_special_triples_step2(triple, possible_member_cells[i], possible_member_cells, naked_triples_list, size, odd_candidate);

            if (ret)
            {
                possible_member_cells[i]->used = true;
                triple->cells_triples[2] = possible_member_cells[i]->cell;
                triple->value_triples[2] = odd_candidate;
                ret = true;

                return ret;
            } else
            {
                triple->match_positions[0] = 1;
                triple->match_positions[1] = 1;
                triple->match_positions[2] = 1;
                continue;
            }
        }
    }

    return ret;
}

bool find_special_triples_step2(NakedTriples *triple, PossibleMembers *member2, PossibleMembers **possible_member_cells, NakedTriples **naked_triples_list, int size, int odd_candidate)
{
    bool ret = false;

    for (int i=0; i<size; i++)
    {
        if (possible_member_cells[i]->used)
        {
            continue;
        }

        if (possible_member_cells[i]->cell == triple->cells_triples[0] || possible_member_cells[i]->cell == member2->cell)
        {
            continue;
        }

        int counter_validity = 0;
        int draft_candidates[3];
        draft_candidates[0] = triple->value_triples[0];
        draft_candidates[1] = triple->value_triples[1];
        draft_candidates[2] = odd_candidate;

        for (int j=0; j<3; j++)
        {
            if (possible_member_cells[i]->cell->candidates[draft_candidates[j]])
            {
                counter_validity++;
            }
        }

        if (counter_validity != possible_member_cells[i]->cell->num_candidates)
        {
            continue;
        }

        bool house_valid = is_same_house(triple->match_positions, possible_member_cells[i], triple->positions[0], triple->positions[1], triple->positions[2]);

        if (house_valid)
        {
            ret = true;
            possible_member_cells[i]->used = true;
            triple->cells_triples[1] = possible_member_cells[i]->cell;
            break;
        }
    }

    return ret;
}

bool is_same_house(int *match_positions, PossibleMembers *second_cell, int row_index, int col_index, int box_index)
{
    bool ret = false;
    int draft_positions[3];
    for (int i=0; i<3; i++)
    {
        draft_positions[i] = match_positions[i];
    }

    int ori_positions[3] = {row_index, col_index, box_index};
    int compare_positions[3] = {second_cell->cell->row_index, second_cell->cell->col_index, second_cell->cell->box_index};
    
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

void apply_naked_triples_effect(SudokuBoard *p_board, NakedTriples *triple)
{
    // [row, col, box]
    int *match_positions = triple->match_positions;

    for (int i=0; i<3; i++)
    {
        if (match_positions[i] == 0)
        {
            continue;
        }

        if (i == 0)
        {
            unset_candidates_naked_triples(triple, p_board->p_rows[triple->cells_triples[0]->row_index]);
        }

        if (i == 1)
        {
            unset_candidates_naked_triples(triple, p_board->p_cols[triple->cells_triples[0]->col_index]);
        }

        if (i == 2)
        {
            unset_candidates_naked_triples(triple, p_board->p_boxes[triple->cells_triples[0]->box_index]);
        }
    }
}

void unset_candidates_naked_triples(NakedTriples *triple, Cell **group)
{
    int candidate1 = triple->value_triples[0];
    int candidate2 = triple->value_triples[1];
    int candidate3 = triple->value_triples[2];

    for (int i=0; i<BOARD_SIZE; i++)
    {
        Cell *cell = group[i];

        if (cell->num_candidates == 1)
        {
            continue;
        }

        if (cell==triple->cells_triples[0] || cell==triple->cells_triples[1] || cell==triple->cells_triples[2])
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

        if (cell->candidates[candidate3])
        {
            unset_candidate(cell, candidate3 + 1);
        }
    }
}