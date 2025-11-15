#ifndef MYPROJECT_H_
#define MYPROJECT_H_

#include "ap_int.h"
#include "ap_fixed.h"
#include "defines.h"

void myproject(
    input_t rgb_in[3],
    ap_uint<4> &pred_class
);

#endif