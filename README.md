# nqueens-openmp
Brute-force, depth-first tree search solver for the n-queens problem.
Uses openmp to parallelize the search.
This is done using tasks, each of which consists of a call to the recursive search function.

## Compilation
The program is compiled with the following command:
  gcc nqueens.c -o nqueens -Wall -fopenmp
where the fopenmp-flag is necessary for the program to run in parallel.

## Running the program
The program takes the dimension of the problem as a command-line argument.
As an example, to find the number of solutions to the 8-queen problem, the following command
can be run after the program has been compiled:
  ./nqueens 8

The number of solutions is then written to stdout.
