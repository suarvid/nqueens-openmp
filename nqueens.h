#ifndef N_QUEENS_H
#define N_QUEENS_H

bool is_empty_square(int row, int col,  unsigned char **board);
bool orthogonal_search(int dim, int piece_row, int piece_col,  unsigned char **board);
bool diagonal_search(int dim, int piece_row, int piece_col,  unsigned char **board);
void place_piece(int row, int col, unsigned char **board);
void remove_piece(int row, int col, unsigned char **board);
unsigned char **create_board(int dim);
bool is_valid_pos(int row, int col, int dim,  unsigned char **board);
bool recursive_func(int col, int row, unsigned char **board, int dim, int *sol_count);
void print_board( unsigned char **board, int dim);
unsigned char **copy_board( unsigned char **old_board, int dim);
void kill_board(unsigned char **board, int dim);
#endif