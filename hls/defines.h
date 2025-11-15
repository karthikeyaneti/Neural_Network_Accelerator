#ifndef DEFINES_H_
#define DEFINES_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include "nnet_utils/nnet_types.h"
#include <array>
#include <cstddef>
#include <cstdio>
#include <tuple>
#include <tuple>


// hls-fpga-machine-learning insert numbers

// hls-fpga-machine-learning insert layer-precision
typedef ap_fixed<12,4> input_t;
typedef ap_fixed<12,4> model_default_t;
typedef ap_fixed<12,4> layer2_t;
typedef ap_fixed<8,4> dense_12_weight_t;
typedef ap_fixed<12,4> dense_12_bias_t;
typedef ap_uint<1> layer2_index;
typedef ap_fixed<12,4> layer3_t;
typedef ap_fixed<18,8> dense_12_relu_table_t;
typedef ap_fixed<12,4> layer5_t;
typedef ap_fixed<8,4> dense_13_weight_t;
typedef ap_fixed<12,4> dense_13_bias_t;
typedef ap_uint<1> layer5_index;
typedef ap_fixed<12,4> layer6_t;
typedef ap_fixed<18,8> dense_13_relu_table_t;
typedef ap_fixed<30,14> result_t;
typedef ap_fixed<12,4> logits_weight_t;
typedef ap_fixed<12,4> logits_bias_t;
typedef ap_uint<1> layer7_index;

// hls-fpga-machine-learning insert emulator-defines


#endif
