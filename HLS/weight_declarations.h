#ifndef WEIGHT_DECLARATIONS_H_
#define WEIGHT_DECLARATIONS_H_

#include "parameters.h"

// EXTERN DECLARATIONS ONLY - for use in headers
extern dense_weight_t w2[48];
extern dense_bias_t b2[16];
extern batch_normalization_scale_t s4[16];
extern batch_normalization_bias_t b4[16];
extern dense_1_weight_t w5[192];
extern dense_1_bias_t b5[12];
extern batch_normalization_1_scale_t s7[12];
extern batch_normalization_1_bias_t b7[12];
extern dense_2_weight_t w8[96];
extern dense_2_bias_t b8[8];
extern batch_normalization_2_scale_t s10[8];
extern batch_normalization_2_bias_t b10[8];
extern dense_3_weight_t w11[80];
extern dense_3_bias_t b11[10];

#endif