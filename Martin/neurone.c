#include <stdio.h>
#include <complex.h>
#include <tgmath.h>
#include <math.h>


double exp(double value){


double sigmoid(double value){
    	double result;

    	/* Assign the value we will find the exp of */
    	

    	/* Calculate the exponential of the value */
    	result = exp(value);
	return result;
}
double initialize(){
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
    	printf("\n");
	printf("%f", sigmoid(1));

return sigmoid(1);
}

