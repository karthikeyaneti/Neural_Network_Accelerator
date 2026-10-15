#ifndef MYPROJECT_H_
#define MYPROJECT_H_

#include "ap_fixed.h"
#include "ap_int.h"

// Include extern declarations
#include "weight_declarations.h"

// hls-fpga-machine-learning insert includes
#include "nnet_utils/nnet_helpers.h"

// hls-fpga-machine-learning insert layer-config
#include "parameters.h"

// Output type
typedef ap_uint<4> result_t;

void myproject(
    input_t input_layer[3],
    result_t &predicted_class
);

#endif