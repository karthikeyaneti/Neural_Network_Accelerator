#include "myproject.h"
#include "parameters.h"
#include "weight_definitions.h"

#ifndef __SYNTHESIS__
#include <iostream>
#endif

void myproject(
    input_t input_layer[3],
    result_t &predicted_class
) {

    #pragma HLS INTERFACE s_axilite port=input_layer bundle=control
    #pragma HLS INTERFACE s_axilite port=predicted_class bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control
    #pragma HLS DESIGN top

#ifndef __SYNTHESIS__
    static bool first_run = true;
    if (first_run) {
        std::cout << "=== COMPREHENSIVE DEBUG ===" << std::endl;
        
        // 1. Check input scaling
        std::cout << "Input range check - should be around [-2, 2]: [" 
                  << input_layer[0] << ", " << input_layer[1] << ", " << input_layer[2] << "]" << std::endl;
        
        // 2. Check if weights are loaded
        std::cout << "Weight check - w2[0]: " << w2[0] << " b2[0]: " << b2[0] << std::endl;
        std::cout << "BatchNorm check - s4[0]: " << s4[0] << " b4[0]: " << b4[0] << std::endl;
        
        // 3. Check if any weights are non-zero
        bool any_nonzero = false;
        for(int i = 0; i < 48; i++) {
            if(w2[i] != 0) {
                any_nonzero = true;
                break;
            }
        }
        std::cout << "Any non-zero weights in w2: " << (any_nonzero ? "YES" : "NO - PROBLEM!") << std::endl;
        
        first_run = false;
    }
#endif

    // Temporary array for original 10 outputs
    result_t temp_outputs[10];
    #pragma HLS ARRAY_PARTITION variable=temp_outputs complete dim=0

    // Network layers
    dense_result_t layer2_out[16];
    layer3_t layer3_out[16];
    batch_normalization_result_t layer4_out[16];
    dense_1_result_t layer5_out[12];
    layer6_t layer6_out[12];
    batch_normalization_1_result_t layer7_out[12];
    dense_2_result_t layer8_out[8];
    layer9_t layer9_out[8];
    batch_normalization_2_result_t layer10_out[8];
    dense_3_result_t layer11_out[10];

    // Layer 1: Dense
    nnet::dense<input_t, dense_result_t, config2>(input_layer, layer2_out, w2, b2);

#ifndef __SYNTHESIS__
    std::cout << "After Dense1 - layer2_out[0]: " << layer2_out[0] << std::endl;
#endif

    // ReLU + BatchNorm Layer 1
    nnet::relu<dense_result_t, layer3_t, relu_config3>(layer2_out, layer3_out);
    nnet::normalize<layer3_t, batch_normalization_result_t, config4>(layer3_out, layer4_out, s4, b4);

#ifndef __SYNTHESIS__
    std::cout << "After BN1 - layer4_out[0]: " << layer4_out[0] << std::endl;
#endif

    // Layer 2: Dense
    nnet::dense<batch_normalization_result_t, dense_1_result_t, config5>(layer4_out, layer5_out, w5, b5);

#ifndef __SYNTHESIS__
    std::cout << "After Dense2 - layer5_out[0]: " << layer5_out[0] << std::endl;
#endif

    // ReLU + BatchNorm Layer 2
    nnet::relu<dense_1_result_t, layer6_t, relu_config6>(layer5_out, layer6_out);
    nnet::normalize<layer6_t, batch_normalization_1_result_t, config7>(layer6_out, layer7_out, s7, b7);

#ifndef __SYNTHESIS__
    std::cout << "After BN2 - layer7_out[0]: " << layer7_out[0] << std::endl;
#endif

    // Layer 3: Dense
    nnet::dense<batch_normalization_1_result_t, dense_2_result_t, config8>(layer7_out, layer8_out, w8, b8);

#ifndef __SYNTHESIS__
    std::cout << "After Dense3 - layer8_out[0]: " << layer8_out[0] << std::endl;
#endif

    // ReLU + BatchNorm Layer 3
    nnet::relu<dense_2_result_t, layer9_t, relu_config9>(layer8_out, layer9_out);
    nnet::normalize<layer9_t, batch_normalization_2_result_t, config10>(layer9_out, layer10_out, s10, b10);

#ifndef __SYNTHESIS__
    std::cout << "After BN3 - layer10_out[0]: " << layer10_out[0] << std::endl;
#endif

    // Output Layer: Dense
    nnet::dense<batch_normalization_2_result_t, dense_3_result_t, config11>(layer10_out, layer11_out, w11, b11);

#ifndef __SYNTHESIS__
    std::cout << "After Dense4 (raw logits): [";
    for(int i = 0; i < 10; i++) {
        std::cout << layer11_out[i];
        if(i < 9) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
#endif

    // Sigmoid activation
    nnet::sigmoid<dense_3_result_t, result_t, sigmoid_config12>(layer11_out, temp_outputs);

#ifndef __SYNTHESIS__
    std::cout << "After Sigmoid: [";
    for(int i = 0; i < 10; i++) {
        std::cout << temp_outputs[i];
        if(i < 9) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
#endif

    // ArgMax
    result_t max_val = temp_outputs[0];
    ap_uint<4> max_idx = 0;
    
    for(int i = 1; i < 10; i++) {
        if(temp_outputs[i] > max_val) {
            max_val = temp_outputs[i];
            max_idx = i;
        }
    }
    
    predicted_class = max_idx;

#ifndef __SYNTHESIS__
    std::cout << "Final prediction: " << max_idx << " (value: " << max_val << ")" << std::endl;
    std::cout << "---" << std::endl;
#endif
}