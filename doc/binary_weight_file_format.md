# Weight File Format
## General Structure
```
MAGIC_NUMBER
MODEL_NAME
NUM_LAYERS
LAYER_0_NAME
LAYER_0_OP_TYPE
LAYER_0_DATA
LAYER_1_NAME
LAYER_1_OP_TYPE
LAYER_1_DATA
...
LAYER_{NUM_LAYERS-1}_NAME
LAYER_{NUM_LAYERS-1}_OP_TYPE
LAYER_{NUM_LAYERS-1}_DATA
...
END_MARKER
```
## Structure of LAYER_X_DATA
```
if LAYER_X_OP_TYPE == "Conv":
    LAYER_X_KERNEL_HEIGHT
    LAYER_X_KERNEL_WIDTH
    LAYER_X_NUM_KERNELS
    LAYER_X_KERNEL_0[0:LAYER_X_KERNEL_HEIGHT x LAYER_X_KERNEL_WIDTH-1]
    LAYER_X_KERNEL_1[0:LAYER_X_KERNEL_HEIGHT x LAYER_X_KERNEL_WIDTH-1]
    LAYER_X_KERNEL_2[0:LAYER_X_KERNEL_HEIGHT x LAYER_X_KERNEL_WIDTH-1]
    ...
    LAYER_X_KERNEL_{LAYER_X_NUM_KERNELS-2}[0:LAYER_X_KERNEL_HEIGHT x LAYER_X_KERNEL_WIDTH-1]
    LAYER_X_KERNEL_{LAYER_X_NUM_KERNELS-1}[0:LAYER_X_KERNEL_HEIGHT x LAYER_X_KERNEL_WIDTH-1]
    LAYER_X_NUM_BIASES
    LAYER_X_BIAS[0:LAYER_0_NUM_BIASES-1]

if LAYER_X_OP_TYPE == "BatchNormalization":
    LAYER_X_NUM_GAMMA
    LAYER_X_GAMMAS[0:LAYER_X_NUM_GAMMA-1]
    LAYER_X_NUM_BETA
    LAYER_X_BETAS[0:LAYER_X_NUM_BETA-1]
    LAYER_X_NUM_MEAN
    LAYER_X_MEANS[0:LAYER_X_NUM_MEAN-1]
    LAYER_X_NUM_VARIANCE
    LAYER_X_VARIANCES[0:LAYER_X_NUM_VARIANCE-1]

if LAYER_X_OP_TYPE == ["Gemm"|"MatMul"|"Transpose"]:
    LAYER_X_KERNEL_HEIGHT
    LAYER_X_KERNEL_WIDTH
    LAYER_X_NUM_KERNELS
    LAYER_X_KERNEL_0[0:LAYER_X_KERNEL_HEIGHT x LAYER_X_KERNEL_WIDTH-1]
    LAYER_X_KERNEL_1[0:LAYER_X_KERNEL_HEIGHT x LAYER_X_KERNEL_WIDTH-1]
    LAYER_X_KERNEL_2[0:LAYER_X_KERNEL_HEIGHT x LAYER_X_KERNEL_WIDTH-1]
    ...
    LAYER_X_KERNEL_{LAYER_X_NUM_KERNELS-2}[0:LAYER_X_KERNEL_HEIGHT x LAYER_X_KERNEL_WIDTH-1]
    LAYER_X_KERNEL_{LAYER_X_NUM_KERNELS-1}[0:LAYER_X_KERNEL_HEIGHT x LAYER_X_KERNEL_WIDTH-1]
    LAYER_X_NUM_BIASES
    LAYER_X_BIAS[0:LAYER_0_NUM_BIASES-1]

if LAYER_X_OP_TYPE == "Add":
    LAYER_X_NUM_BIASES
    LAYER_X_BIAS[0:LAYER_0_NUM_BIASES-1]
```

## Example LeNet-5
```
FD
LeNet-5
4
C1
Conv
5
5
20
...
20
...
...
FC2
Gemm
10
500
1
...
10
...
end
```
