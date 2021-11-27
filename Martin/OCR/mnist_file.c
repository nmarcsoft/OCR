#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "include/mnist_file.h"


uint32_t to_uint32(uint32_t in)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return (
        ((in & 0xFF000000) >> 24) |
        ((in & 0x00FF0000) >>  8) |
        ((in & 0x0000FF00) <<  8) |
        ((in & 0x000000FF) << 24)
    );
#else
    return in;
#endif
}

mnist_image_t *get_images(const char *path, uint32_t *nbimages)
{
    FILE *file;
    mnist_image_file_header_t header;
    mnist_image_t * images;
    file = fopen(path, "rb"); 
    if (1 != fread(&header, sizeof(mnist_image_file_header_t), 1, file)){
        printf("Can't read the image");
        fclose(file);
        return NULL;
    }
    header.magic_number = to_uint32(header.magic_number);
    header.number_of_images = to_uint32(header.number_of_images);
    header.number_of_rows = to_uint32(header.number_of_rows);
    header.number_of_columns = to_uint32(header.number_of_columns);
    *nbimages = header.number_of_images;
    images = malloc(*nbimages * sizeof(mnist_image_t));
    if(images == NULL){
        printf("Can't alloc");
        fclose(file);
        return NULL;
    }
    if (*nbimages != fread(images, sizeof(mnist_image_t), *nbimages, file)) {
        printf("Can't read images");
        free(images);
        fclose(file);
        return NULL;
    }
    fclose(file);
    return images;
}

uint8_t *get_labels(const char *path, uint32_t *nblabels)
{
    FILE *file;
    uint8_t *labels;
    mnist_label_file_header_t header;
    file = fopen(path, "rb");
    if(file == NULL){
        printf("Can't open the file");
        return NULL;
    }
    if(1 != fread(&header, sizeof(mnist_label_file_header_t), 1, file)){
        printf("Can not read the label");
        fclose(file);
        return NULL;
    }
    header.magic_number = to_uint32(header.magic_number);
    header.number_of_labels = to_uint32(header.number_of_labels);
    if(label_base != header.magic_number){
        printf("Invalid header");
        return NULL;
    } 
    *nblabels = header.number_of_labels;
    labels = malloc(*nblabels * sizeof(uint8_t));
    if(labels == NULL){
        printf("Can't alloc");
    }
    if(*nblabels != fread(labels, 1, *nblabels, file)){
        printf("can't read labels");
        free(labels);
        fclose(file);
        return NULL;
    }
    fclose(file);
    return labels;
}

mnist_dataset_t *mnist_get_dataset(const char *image_path ,const char *label_path)
{
    mnist_dataset_t *dataset;
    uint32_t nbimages, nblabels;
    dataset = calloc(1, sizeof(mnist_dataset_t));
    if(dataset == NULL){
        return NULL;
    }
    dataset->images = get_images(image_path, &nbimages);
    if(dataset->images == NULL){
        mnist_free_dataset(dataset);
        return NULL;
    }
    dataset->labels = get_labels(label_path, &nblabels); 
    if(dataset->labels == NULL){
        mnist_free_dataset(dataset);
        return NULL;
    }
    dataset->size = nbimages;
    return dataset;
}

void mnist_free_dataset(mnist_dataset_t *dataset)
{
    free(dataset->images);
    free(dataset->labels);
    free(dataset);
}

int mnist_batch(mnist_dataset_t *dataset, mnist_dataset_t *batch, int size, int number)
{
    int start_offset = size * number;
    if(start_offset >= dataset->size){
        return 0;
    }
    batch->images = &dataset->images[start_offset];
    batch->labels = &dataset->labels[start_offset];
    batch->size = size;
    if(start_offset + batch->size > dataset->size){
        batch->size = dataset->size - start_offset;
    }
    return 1;
}
