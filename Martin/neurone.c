#include <stdio.h>

int initialize(){
	int output[4][2] = {
		{0,0},
		{0,1},
		{1,0},
		{1,1}
	};
    for(int i=0; i<4;i++){
	    printf("[%d,%d]", output[i][0], output[i][1]);
    }
    float expected_output[4] = {0.0, 1.0, 1.0, 0.0};
    for(int i=0; i<4;i++){
        printf("[%.6f]", expected_output[i]);
    }
    int bias[][]
return 0;
}

