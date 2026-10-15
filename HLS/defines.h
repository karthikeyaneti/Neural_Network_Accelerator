#ifndef DEFINES_H_
#define DEFINES_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include "nnet_utils/nnet_types.h"
#include <array>
#include <cstddef>
#include <cstdio>
#include <tuple>


// hls-fpga-machine-learning insert numbers

// hls-fpga-machine-learning insert layer-precision
typedef ap_fixed<12,6> input_t;
typedef ap_fixed<12,6> model_default_t;
typedef ap_fixed<27,15> dense_result_t;
typedef ap_fixed<12,6> dense_weight_t;
typedef ap_fixed<12,6> dense_bias_t;
typedef ap_uint<1> layer2_index;
typedef ap_fixed<12,6> layer3_t;
typedef ap_fixed<18,8> dense_relu_table_t;
typedef ap_fixed<25,13> batch_normalization_result_t;
typedef ap_fixed<12,6> batch_normalization_scale_t;
typedef ap_fixed<12,6> batch_normalization_bias_t;
typedef ap_fixed<42,24> dense_1_result_t;
typedef ap_fixed<12,6> dense_1_weight_t;
typedef ap_fixed<12,6> dense_1_bias_t;
typedef ap_uint<1> layer5_index;
typedef ap_fixed<12,6> layer6_t;
typedef ap_fixed<18,8> dense_1_relu_table_t;
typedef ap_fixed<25,13> batch_normalization_1_result_t;
typedef ap_fixed<12,6> batch_normalization_1_scale_t;
typedef ap_fixed<12,6> batch_normalization_1_bias_t;
typedef ap_fixed<42,24> dense_2_result_t;
typedef ap_fixed<12,6> dense_2_weight_t;
typedef ap_fixed<12,6> dense_2_bias_t;
typedef ap_uint<1> layer8_index;
typedef ap_fixed<12,6> layer9_t;
typedef ap_fixed<18,8> dense_2_relu_table_t;
typedef ap_fixed<25,13> batch_normalization_2_result_t;
typedef ap_fixed<12,6> batch_normalization_2_scale_t;
typedef ap_fixed<12,6> batch_normalization_2_bias_t;
typedef ap_fixed<41,23> dense_3_result_t;
typedef ap_fixed<12,6> dense_3_weight_t;
typedef ap_fixed<12,6> dense_3_bias_t;
typedef ap_uint<1> layer11_index;
// typedef ap_fixed<12,6> result_t;
typedef ap_fixed<18,8> dense_3_sigmoid_table_t;

// hls-fpga-machine-learning insert emulator-defines


#endif
