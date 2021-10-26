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
	/*double random_value;
	srand ( time ( NULL));
	random_value = (double)rand()/RAND_MAX*2.0-1.0;
	return random_value*random_value;
    */
    return ((double)rand()/(double)RAND_MAX); //Generate a double between 0 and 1
}


double initialize(){
	return sigmoid(1);
}

