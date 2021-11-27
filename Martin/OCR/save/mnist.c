#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "include/mnist_file.h"
#include "include/neural_network.h"

#define EPOCH 1000
//#define EPOCH 50
#define BATCH_SIZE 100

const char *train_images_file = "data/train-images-idx3-ubyte";
const char *train_labels_file = "data/train-labels-idx1-ubyte";
const char *test_images_file = "data/t10k-images-idx3-ubyte";
const char *test_labels_file = "data/t10k-labels-idx1-ubyte";

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

int main(int argc, char *argv[])
{
    mnist_dataset_t *train_dataset, *test_dataset;
    mnist_dataset_t batch;
    neural_network_t network;
    float loss, accuracy;
    train_dataset = mnist_get_dataset(train_images_file, train_labels_file);
    test_dataset = mnist_get_dataset(test_images_file, test_labels_file);
    neural_network_random_weights(&network);
    int batches = train_dataset->size / BATCH_SIZE;
    //TRAIN
    for (int i = 0; i < EPOCH; i++) {
        mnist_batch(train_dataset, &batch, 100, i % batches);
        loss = neural_network_training_step(&batch, &network, 0.5);
        accuracy = calculate_accuracy(test_dataset, &network);
        printf("Epoch %04d\t Accuracy: %.3f\n", i, accuracy);
    }
    //PREDICT
    int predict;
    float activations[MNIST_LABELS], max_activation;
    hypothesis(&test_dataset->images[0], &network, activations);
    for (int j = 0, predict = 0, max_activation = activations[0]; j < MNIST_LABELS; j++) {
            if (max_activation < activations[j]) {
                max_activation = activations[j];
                predict = j;
            }
        }
        printf("%i", predict);
    //FINISH
    mnist_free_dataset(train_dataset);
    mnist_free_dataset(test_dataset);
    return 0;
}
