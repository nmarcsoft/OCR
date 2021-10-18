#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <tgmath.h>
#include <math.h>
#include <time.h>

double sigmoid(double value){
    	return 1 / exp(-value);
}

double grd(){//Generate random value
	double random_value;
	srand ( time ( NULL));
	random_value = (double)rand()/RAND_MAX*2.0;
	return random_value;
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
	
	//RANDOM GENERATION OF WEIGHT
	//
	//ATTENTION WEIGHTS IS AlWAYS THE SAME FOR THE MOMENT
	double w1 = grd();
	double w2 = grd();
	double w3 = grd();
	double w4 = grd();
	printf("%f, %f", w1, w2);
	double hidden_weights[2][2] = {
		{w1,w2},
		{w3,w4}
	};

	double w5 = grd();
	double w6 = grd();
	double output_bias[1][2] = {
		{w5,w6}
	}
	
	double w7 = grd();
	double w8 = grd();
	double output_weights[2][1]{
		{w7},
		{w8}
	}
	
	double w9 = grd();
	double output_bias[1][1]{
		{w9}
	}



	    /*printf("[%d,%d]", output[i][0], output[i][1]);*/
    
    	//printf("\n");
	//printf("%f", sigmoid(1));

	return sigmoid(1);
}

