#ifndef NEURAL_NETWORK_H_
#define NEURAL_NETWORK_H_
#include "mnist_file.h"

typedef struct neural_network_gradient_t_ {
    float bias_grad[MNIST_LABELS];
    float weights_grad[MNIST_LABELS][image_size];
} neural_network_gradient_t;

typedef struct neural_network_t_ {
    float weights[MNIST_LABELS][image_size];
    float bias[MNIST_LABELS];
    } neural_network_t;


float neural_network_training_step(mnist_dataset_t *dataset, neural_network_t *network, float learning_rate);
void hypothesis(mnist_image_t *image, neural_network_t *network, float activations[MNIST_LABELS]);
void neural_network_random_weights(neural_network_t *network);
float neural_network_gradient_update(mnist_image_t *image, neural_network_t *network, neural_network_gradient_t *gradient, uint8_t label);
#endif
