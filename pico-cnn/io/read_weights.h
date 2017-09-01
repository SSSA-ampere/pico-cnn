/** 
 * @brief provides functions to read the weights running CNNs
 *
 * @author Konstantin Luebeck (University of Tuebingen, Chair for Embedded Systems)
 */

#ifndef READ_WEIGHTS_H
#define READ_WEIGHTS_H

#include "../parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/** 
 * @brief reads weights (elements of kernels and biasses) from a weight file
 * Find the definition of the file format in doc/weight_file_format.md
 *
 * @param path_to_weight_file file in which the weights are stored
 * @param kernels a 4-dimensional array which stores the kernels:
 * (*kernels)[layer_num][kernel_num][kernel_element]. Memory for it will be
 * allocated inside of the function
 * @param biasses a 3-dimensional array which stores the biasses:
 * (*biasses)[layer_num][bias_num]
 *
 * @return =! 0 means an error occured
 */
int read_weights(const char* path_to_weights_file, float_t**** kernels, float_t*** biasses) {

    FILE *weights;
    weights = fopen(path_to_weights_file, "r");

    if(weights != 0) {

        char buffer[100];

        uint16_t num_layers;

        // read magic number
        fgets(buffer, 100, weights);

        if(strcmp(buffer,"EF\n") != 0) {
            fclose(weights);
            return 1;
        }

        // read name
        fgets(buffer, 100, weights);

        // read number of layers
        fscanf(weights, "%hu\n", &num_layers);

        // allocate memory for layers
        *kernels = (float_t***) malloc(num_layers * sizeof(float_t**));
        *biasses = (float_t**) malloc(num_layers * sizeof(float_t*));

        uint16_t layer;

        // loop over layers
        for(layer = 0; layer < num_layers; layer++) {

            // read layer name
            fgets(buffer, 100, weights);

            uint16_t kernel_height;
            uint16_t kernel_width;
            unsigned int num_kernels;

            // read kernel dimensions
            fscanf(weights, "%hu\n", &kernel_height);
            fscanf(weights, "%hu\n", &kernel_width);

            // read number of kernels
            fscanf(weights, "%u\n", &num_kernels);

            uint16_t kernel;

            // allocate memory for kernels
            (*kernels)[layer] = (float_t**) malloc(num_kernels * sizeof(float_t*));

            // fill kernels
            for(kernel = 0; kernel < num_kernels; kernel++) {

                (*kernels)[layer][kernel] = (float_t*) malloc(kernel_height*kernel_width * sizeof(float_t));
            
                unsigned int kernel_pos;
                float_t kernel_entry;

                for(kernel_pos = 0; kernel_pos < kernel_height*kernel_width; kernel_pos++) {
                    fscanf(weights, "%a\n", &kernel_entry);
                    (*kernels)[layer][kernel][kernel_pos] = kernel_entry;
                }
            }

            // read number of biasses
            uint16_t num_biasses;

            fscanf(weights, "%hu\n", &num_biasses);
            
            // allocate memory for biasses
            (*biasses)[layer] = (float_t*) malloc(num_biasses * sizeof(float_t));

            uint16_t bias;

            for(bias = 0; bias < num_biasses; bias++) {
                float_t bias_entry;
                fscanf(weights, "%a\n", &bias_entry);
                (*biasses)[layer][bias] = bias_entry;
            }
        }

        fclose(weights);
        return 0;
    }

    return 1;

}

#endif // READ_WEIGHTS_H