#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include "defines.h"

#include "nnet_utils/nnet_code_gen.h"
#include "nnet_utils/nnet_helpers.h"
// hls-fpga-machine-learning insert includes
#include "nnet_utils/nnet_activation.h"
#include "nnet_utils/nnet_activation_stream.h"
#include "nnet_utils/nnet_batchnorm.h"
#include "nnet_utils/nnet_batchnorm_stream.h"
#include "nnet_utils/nnet_dense.h"
#include "nnet_utils/nnet_dense_compressed.h"
#include "nnet_utils/nnet_dense_stream.h"

// hls-fpga-machine-learning insert layer-config
// dense
struct config2 : nnet::dense_config {
    static const unsigned n_in = 3;
    static const unsigned n_out = 16;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::resource;
    static const unsigned reuse_factor = 48;
    static const unsigned n_zeros = 0;
    static const unsigned n_nonzeros = 48;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef dense_bias_t bias_t;
    typedef dense_weight_t weight_t;
    typedef layer2_index index_t;
    template<class data_T, class res_T, class CONFIG_T>
    using kernel = nnet::DenseResource_rf_gt_nin_rem0<data_T, res_T, CONFIG_T>;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_relu
struct relu_config3 : nnet::activ_config {
    static const unsigned n_in = 16;
    static const unsigned table_size = 1024;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 256;
    typedef dense_relu_table_t table_t;
};

// batch_normalization
struct config4 : nnet::batchnorm_config {
    static const unsigned n_in = 16;
    static const unsigned n_filt = -1;
    static const unsigned n_scale_bias = (n_filt == -1) ? n_in : n_filt;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 256;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in, reuse_factor);
    static const bool store_weights_in_bram = false;
    typedef batch_normalization_bias_t bias_t;
    typedef batch_normalization_scale_t scale_t;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_1
struct config5 : nnet::dense_config {
    static const unsigned n_in = 16;
    static const unsigned n_out = 12;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::resource;
    static const unsigned reuse_factor = 192;
    static const unsigned n_zeros = 0;
    static const unsigned n_nonzeros = 192;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef dense_1_bias_t bias_t;
    typedef dense_1_weight_t weight_t;
    typedef layer5_index index_t;
    template<class data_T, class res_T, class CONFIG_T>
    using kernel = nnet::DenseResource_rf_gt_nin_rem0<data_T, res_T, CONFIG_T>;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_1_relu
struct relu_config6 : nnet::activ_config {
    static const unsigned n_in = 12;
    static const unsigned table_size = 1024;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 256;
    typedef dense_1_relu_table_t table_t;
};

// batch_normalization_1
struct config7 : nnet::batchnorm_config {
    static const unsigned n_in = 12;
    static const unsigned n_filt = -1;
    static const unsigned n_scale_bias = (n_filt == -1) ? n_in : n_filt;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 256;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in, reuse_factor);
    static const bool store_weights_in_bram = false;
    typedef batch_normalization_1_bias_t bias_t;
    typedef batch_normalization_1_scale_t scale_t;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_2
struct config8 : nnet::dense_config {
    static const unsigned n_in = 12;
    static const unsigned n_out = 8;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::resource;
    static const unsigned reuse_factor = 96;
    static const unsigned n_zeros = 0;
    static const unsigned n_nonzeros = 96;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef dense_2_bias_t bias_t;
    typedef dense_2_weight_t weight_t;
    typedef layer8_index index_t;
    template<class data_T, class res_T, class CONFIG_T>
    using kernel = nnet::DenseResource_rf_gt_nin_rem0<data_T, res_T, CONFIG_T>;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_2_relu
struct relu_config9 : nnet::activ_config {
    static const unsigned n_in = 8;
    static const unsigned table_size = 1024;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 256;
    typedef dense_2_relu_table_t table_t;
};

// batch_normalization_2
struct config10 : nnet::batchnorm_config {
    static const unsigned n_in = 8;
    static const unsigned n_filt = -1;
    static const unsigned n_scale_bias = (n_filt == -1) ? n_in : n_filt;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 256;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in, reuse_factor);
    static const bool store_weights_in_bram = false;
    typedef batch_normalization_2_bias_t bias_t;
    typedef batch_normalization_2_scale_t scale_t;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_3
struct config11 : nnet::dense_config {
    static const unsigned n_in = 8;
    static const unsigned n_out = 10;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::resource;
    static const unsigned reuse_factor = 80;
    static const unsigned n_zeros = 0;
    static const unsigned n_nonzeros = 80;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef dense_3_bias_t bias_t;
    typedef dense_3_weight_t weight_t;
    typedef layer11_index index_t;
    template<class data_T, class res_T, class CONFIG_T>
    using kernel = nnet::DenseResource_rf_gt_nin_rem0<data_T, res_T, CONFIG_T>;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_3_sigmoid
struct sigmoid_config12 : nnet::activ_config {
    static const unsigned n_in = 10;
    static const unsigned table_size = 1024;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 256;
    typedef dense_3_sigmoid_table_t table_t;
};



#endif
