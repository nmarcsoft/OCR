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
	random_value = (double)rand()/RAND_MAX*2.0-1.0;
	return random_value*random_value;
}


double initialize(){
	
	//Input datasets
	int output[4][2] = {
		{0,0},
		{0,1},
		{1,0},
		{1,1}
	};
	int expected_output[4] = {0,1,1,0};

	int epochs = 10000;
	double lr = 0.1;
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
	double hidden_bias[1][2] = {
		{w5,w6}
	};
	
	double w7 = grd();
	double w8 = grd();
	double output_weights[2][1] = {
		{w7},
		{w8}
	};
	
	double w9 = grd();
	double output_bias[1][1] = {
		{w9}
	};
	
	double hidden_layer_activation[4][2] = {
		{0,0},
		{0,0},
		{0,0},
		{0,0}
	};

	double hw00;
	double hw01;
	double hw10;
	double hw11;
	//Training algorithm
	for(int i = 0; i < epochs; i++){
		hw00 = hidden_weights[0][0];
		hw01 = hidden_weights[0][1];
		hw10 = hidden_weights[1][0];
		hw11 = hidden_weights[1][1];
		hidden_layer_activation[1][0] += hw10;
		hidden_layer_activation[1][1] += hw11;
		hidden_layer_activation[2][0] += hw00;
		hidden_layer_activation[2][1] += hw01;
		hidden_layer_activation[3][0] += hw00 + hw10;
		hidden_layer_activation[3][1] += hw01 + hw11;

	}

	    /*printf("[%d,%d]", output[i][0], output[i][1]);*/
    
    	//printf("\n");
	//printf("%f", sigmoid(1));

	return sigmoid(1);
}

