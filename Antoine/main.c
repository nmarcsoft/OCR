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


/*void print_in_txt(int arr[N][N])
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
}*/

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
   int e[9][9]={{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 }};

    
    FILE *fptr;
    if((fptr = 
fopen("/home/antoll/Desktop/EPITA/PROJET/OCR/Antoine/grid_00","r")) 
		    == NULL){
	    printf("No such file given called grid_00 is in the repository");
	    exit(1);
    }
    char c;
    int i = 0;
    int j = 0;
    int space = 0;
    int ligne = 0;
    while((c = fgetc(fptr)) != EOF)
    {
   	    if(j > 8)
        {
		    j = 0;
		    i +=1;
	    }
        /*
	if(c == '.'){
		e[i][j] = 0;
		j+=1;
	}
	if(c == ' '){
		space +=1;
	}
	else{
		e[i][j] = c-48;
		j+=1;
	}*/
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
			    ligne +=1;
			    break;
		    default:
			    e[i][j] = c -48;
			    j+=1;
	    }
    }
    //print(e);
    if (solveSudoku(e, 0, 0)==1)
        print_in_txt_v2(e);
    else
        printf("No solution exists");

    return 0;
}
