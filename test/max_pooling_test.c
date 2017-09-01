/** 
 * @brief test for max-pooling
 *
 * @author Konstantin Luebeck (University of Tuebingen, Chair for Embedded Systems)
 */
#include "pico-cnn/pico-cnn.h"

void usage() {
    printf("./max_pooling_test PATH_TO_INPUT_PGM_IMAGE PATH_TO_OUTPUT_PGM_IMAGE\n");
}

int main(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr, "no path to input/output pgm image provided!\n");
        usage();
    }

    float_t* input_image;

    if(read_pgm(&input_image, argv[1], 0, 0.0, 1.0) != 0) {
        fprintf(stderr, "could not read pgm image '%s'!\n", argv[1]);
        return 1;
    }

    uint16_t height, width;
    height = 32;
    width = 32;

    float_t* output_image = (float_t*) malloc((height/2) * (width/2) * sizeof(float_t));

    max_pooling2d_naive(input_image, height, width, output_image, 2);

    write_pgm(output_image, (height/2), (width/2), argv[2]);

    free(input_image);
    free(output_image);

    return 0;
}