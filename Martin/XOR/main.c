#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <err.h>
#include "neurone.h"


int main(int argc, char *argv[]){
    if(argc < 2){
        errx(1, "you must write an argument to run the program");
    }

    char *program = argv[1];

    if(strcmp("00", program) == 0){
        initialize(0);
    }
    else if(strcmp("01", program) == 0){
        initialize(1);
    }
    else if(strcmp("10", program) == 0){
        initialize(2);
    }
    else if(strcmp("11", program) == 0){
        initialize(3);
    }
    else{
        errx(1, "Wrong argument");
    }
	return 0;
}
