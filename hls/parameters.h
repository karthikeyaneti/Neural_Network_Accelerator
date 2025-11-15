#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "ap_fixed.h"
#include "ap_int.h"

#include "nnet_utils/nnet_code_gen.h"
#include "nnet_utils/nnet_helpers.h"
// hls-fpga-machine-learning insert includes
#include "nnet_utils/nnet_activation.h"
#include "nnet_utils/nnet_activation_stream.h"
#include "nnet_utils/nnet_dense.h"
#include "nnet_utils/nnet_dense_compressed.h"
#include "nnet_utils/nnet_dense_stream.h"

#include "defines.h"

// hls-fpga-machine-learning insert weights
#include "weights/w2.h"
#include "weights/b2.h"
#include "weights/w5.h"
#include "weights/b5.h"
#include "weights/w7.h"
#include "weights/b7.h"


// hls-fpga-machine-learning insert layer-config
// dense_12
struct config2 : nnet::dense_config {
    static const unsigned n_in = 3;
    static const unsigned n_out = 48;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::resource;
    static const unsigned reuse_factor = 48;
    static const unsigned n_zeros = 0;
    static const unsigned n_nonzeros = 144;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef dense_12_bias_t bias_t;
    typedef dense_12_weight_t weight_t;
    typedef layer2_index index_t;
    template<class data_T, class res_T, class CONFIG_T>
    using kernel = nnet::DenseResource_rf_gt_nin_rem0<data_T, res_T, CONFIG_T>;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_12_relu
struct relu_config3 : nnet::activ_config {
    static const unsigned n_in = 48;
    static const unsigned table_size = 1024;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 48;
    typedef dense_12_relu_table_t table_t;
};

// dense_13
struct config5 : nnet::dense_config {
    static const unsigned n_in = 48;
    static const unsigned n_out = 24;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::resource;
    static const unsigned reuse_factor = 48;
    static const unsigned n_zeros = 0;
    static const unsigned n_nonzeros = 1152;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef dense_13_bias_t bias_t;
    typedef dense_13_weight_t weight_t;
    typedef layer5_index index_t;
    template<class data_T, class res_T, class CONFIG_T>
    using kernel = nnet::DenseResource_rf_leq_nin<data_T, res_T, CONFIG_T>;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_13_relu
struct relu_config6 : nnet::activ_config {
    static const unsigned n_in = 24;
    static const unsigned table_size = 1024;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 48;
    typedef dense_13_relu_table_t table_t;
};

// logits
struct config7 : nnet::dense_config {
    static const unsigned n_in = 24;
    static const unsigned n_out = 10;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::resource;
    static const unsigned reuse_factor = 48;
    static const unsigned n_zeros = 0;
    static const unsigned n_nonzeros = 240;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef logits_bias_t bias_t;
    typedef logits_weight_t weight_t;
    typedef layer7_index index_t;
    template<class data_T, class res_T, class CONFIG_T>
    using kernel = nnet::DenseResource_rf_gt_nin_rem0<data_T, res_T, CONFIG_T>;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};



#endif
