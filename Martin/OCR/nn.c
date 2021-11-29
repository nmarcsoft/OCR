#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mnist_file.h"
#include "nn.h"

#define RAND_FLOAT() (((float) rand()) / ((float) RAND_MAX))

float scale(float x){
    return x / 255.0f;
}

int resize(float x){
    if(x != 0){
        return 1;
    }
    else{
        return 0;
    }
}
void neural_network_random_weights(neural_network_t *network)
{
    for (int i = 0; i < MNIST_LABELS; i++) {
        network->bias[i] = RAND_FLOAT();
        for (int j = 0; j < image_size; j++) {
            network->weights[i][j] = RAND_FLOAT();
        }
    }
}

void neural_network_softmax(float *activations, int length)
{
    int i;
    float sum, max;
    for (i = 1, max = activations[0]; i < length; i++) {
        if (activations[i] > max) {
            max = activations[i];
        }
    }
    for (i = 0, sum = 0; i < length; i++) {
        activations[i] = exp(activations[i] - max);
        sum += activations[i];
    }
    for (i = 0; i < length; i++) {
        activations[i] /= sum;
    }
}

//A FAIRE
void hypothesis_real(int *image, neural_network_t *network, float activations[MNIST_LABELS])
{
    for (int i = 0; i < MNIST_LABELS; i++) {
        activations[i] = network->bias[i];
        for (int j = 0; j < image_size; j++){
            activations[i] += (network->weights[i][j] * *(image + j));
//	    printf("%d ", *(image + j));
//	    if(j % 28 == 0){
//	    	printf("\n");
//		}
        }
    }
  //  printf("\n");
    neural_network_softmax(activations, MNIST_LABELS);
}



void hypothesis(mnist_image_t *image, neural_network_t *network, float activations[MNIST_LABELS])
{
    for (int i = 0; i < MNIST_LABELS; i++) {
        activations[i] = network->bias[i];
        for (int j = 0; j < image_size; j++) {
            activations[i] += (network->weights[i][j] * resize(scale(image->pixels[j])));
//	    printf("%d ", resize(scale(image->pixels[j])));
//	    if(j % 28 == 0){
//	    	printf("\n");
//		}
        }
    }
  //  printf("\n");
    //printf("\n");
    neural_network_softmax(activations, MNIST_LABELS);
}

float neural_network_gradient_update(mnist_image_t *image, neural_network_t *network, neural_network_gradient_t *gradient, uint8_t label)
{
    float activations[MNIST_LABELS];
    float bias_grad, weights_grad;
    hypothesis(image, network, activations);
    for (int i = 0; i < MNIST_LABELS; i++) {
        bias_grad = (i == label) ? activations[i] - 1 : activations[i];
        for (int j = 0; j < image_size; j++) {
            weights_grad = bias_grad * resize(scale(image->pixels[j]));
            gradient->weights_grad[i][j] += weights_grad;
        }
        gradient->bias_grad[i] += bias_grad;
    }
    return 0.0f - log(activations[label]);
}

float neural_network_training_step(mnist_dataset_t *dataset, neural_network_t *network, float learning_rate)
{
    neural_network_gradient_t gradient;
    float total_loss;
    memset(&gradient, 0, sizeof(neural_network_gradient_t));
    for (int i = 0, total_loss = 0; i < dataset->size; i++) {
        total_loss += neural_network_gradient_update(&dataset->images[i], network, &gradient, dataset->labels[i]);
    }
    for (int i = 0; i < MNIST_LABELS; i++) {
        network->bias[i] -= learning_rate * gradient.bias_grad[i] / ((float) dataset->size);
        for (int j = 0; j < image_size; j++) {
            network->weights[i][j] -= learning_rate * gradient.weights_grad[i][j] / ((float) dataset->size);
        }
    }
    return total_loss;
}
