#include <stdio.h>
#include <stdlib.h>
#include "solving.h"




void print(int arr[N][N])
{
     for (int i = 0; i < N; i++)
      {
         for (int j = 0; j < N; j++)
            printf("%d ",arr[i][j]);
         printf("\n");
       }
}


void print_in_txt(int arr[N][N])
{
	FILE *fptr;
	fptr = fopen("/home/antoll/Desktop/EPITA/PROJET/OCR/Antoine/grid_00.result","w");
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			//arr[i][j]
			if(j % 3 == 0)
			{
				fputs(" ",fptr);
				fprintf(fptr,"%d",arr[i][j]);
			}
			if(i % 3 == 0)
			{
				fputs(" ",fptr);
				fprintf(fptr,"%d",arr[i][j]);
			}
			else{
			fprintf(fptr,"%d",arr[i][j]);
			}
		}
		fputs("\n", fptr);
	}
}

void print_in_txt_v2(int arr[N][N])
{
	FILE *fptr;
	fptr = fopen("grid_00.result","w");
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < (N/3); j++)
		{
			fprintf(fptr, "%d",arr[i][j]);
		}
		fputs(" ", fptr);
		for(int j = 3; j < (N/3)*2; j++)
		{
			fprintf(fptr,"%d", arr[i][j]);
		}
		fputs(" ", fptr);
		for(int j = 6; j < N; j++)
		{
			fprintf(fptr,"%d" ,arr[i][j]);
		}
		fputs("\n",fptr);
		if((i == 2) | (i == 5))
		{
			fputs("\n", fptr);
		}
	}
}	


	

int main()
{
    int grid[9][9] = { { 0, 0, 0, 0, 0, 4, 5, 8, 0 },
                       { 0, 0, 0, 7, 2, 1, 0, 0, 3 },
                       { 4, 0, 3, 0, 0, 0, 0, 0, 0 },
                       { 2, 1, 0, 0, 6, 7, 0, 0, 4 },
                       { 0, 7, 0, 0, 0, 0, 2, 0, 0 },
                       { 6, 3, 0, 0, 4, 9, 0, 0, 1 },
                       { 3, 0, 6, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 1, 5, 8, 0, 0, 6 },
                       { 0, 0, 0, 0, 0, 6, 9, 5, 0 } };

    if (solveSudoku(grid, 0, 0)==1)
        print_in_txt_v2(grid);
    else
        printf("No solution exists");

    return 0;
}
