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
	int inputs[4][2] = {
		{0,0},
		{0,1},
		{1,0},
		{1,1}
	};
	int expected_output[4] = {0,1,1,0};

	int epochs = 100000;
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
	
	
	double hidden_layer_output_t_dot[2][1] = {
		{0},
		{0}
	};

	double hidden_layer_output_t[2][4] = {
		{0,0,0,0},
		{0,0,0,0}
	};

	double d_predicted_output_sum[1][1] = {
		{0}
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

	double inputs_t[2][4]={
		{0,0,1,1},
		{0,1,0,1}
	};

	double inputs_t_dot[2][2] = {
		{0,0},
		{0,0}
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

	double d_hidden_layer_sum[1][2] = {
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

	int R1;
	int C2;
	int R2;
	//Training algorithm
	for(int i = 0; i < epochs; i++){
		printf("Epoch : %d \n", i);
printf("[%lf,%lf,%lf,%lf]", predicted_output[0][0], predicted_output[0][1],
			    predicted_output[0][2],
			    predicted_output[0][3]);
  
		//First step
		R1 = 4;
		C2 = 2;
		R2 = 2;
		for (int x = 0; x < R1; x++) {
        		for (int j = 0; j < C2; j++) {
				hidden_layer_activation[x][j] = 0;
				for (int k = 0; k < R2; k++) {
					hlo = inputs[x][k];
					ow = hidden_weights[k][j];
hidden_layer_activation[x][j] += hlo * ow;
				}
			}
		}

		//Second step
		hidden_layer_activation[0][0] += hidden_bias[0][0];
		hidden_layer_activation[0][1] += hidden_bias[0][1];
		//Third step
		for(int x = 0; x < 4; x++){
			for(int j = 0; j < 2; j++){
hidden_layer_output[x][j] = sigmoid(hidden_layer_activation[x][j]); //indent
			}
		}

		//4th step
		R1 = 4;
		C2 = 1;
		R2 = 2;
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
		R1 = 4;
		C2 = 2;
		R2 = 1;
		for (int x = 0; x < R1; x++) {
        		for (int j = 0; j < C2; j++) {
				error_hidden_layer[x][j] = 0;
				for (int k = 0; k < R2; k++) {
					hlo = d_predicted_output[x][k];
					ow = output_weights_t[k][j];
error_hidden_layer[x][j] += hlo * ow; //Indent
				}
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
		R1 = 2;
		C2 = 1;
		R2 = 4;
		for (int x = 0; x < R1; x++) {
        		for (int j = 0; j < C2; j++) {
				hidden_layer_output_t_dot[x][j] = 0;
				for (int k = 0; k < R2; k++) {
					hlo = hidden_layer_output_t[x][k];
					ow = d_predicted_output[k][j];
hidden_layer_output_t_dot[x][j] += hlo * ow; //Indent
				}
			}
		}
		for(int x = 0; x < 2; x++){
output_weights[x][0] += hidden_layer_output_t_dot[x][0] * lr;
		}

		//12th step
		for(int x = 0; x < 4; x++){
d_predicted_output_sum[0][0] += d_predicted_output[x][0];
		}
		d_predicted_output_sum[0][0] *= lr;
		output_bias[0][0] += d_predicted_output_sum[0][0];
		
		//13th step
		R1 = 2;
		C2 = 2;
		R2 = 4;
		for (int x = 0; x < R1; x++) {
        		for (int j = 0; j < C2; j++) {
				inputs_t_dot[x][j] = 0;
				for (int k = 0; k < R2; k++) {
					hlo = inputs_t[x][k];
					ow = d_hidden_layer[k][j];
inputs_t_dot[x][j] += hlo * ow; //Indent
				}
			}
		}
		for(int x = 0; x < 2; x++){
			for(int j = 0; j < 2; j++){
				inputs_t_dot[x][j] *= lr;
			}
		}
		for(int x = 0; x < 2; x++){
			for(int j = 0; j < 2; j++){
				hidden_weights[x][j] += inputs_t_dot[x][j];
			}
		}

		//14th step
		for(int x = 0; x < 4; x++){
			for(int j = 0; j < 2; j++){
d_hidden_layer_sum[0][j] += d_hidden_layer[x][j];
			}
		}
		for(int x = 0; x < 2; x++){
			d_hidden_layer_sum[0][x] *= lr;
		}
		for(int x = 0; x < 2; x++){
			hidden_bias[0][x] += d_hidden_layer_sum[0][x];
		}


	}

		
		
printf("[%lf,%lf,%lf,%lf]", predicted_output[0][0], predicted_output[0][1],
			    predicted_output[0][2],
			    predicted_output[0][3]);
    
    	//printf("\n");
	//printf("%f", sigmoid(1));

	return sigmoid(1);
}

