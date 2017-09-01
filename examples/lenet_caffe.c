/** 
 * @brief LeNet implementation as provided by the Caffe LeNet MNIST example:
 * http://caffe.berkeleyvision.org/gathered/examples/mnist.html
 *
 * @author Konstantin Luebeck (University of Tuebingen, Chair for Embedded Systems)
 */

#include "pico-cnn/pico-cnn.h"

#define NUM 1
#define DEBUG
#define INDEX 0

void usage() {
    printf("./lenet_caffe PATH_TO_MNIST_DATASET PATH_TO_WEIGHTS_FILE\n");
}

int main(int argc, char** argv) {

    if(argc == 1) {
        fprintf(stderr, "no path to mnist dataset and weights provided!\n");
        usage();
        return 1;
    }

    if(argc == 2) {
        fprintf(stderr, "no path to weights provided!\n");
        usage();
        return 1;
    }

    int i, j, k;

    // read mnist t10k images
    char t10k_images_path[strlen(argv[1]) + 20];
    t10k_images_path[0] = '\0';
    strcat(t10k_images_path, argv[1]);
    strcat(t10k_images_path, "/t10k-images.idx3-ubyte");
    //strcat(t10k_images_path, "/train-images.idx3-ubyte");

    float_t** t10k_images;
    int num_t10k_images;
    
    printf("reading images from '%s'\n", t10k_images_path);

    num_t10k_images = read_mnist_images(t10k_images_path, &t10k_images, NUM, 0, 0.0, 1.0);

    if(num_t10k_images < 1) {
        fprintf(stderr, "could not read mnist images from '%s'\n", t10k_images_path);
        return 1;
    }

    // read t10k labels
    char t10k_labels_path[strlen(argv[1]) + 20];
    t10k_labels_path[0] = '\0';
    strcat(t10k_labels_path, argv[1]);
    strcat(t10k_labels_path, "/t10k-labels.idx1-ubyte");
    //strcat(t10k_labels_path, "/train-labels.idx1-ubyte");

    uint8_t* labels;
    labels = (uint8_t*) malloc(10*sizeof(uint8_t)); 

    uint8_t* t10k_labels;
    int num_t10k_labels;

    printf("reading labels from '%s'\n", t10k_labels_path);

    num_t10k_labels = read_mnist_labels(t10k_labels_path, &t10k_labels, NUM);

    if(num_t10k_images != num_t10k_labels) {
        fprintf(stderr, "%d images != %d labels\n", num_t10k_images, num_t10k_labels);
        return 1;
    }

    // make pgm of original image
    #ifdef DEBUG
    write_pgm(t10k_images[INDEX], 28, 28, "input.pgm");
    write_float(t10k_images[INDEX], 28, 28, "input.float");
    #endif

    // read kernels and biasses
    float_t*** kernels;
    float_t** biasses;

    printf("reading weights from '%s'\n", argv[2]);

    if(read_weights(argv[2], &kernels, &biasses) != 0) {
        fprintf(stderr, "could not read weights from '%s'\n", t10k_images_path);
        return 1;
    }

    int correct_predictions = 0;

    int confusion_matrix[10][10] = {
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0}
    };

    for(i = 0; i < NUM; i++) {
        // C1 input 28x28x1 -> output 24x24x20
        float_t** c1_output;
        c1_output = (float_t**) malloc(20*sizeof(float_t*));
        
        float_t** c1_kernels = kernels[0];
        float_t* c1_bias = biasses[0];

        for(j = 0; j < 20; j++) {
            c1_output[j] = (float_t*) malloc(24*24*sizeof(float_t));
            convolution2d_naive(t10k_images[i], 28, 28, c1_output[j], c1_kernels[j], 5, c1_bias[j]);
        }

        // make pgm C1
        #ifdef DEBUG
        if(i == INDEX) {
            float* c1_file_content = (float_t*) malloc(24*24*20*sizeof(float_t));
            for(j = 0; j < 20; j++) {
                memcpy(&c1_file_content[j*24*24], c1_output[j], 24*24*sizeof(float_t));
            }
            
            write_pgm(c1_file_content, 20*24, 24, "c1_output.pgm");
            write_float(c1_file_content, 20*24, 24, "c1_output.float");
            free(c1_file_content);
        }
        #endif

        // S2 input 24x24x20 -> output 12x12x20
        float_t** s2_output;
        s2_output = (float_t**) malloc(20*sizeof(float_t*));

        for(j = 0; j < 20; j++) {
            s2_output[j] = (float_t*) malloc(12*12*sizeof(float_t));
            max_pooling2d_naive(c1_output[j], 24, 24, s2_output[j], 2);
        }

        // make pgm S2
        #ifdef DEBUG
        if(i == INDEX) {
            float* s2_file_content = (float_t*) malloc(12*12*20*sizeof(float_t));
            for(j = 0; j < 20; j++) {
                memcpy(&s2_file_content[j*12*12], s2_output[j], 12*12*sizeof(float_t));
            }
            write_pgm(s2_file_content, 20*12, 12, "s2_output.pgm");
            write_float(s2_file_content, 20*12, 12, "s2_output.float");
            free(s2_file_content);
        }
        #endif

        // C1 free memory
        for(j = 0; j < 20; j++) {
            free(c1_output[j]);
        }

        free(c1_output);

        // C3 input 12x12x20 -> output 8x8x50
        float_t** c3_output;
        c3_output = (float_t**) malloc(50*sizeof(float_t*));

        float_t* c3_intermediate = malloc(8*8*sizeof(float_t*));
        
        float_t** c3_kernels = kernels[1];
        float_t* c3_bias = biasses[1];

        // TODO find out how (in tiny-dnn) input 20 -> output 50
        for(j = 0; j < 50; j++) {
            c3_output[j] = (float_t*) malloc(8*8*sizeof(float_t));
            for(k = 1; k < 20; k++) {

            }
            //c1_output[j] = (float_t*) malloc(24*24*sizeof(float_t));
            //convolution2d_naive(t10k_images[i], 28, 28, c1_output[j], c1_kernels[j], 5, c1_bias[j]);
        }


        // S2 free memory
        for(j = 0; j < 20; j++) {
            free(s2_output[j]);
        }

        free(s2_output);

        // C3 free memory
        for(j = 0; j < 50; j++) {
            free(c3_output[j]);
        }

        free(c3_output);

    }

    // freeing memory
    for(i = 0; i < NUM; i++) {
        free(t10k_images[i]);
    }

    free(t10k_images);
    free(t10k_labels);

    for(i = 0; i < 20; i++) {
        free(kernels[0][i]);
    }
    for(i = 0; i < 1000; i++) {
        free(kernels[1][i]);
    }

    free(kernels[2][0]);
    free(kernels[3][0]);

    for(i = 0; i < 4; i++) {
        free(biasses[i]);
    }

    return 0;
}
