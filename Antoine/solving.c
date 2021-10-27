#include <stdio.h>
#include <stdlib.h>
 
// N is the size of the matrix
#define N 9
 
// isSafe will check if it is allowed to put the num
// at the row,col or 3x3 matrix in param
int isSafe(int grid[N][N], int row,
                       int col, int num)
{
     
    // Check if its safe to put the num on this row
    // return 0 if not

    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num)
            return 0;
 
    // Check if its safe to put the num on thos col
    // return 0 if not

    for (int x = 0; x <= 8; x++)
        if (grid[x][col] == num)
            return 0;
 
    // Check if its safe to put the num on this 3x3 matrix
    // return 0 if not
    int WhichRow = row - row % 3;
    int WhichCol = col - col % 3;
   
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + WhichRow][j +
                          WhichCol] == num)
                return 0;
    //return 1 if we can put the num at the case 
    return 1;
}

//solveSudoku will seek in the grid given, the 
//different cases that are empty and will try to assign
//them to a number by respecting the isSafe fucntion
//for each numbers.
int solveSudoku(int grid[N][N], int row, int col)
{
    // Check if we have reached the 8th row and the 8th
    // column
    if (row == N - 1 && col == N)
        return 1;
 
    //  Check if we are at the end of the row to go 
    //  to the other one
    if (col == N)
    {
        row++;
        col = 0;
    }
    // Check if the grid[row][col] is empty, if yes,
    // then we move to the next case
    if (grid[row][col] > 0)
        return solveSudoku(grid, row, col + 1);
 
    for (int num = 1; num <= N; num++)
    { 
	// Check if its safe to put the num in the for
	// in the given grid[row][col]
        if (isSafe(grid, row, col, num)==1)
        {
	    // If isSafe return 1, its safe to put the num 
	    // at this grid[row][col] place, so we put it
            grid[row][col] = num;
           
            //  Checking for the next possibility with the
            //  next column
            if (solveSudoku(grid, row, col + 1)==1)
                return 1;
        }
        grid[row][col] = 0;
    }
    return 0;
}

