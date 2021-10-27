#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#include "solving.h"

//function that is printing an arr matrix of N*N size
//in a .txt file called grid_00.result.

/*
void print_in_txt(int arr[N][N])
{
	FILE *fptr;
	fptr = fopen("grid_00.result","w+"); //opening the file
	for(int i = 0; i < N; i++)
	{
		//the first loop is to print in the file the first
        //3 numbers of the row.

        for(int j = 0; j < (N/3); j++)
		{
			fprintf(fptr, "%d",arr[i][j]);
		}

		fputs(" ", fptr); // there is a space between bloc of 3 numbers

        //the second loop is to print in the file the 4-6 
        //numbers of the row.
        
		for(int j = 3; j < (N/3)*2; j++)
		{
			fprintf(fptr,"%d", arr[i][j]);
		}

		fputs(" ", fptr); //once again, another space

        //the last loop to print in the FILE
        //the 3 last numbers of the row
        
		for(int j = 6; j < N; j++)
		{
			fprintf(fptr,"%d" ,arr[i][j]);
		}

        //at the end of these loops, we go back to the line 
        //and if i is equal to 2 or 5, we jump to another line
        //to be conform with the .txt representation of the solver
        //and we do it again on the next row.
        
		fputs("\n",fptr);
		if((i == 2) | (i == 5))
		{
			fputs("\n", fptr);
		}
	}
}*/

	 


int main(int argc, char *argv[])
{


    //initializing a 9x9 matrix full of 0
   /*int e[9][9]={{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                  { 0, 0, 0, 0, 0, 0, 0, 0, 0 }};*/

     
    int e[9][9] = {0};

    
    FILE *fptr;
    
    if(argc != 2){
        errx(1,"Need ./solver with the Name of the Grid");
    }

    int len = strlen(argv[1]);
    char* FileName = NULL;
    FileName = malloc((len+7)*sizeof(int));
    if(FileName == NULL){
        errx(2,"malloc() doesn't work");
    }
    FileName = argv[1];


    //if the file doesn't exist, then we return an error to the user.

    if((fptr = fopen(FileName,"r")) == NULL)
    {
	    errx(1,"No such file given called in parameter is in the repository");
    }
    char c;
    int i = 0;
    int j = 0;
    int space = 0;
    int line = 0;
    
    //else, while fgetc having a char
    //we are going, in this while to re write the matrix in the .txt file
    //into the e matrix full of 0 that we initialized before.

    while((c = fgetc(fptr)) != EOF)
    {
        //if j > 8, that means that we have reached the end of the row
        //so we go to the next one

   	    if(j > 8)
        {
		    j = 0;
		    i +=1;
	    }

        //there is a switch of the different char we can meet between the
        //read of the file.

        switch(c)
	    {
		    case('.'):
			    e[i][j] = 0;
			    j+=1;
			    break;
		    case(' '):
			    space +=1;
			    break;
		    case(10):
			    line +=1;
			    break;
		    default:
			    e[i][j] = c -48;
			    j+=1;
	    }
    }

    //when we writed all the file in a e matrix, we can call this matrix
    //in our solveSudoku to resolve it and return a new matrix.
    //
    //To finish with, we call this new matrix in our print_in_txt function 
    //that we re write this resolved matrix in a new .txt file
    //called grid_00.result

    if (solveSudoku(e, 0, 0)==1)
    {
        //print_in_txt(e);
        char res[] = ".result";
        strcat(FileName,res);
        
        FILE *fptr2;
	    fptr2 = fopen(FileName,"w+"); //opening the file
	    for(int i = 0; i < N; i++)
	    {
		    //the first loop is to print in the file the first
            //3 numbers of the row.

            for(int j = 0; j < (N/3); j++)
		    {
			    fprintf(fptr2, "%d",e[i][j]);
		    }

		    fputs(" ", fptr2); // there is a space between bloc of 3 numbers

            //the second loop is to print in the file the 4-6 
            //numbers of the row.
        
		    for(int j = 3; j < (N/3)*2; j++)
		    {
			    fprintf(fptr2,"%d", e[i][j]);
		    }

		    fputs(" ", fptr2); //once again, another space

            //the last loop to print in the FILE
            //the 3 last numbers of the row
        
		    for(int j = 6; j < N; j++)
		    {
			    fprintf(fptr2,"%d" ,e[i][j]);
		    }

            //at the end of these loops, we go back to the line 
            //and if i is equal to 2 or 5, we jump to another line
            //to be conform with the .txt representation of the solver
            //and we do it again on the next row.
        
		    fputs("\n",fptr2);
		    if((i == 2) | (i == 5))
		    {
			    fputs("\n", fptr2);
		    }
	    }
    }
    else
        printf("No solution exists");

    return 0;
}
