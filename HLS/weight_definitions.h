#ifndef WEIGHT_DEFINITIONS_H_
#define WEIGHT_DEFINITIONS_H_

// This file contains the actual weight array DEFINITIONS
// Include this ONLY in myproject.cpp, NOT in the testbench

#include "parameters.h"

// WEIGHT ARRAY DEFINITIONS
dense_weight_t w2[48] = {
    #include "weights/w2.txt"
};

dense_bias_t b2[16] = {
    #include "weights/b2.txt"
};

batch_normalization_scale_t s4[16] = {
    #include "weights/s4.txt"
};

batch_normalization_bias_t b4[16] = {
    #include "weights/b4.txt"
};

dense_1_weight_t w5[192] = {
    #include "weights/w5.txt"
};

dense_1_bias_t b5[12] = {
    #include "weights/b5.txt"
};

batch_normalization_1_scale_t s7[12] = {
    #include "weights/s7.txt"
};

batch_normalization_1_bias_t b7[12] = {
    #include "weights/b7.txt"
};

dense_2_weight_t w8[96] = {
    #include "weights/w8.txt"
};

dense_2_bias_t b8[8] = {
    #include "weights/b8.txt"
};

batch_normalization_2_scale_t s10[8] = {
    #include "weights/s10.txt"
};

batch_normalization_2_bias_t b10[8] = {
    #include "weights/b10.txt"
};

dense_3_weight_t w11[80] = {
    #include "weights/w11.txt"
};

dense_3_bias_t b11[10] = {
    #include "weights/b11.txt"
};

#endif