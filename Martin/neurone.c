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
	return 0;
}

