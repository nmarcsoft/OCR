#include <stdlib.h>
#include <stdio.h>
#include "backprop.h"

void print_test2(int* a, size_t height, size_t width)
{
  printf("{");
  for (size_t i = 0; i < height; i++)
  {
    for (size_t j = 0; j < width; j++)
	  {
	    int test = *(a + i*width + j);
	    printf("%i ", test);
	  }
    if (i!= height-1)
      printf("\n");
  }
  printf("}");
  printf("\n\n");
}

//pour découper en mots/lettres
int * vert_histogram(int * binary, size_t height, size_t width)
{
  int *vert_hist = malloc(width * sizeof(int));
  for (size_t x = 0; x < width; x++)
    *(vert_hist+x) = 0;
  for(size_t j = 0; j < width; j++)
  {
    for(size_t i = 0; i < height; i++)
	  {
	    if (*(binary + i*width + j) == 0)
	      ++*(vert_hist+j);
	  }
  }
  return vert_hist;
}
//return will have height = 1 and width = binary width
//works, tested


//pour découper en lignes
int * horizontal_histogram(int * binary, size_t height, size_t width)
{
  int *horizontal_hist = malloc(height * sizeof(int));
  for (size_t x = 0; x < height; x++)
    *(horizontal_hist+x) = 0;
  for(size_t i = 0; i < height; i++)
  {
    for(size_t j = 0; j < width; j++)
	  {
	    if (*(binary + i*width + j) == 0)
	      ++*(horizontal_hist+i);
	  }
  }
  return horizontal_hist;
}
//return will have width = 1 and height = binary height
//works, tested

/*
//testing with files
void test_file(char * t)
{
  FILE *text_file_pointer;
  text_file_pointer = fopen("all_letters.txt", "a");
  fputs(t, text_file_pointer);
  fclose(text_file_pointer);
}
*/
/*void center_letter_height(int* letter, size_t height, size_t width)
{
  int * histogram = horizontal_histogram(letter, height, width);
  size_t space_1 = 0;
  size_t space_2 = height - 1;
  while(*(histogram+space_1) == 0){
    space_1++;
  }
  while(*(histogram+space_2)==0){
    space_2--;
  }
  if (space_2 > space_1){
    size_t ecart = space_2 - space_1;
    for (size_t i = 0; i < height; i++)
    {
      for (size_t j = 0; i < width; j++)
      {
        if (width - j > ecart)
        {
          (*(letter + i*width + (width-1-j))) = (*(letter + i*width + (width-1-j-ecart)));
        }
        else
        {
          (*(letter + i*width + (width-1-j))) = 1;
        }
      }
    }
  }
}*/


/*void center_letter_width(int* letter, size_t height, size_t width)
{
  int * histogram = vert_histogram(letter, height, width);
  size_t white = 0;
  print_test2(histogram, 1, 16);
  while (*(histogram+white) == 0)
  {
    white++;
  }
  size_t black = 0;
  while ((*(histogram+black+white) > 0) & (white+black < width))
  {
    black++;  
  }
  int bool = 1;
  size_t ecart = (width - black)/2;
  if (white > ecart)
  {
    ecart = white - ecart;
    bool = 0;
  }
  else
  {
    ecart -= white;
  }
  if (ecart == 0)
  {
    return;
  }
  if (bool)
  {
    for (size_t j = 0; j < width; j++)
    {
      for (size_t i = 0; i < height; i++)
      {
        if (width - j > ecart)
        {
          (*(letter + i*width + (width-1-j))) = (*(letter + i*width + (width-1-j-ecart)));
        }
        else
        {
          (*(letter + i*width + (width-1-j))) = 1;
        }
      }
    }
  }
  else
  {
    for (size_t j = 0; j < width; j++)
    {
      for (size_t i = 0; i < height; i++)
      {
        if (j < width - ecart - 1)
        {
          (*(letter + i*width + j)) = (*(letter + i*width + j+ecart));
        }
        else
        {
          (*(letter + i*width + j)) = 1;
        }
      }
    }
  }
}*/


//fonction qui analyse contenu d'une ligne:
/*int analyse_line(int * line, size_t height, size_t width, FILE * file)
{
  int * histogram = vert_histogram(line, height, width);
  size_t i = 0;
  int count = 0;
  int space_count = 0;
  int * letter = malloc(16 * 16 * sizeof(int));
  while(i < width)
  {
    if (*(histogram+i) > 0)
    {
      while (space_count > 16)
      {
        //adds a space
        fputs(" ", file);
        space_count -= 16;
        count++;
      }
      space_count = 0;
      for(size_t x = 0; x < 16; x++)
      {
        for(size_t y = 0; y < 16; y++)
        {
          if(i+y < width)
            *(letter + x*16 + y) = *(line + x*width + i + y);
        }
      }
      //center_letter_width(letter, 16, 16);
      char *p;
      char l = give_letter(letter);//fonction de babar et erwann avec parametre (letter)
      p = &l;
      count++;
      //ajouter le char l au file
      fputs(p, file);
      i+=16;
    }
    else
    {
      i++;
      space_count++;
    }
  }
  //add end of line (\n) to file
  fputs("\n", file);
  count++;
  free(histogram);
  free(letter);
  return count;
}
*/

//2nd version of analyse line
int analyse_line_better(int * line, size_t height, size_t width, FILE * file)
{
  int * histogram = vert_histogram(line, height, width);
  size_t i = 0;
  int count = 0;
  int space_count = 0;
  //print_test2(histogram, 1, 533);
  int * letter = malloc(16 * 16 * sizeof(int));
  while(i < width)
  {
    if (*(histogram+i) > 0)
    {
      size_t width_letter = 1;
      while (space_count > 6)
      {
        //adds a space
        fputs(" ", file);
        space_count -= 6;
        count++;
      }
      space_count = 0;
      while(*(histogram + i + width_letter) > 0){
        width_letter++;
      }
      for(size_t x = 0; x < 16; x++)
      {
        for(size_t y = 0; y < 16; y++)
        {
          if(i+y < width){
            if(y<width_letter)
              *(letter + x*16 + y) = *(line + x*width + i + y);
            else
            {
              *(letter + x*16 + y) = 1;
            }
          }
        }
      }
      //center_letter_width(letter, 16, 16);
      char *p;
      char l = give_letter(letter);//fonction de babar et erwann avec parametre (letter)
      p = &l;
      count++;
      //ajouter le char l au file
      fputs(p, file);
      i+=width_letter;
    }
    else
    {
      i++;
      space_count++;
    }
  }
  //add end of line (\n) to file
  fputs("\n", file);
  free(histogram);
  free(letter);
  return count;
}



//fonction qui découpe binary en lignes:
int find_line(int * binary, size_t height, size_t width)
{
  int * histogram = horizontal_histogram(binary, height, width);
  size_t i = 0;
  int space_count = 0;
  int count = 0;
  int * line = malloc(16 * width * sizeof(int));
  FILE *text_file_pointer;
  text_file_pointer = fopen("output_result.txt", "w"); //write mode, starts with empty file
  while(i < height)
  {
    if (*(histogram+i) > 0)
    {
      if (space_count > 16)
      {
        //adds new paragraph to text file
        fputs("\n", text_file_pointer);
        count++;
      }
      for(size_t x = 0; x < 16; x++)
        {
          for(size_t y = 0; y < width; y++)
          {
            if (x+i < height)
              *(line + x*width + y) = *(binary + (x+i)*width + y);
          }
        }
      i+=16;
      space_count = 0;
      count += analyse_line_better(line, 16, width, text_file_pointer);
    }
    else
    {
      i++;
      space_count++;
    }
  }
  fclose(text_file_pointer);
  free(line);
  free(histogram);
  printf("============ FINI ============\n");
  return count;
}
