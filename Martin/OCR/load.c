#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "mnist_file.h"
#include "nn.h"


void weights(int i, int j, char *line, neural_network_t *network){
    network->weights[i][j] = strtod(line, NULL);
}

void bias(int i, char *line, neural_network_t *network){
    network->bias[i] = strtod(line, NULL);
}

float calculate_accuracy(mnist_dataset_t * dataset, neural_network_t * network)
{
    float activations[MNIST_LABELS], max_activation;
    int i, j, correct, predict;
    for (i = 0, correct = 0; i < dataset->size; i++) {
        hypothesis(&dataset->images[i], network, activations);
        for (j = 0, predict = 0, max_activation = activations[0]; j < MNIST_LABELS; j++) {
            if (max_activation < activations[j]) {
                max_activation = activations[j];
                predict = j;
            }
        }
        if (predict == dataset->labels[i]) {
            correct++;
        }
    }
    return ((float) correct) / ((float) dataset->size);
}


int main(){
    float accuracy;
    const char *test_images_file = "data/t10k-images-idx3-ubyte";
    const char *test_labels_file = "data/t10k-labels-idx1-ubyte";
    neural_network_t network;
    FILE *fp;
    char *line;
    size_t len = 0;
    ssize_t read;
    fp = fopen("program.txt", "r");
    if(fp == NULL){
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i <10; i++){
        read = getline(&line, &len, fp);
        bias(i, line, &network);
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 784; j++){
            read = getline(&line, &len, fp);
            weights(i, j, line, &network);
        }
    }
    mnist_dataset_t *test_dataset;
    test_dataset = mnist_get_dataset(test_images_file, test_labels_file);
    accuracy = calculate_accuracy(test_dataset, &network);
    printf("Accuracy: %.3f\n", accuracy);
    //PREDICT
    int predict;
    float activations[MNIST_LABELS], max_activations;
    hypothesis_real(/*TODO*/, &network, activations);
    max_activations = activations[0];
        for(int z = 1; z < 10; z++){
            if(max_activations < activations[z]){
                max_activations = activations[z];
                predict = z;
            }
        }

    printf("%i, %i", predict, *(&test_dataset->labels[0]));   
    return 0;
}
