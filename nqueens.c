#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>
#include "nqueens.h"

bool is_empty_square(int row, int col, unsigned char **board)
{
    return board[row][col] == 0;
}

bool orthogonal_search(int dim, int piece_row, int piece_col, unsigned char **board)
{
    for (int col = 0; col < dim; col++)
    {
        if (board[piece_row][col] != 0)
        {
            return false;
        }
    }

    for (int row = 0; row < dim; row++)
    {
        if (board[row][piece_col] != 0)
        {
            return false;
        }
    }

    return true;
}

bool diagonal_search(int dim, int piece_row, int piece_col, unsigned char **board)
{
    // Search up-left
    int temp_row = piece_row + 1;
    int temp_col = piece_col - 1;
    while (temp_row < dim && temp_col >= 0)
    {
        if (board[temp_row][temp_col] != 0)
        {
            return false;
        }

        temp_row++;
        temp_col--;
    }

    // Search up-right
    temp_row = piece_row + 1;
    temp_col = piece_col + 1;
    while (temp_row < dim && temp_col < dim)
    {
        if (board[temp_row][temp_col] != 0)
        {
            return false;
        }
        temp_row++;
        temp_col++;
    }

    // Search down-left
    temp_row = piece_row - 1;
    temp_col = piece_col - 1;
    while (temp_row >= 0 && temp_col >= 0)
    {
        if (board[temp_row][temp_col] != 0)
        {
            return false;
        }
        temp_row--;
        temp_col--;
    }

    // Search down-right
    temp_row = piece_row - 1;
    temp_col = piece_col + 1;
    while (temp_row >= 0 && temp_col < dim)
    {
        if (board[temp_row][temp_col] != 0)
        {
            return false;
        }
        temp_row--;
        temp_col++;
    }

    return true;
}

void place_piece(int row, int col, unsigned char **board)
{
    board[row][col] = 1;
}

void remove_piece(int row, int col, unsigned char **board)
{
    board[row][col] = 0;
}

unsigned char **create_board(int dim)
{
    unsigned char **board = malloc(dim * sizeof(*board));
    for (int i = 0; i < dim; i++)
    {
        board[i] = calloc(dim, sizeof(*board[i]));
    }

    return board;
}

bool is_valid_pos(int row, int col, int dim, unsigned char **board)
{
    bool safe_orthogonal = orthogonal_search(dim, row, col, board);
    if (safe_orthogonal) // Don't waste time searching if position already known as invalid
    {
        bool safe_diagonal = diagonal_search(dim, row, col, board);
        return safe_diagonal;
    }

    return false;
}

/*
    


*/
bool recursive_func(int col, int row, unsigned char **board, int dim, int *sol_count)
{
    if (col == dim)
    {
#pragma omp atomic
        *sol_count += 1;
        kill_board(board, dim);
        return true;
    }

    while (row < dim)
    {
        if (is_valid_pos(row, col, dim, board))
        {
            unsigned char **board_copy = copy_board(board, dim);
            place_piece(row, col, board_copy);
#pragma omp task shared(dim)
            if (!recursive_func(col + 1, 0, board_copy, dim, sol_count))
            {

                kill_board(board_copy, dim);
            }
        }
        row++;
    }
    return false;
}

/*
    Prints the pieces placed on the given board.
    Queens are represented by a 'q', vacant positions
    by an empty space.
    @board: the board to print
    @dim: the dimension of the given board (dim x dim)

*/
void print_board(unsigned char **board, int dim)
{
    bool found_nonzero = false;
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            if (board[row][col] != 0)
            {
                found_nonzero = true;
                printf("|q");
            }
            else
            {
                printf("| ");
            }
        }
        printf("|\n");
    }
    if (!found_nonzero)
    {
        printf("All positions in board are zero!");
    }
}


/*
    Copy the contents of a given board into a separate
    memory location.
    @old_board: The board to copy.
    @dim: The dimension of both the original and copy boards (dim x dim).

*/
unsigned char **copy_board(unsigned char **old_board, int dim)
{
    unsigned char **new_board = malloc(sizeof(*new_board) * dim);
    for (int row = 0; row < dim; row++)
    {
        new_board[row] = malloc(sizeof(*(new_board[row])) * dim);
        memcpy(new_board[row], old_board[row], dim);
    }

    return new_board;
}

/*
    Deallocate all memory used by a given board.
    @board: board of chars to deallocate
    @dim: the dimension of the board (dim x dim) 
*/
void kill_board(unsigned char **board, int dim)
{
    for (int row = 0; row < dim; row++)
    {
        free(board[row]);
    }

    free(board);
}

int main(int argc, char *argv[])
{
    unsigned char **board;
    int dim = atoi(argv[1]);
    board = create_board(dim);
    int sol_count = 0;
#pragma omp parallel
    {
#pragma omp single
        {
            recursive_func(0, 0, board, dim, &sol_count);
        }
    }
    printf("Number of solutions found: %d\n", sol_count);

    return EXIT_SUCCESS;
}