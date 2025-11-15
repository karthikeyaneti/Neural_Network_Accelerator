#include <iostream>
#include "parameters.h"
#include "myproject.h"

void myproject(
    input_t rgb_in[3],
    ap_uint<4> &pred_class
) {
    #pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS
    #pragma HLS INTERFACE s_axilite port=rgb_in bundle=CTRL_BUS
    #pragma HLS INTERFACE s_axilite port=pred_class bundle=CTRL_BUS
    
    #pragma HLS ARRAY_PARTITION variable=rgb_in complete dim=1

    // hls-fpga-machine-learning insert load weights
#ifndef __SYNTHESIS__
    static bool loaded_weights = false;
    if (!loaded_weights) {
        nnet::load_weights_from_txt<dense_12_weight_t, 144>(w2, "w2.txt");
        nnet::load_weights_from_txt<dense_12_bias_t, 48>(b2, "b2.txt");
        nnet::load_weights_from_txt<dense_13_weight_t, 1152>(w5, "w5.txt");
        nnet::load_weights_from_txt<dense_13_bias_t, 24>(b5, "b5.txt");
        nnet::load_weights_from_txt<logits_weight_t, 240>(w7, "w7.txt");
        nnet::load_weights_from_txt<logits_bias_t, 10>(b7, "b7.txt");
        loaded_weights = true;
    }
#endif

    // Network layers
    layer2_t layer2_out[48];
    #pragma HLS ARRAY_PARTITION variable=layer2_out complete dim=0

    layer3_t layer3_out[48];
    #pragma HLS ARRAY_PARTITION variable=layer3_out complete dim=0

    layer5_t layer5_out[24];
    #pragma HLS ARRAY_PARTITION variable=layer5_out complete dim=0

    layer6_t layer6_out[24];
    #pragma HLS ARRAY_PARTITION variable=layer6_out complete dim=0

    result_t layer7_out[10];
    #pragma HLS ARRAY_PARTITION variable=layer7_out complete dim=0

    nnet::dense<input_t, layer2_t, config2>(rgb_in, layer2_out, w2, b2);
    nnet::relu<layer2_t, layer3_t, relu_config3>(layer2_out, layer3_out);
    nnet::dense<layer3_t, layer5_t, config5>(layer3_out, layer5_out, w5, b5);
    nnet::relu<layer5_t, layer6_t, relu_config6>(layer5_out, layer6_out);
    nnet::dense<layer6_t, result_t, config7>(layer6_out, layer7_out, w7, b7);

    // Find max index (argmax)
    result_t max_val = layer7_out[0];
    ap_uint<4> max_idx = 0;
    
    ARGMAX_LOOP: for (int i = 1; i < 10; i++) {
        #pragma HLS UNROLL
        if (layer7_out[i] > max_val) {
            max_val = layer7_out[i];
            max_idx = i;
        }
    }

    pred_class = max_idx;
}