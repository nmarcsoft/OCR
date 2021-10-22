#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <tgmath.h>
#include <math.h>
#include <time.h>

double sigmoid(double value){
    	return 1 / (1 + exp(-value));
}

double sigmoid_derivative(double value){
	return value * (1 - value);
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
	int outputLayerNeurons = 1;
	
	//RANDOM GENERATION OF WEIGHT
	//
	//CAUTION WEIGHTS IS AlWAYS THE SAME FOR THE MOMENT
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

	double hidden_layer_output[4][2] = {
		{0,0},
		{0,0},
		{0,0},
		{0,0}
	};

	double output_layer_activation[4][1] = {
		{0},
		{0},
		{0},
		{0}
	};
	

	double hidden_layer_output_t[2][4] = {
		{0,0,0,0},
		{0,0,0,0}
	};
	
	double hlo;//hidden_layer_output
	double ow;//output_weight
	double ob;//output_bias
	
	double predicted_output[4][1] = {
		{0},
		{0},
		{0},
		{0}
	};
	double error[4][1] = {
		{0},
		{0},
		{0},
		{0}
	};

	double d_predicted_output[4][1] = {
		{0},
		{0},
		{0},
		{0}
	};

	double sigmod_predicted_output[4][1] = {
		{0},
		{0},
		{0},
		{0}
	};

	double sigmod_hlo[4][2] = {
		{0,0},
		{0,0},
		{0,0},
		{0,0}
	};


	double error_hidden_layer[4][2] = {
		{0,0},
		{0,0},
		{0,0},
		{0,0}
	};
	double d_hidden_layer[4][2] = {
		{0,0},
		{0,0},
		{0,0},
		{0,0}
	};


	double output_weights_t[1][2] = {
		{0,0}
	};
	
	double ehl00;
	double ehl01;
	double ehl10;
	double ehl11;
	double ehl20;
	double ehl21;
	double ehl30;
	double ehl31;
	//Training algorithm
	for(int i = 0; i < epochs; i++){
		//First step
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
		

		//Second step
		hidden_layer_activation[0][0] = hidden_bias[0][0];
		hidden_layer_activation[0][1] = hidden_bias[0][1];
		//Third step
		for(int x = 0; x < 4; x++){
			for(int j = 0; j < 2; j++){
hidden_layer_output[x][j] = sigmoid(hidden_layer_activation[x][j]); //indent
			}
		}

		//4th step
		int R1 = 4;
		int C2 = 1;
		int R2 = 2;
		for (int x = 0; x < R1; x++) {
        		for (int j = 0; j < C2; j++) {
				output_layer_activation[x][j] = 0;
				for (int k = 0; k < R2; k++) {
					hlo = hidden_layer_output[x][k];
					ow = output_weights[k][j];
output_layer_activation[x][j] += hlo * ow; //Indent
				}
			}
		}

		//5th step
		ob = output_bias[0][0];
		for(int x = 0; x < 4; x++){
			output_layer_activation[x][0] += ob;
		}
		
		//6th step
		for(int x = 0; x < 4; x++){
predicted_output[x][0] = sigmoid(output_layer_activation[x][0]); //indent
		}
		
		//7th step
		for(int x = 0; x < 4; x++){
error[x][0] = expected_output[x] - predicted_output[x][0];//indent
		}

		//8th step
		for(int x = 0; x < 4; x++){
sigmod_predicted_output[x][0] = sigmoid_derivative(predicted_output[x][0]);
		}
		for(int x = 0; x < 4; x++){
d_predicted_output[x][0] = error[x][0] * sigmod_predicted_output[x][0];
		}

		//9th step
		for(int x = 0; x < 2; x++){
			output_weights_t[0][x] = output_weights[x][0];
		}
		for(int x = 0; x < 2; x++){
			for(int j = 0; j < 4; j++){
error_hidden_layer[j][x] = output_weights_t[0][x] * d_predicted_output[j][0];
			}
		}

		//10th step
		for(int x = 0; x < 4; x++){
			for(int j = 0; j < 2; j++){
sigmod_hlo[x][j] = sigmoid_derivative(hidden_layer_output[x][j]);
			}
		}
		for(int x = 0; x < 4; x++){
			for(int j = 0; j < 2; j++){
d_hidden_layer[x][j] = error_hidden_layer[x][j] * sigmod_hlo[x][j];
			}
		}

		//11th step
		for(int x = 0; x < 4; x++){
			for(int j = 0; j < 2; j++){
hidden_layer_output_t[j][x] = hidden_layer_output[x][j];
			}
		}
	}
	    /*printf("[%d,%d]", output[i][0], output[i][1]);*/
    
    	//printf("\n");
	//printf("%f", sigmoid(1));

	return sigmoid(1);
}

