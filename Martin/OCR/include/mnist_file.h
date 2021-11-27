#ifndef MNIST_FILE_H_
#define MNIST_FILE_H_

#include <stdint.h>

#define image_width 28
#define image_height 28
#define image_size image_width * image_height
#define MNIST_LABELS 10
#define label_base 0x00000801

typedef struct mnist_label_file_header_t_ {
    uint32_t magic_number;
    uint32_t number_of_labels;
} __attribute__((packed)) mnist_label_file_header_t;
typedef struct mnist_image_file_header_t_ {
    uint32_t magic_number;
    uint32_t number_of_images;
    uint32_t number_of_rows;
    uint32_t number_of_columns;
} __attribute__((packed)) mnist_image_file_header_t;

typedef struct mnist_image_t_ {
    uint8_t pixels[image_size];
} __attribute__((packed)) mnist_image_t;

typedef struct mnist_dataset_t_ {
    mnist_image_t *images;
    uint8_t *labels;
    uint32_t size;
} mnist_dataset_t;



mnist_dataset_t *mnist_get_dataset(const char *image_path, const char *label_path);
void mnist_free_dataset(mnist_dataset_t *dataset);
int mnist_batch(mnist_dataset_t * dataset, mnist_dataset_t * batch, int batch_size, int batch_number);

#endif
