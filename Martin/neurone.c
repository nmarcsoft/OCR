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
	int expected_output[4] = {0,1,1,0};
	int inputLayersNeurons = 2;
	int hiddenLayerNeurons = 2;
	int outpuyLayerNeurons = 1;

	int hiddeneye[2][2] = {
		{/*aleatoire 01,01*/},
		{/*aleatoire 01,01*/}
	}


	    /*printf("[%d,%d]", output[i][0], output[i][1]);*/
    
    	printf("\n");
	printf("%f", sigmoid(1));

return sigmoid(1);
}

