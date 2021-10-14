#include <stdio.h>
#include <complex.h>
#include <tgmath.h>
#include <math.h>


double sigmoid(double value){
    	return 1 / exp(-value);
}



double initialize(){
	int output[4][2] = {
		{0,0},
		{0,1},
		{1,0},
		{1,1}
	};
	    /*printf("[%d,%d]", output[i][0], output[i][1]);*/
    
    	printf("\n");
	printf("%f", sigmoid(1));

return sigmoid(1);
}

