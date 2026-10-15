#ifndef TESTBENCH_MYPROJECT_H_
#define TESTBENCH_MYPROJECT_H_

#include "myproject.h"

// External scaler parameters
extern const float SCALER_MEAN[3];
extern const float SCALER_SCALE[3];

// Function declarations
void scale_input(float red, float green, float blue, input_t scaled_input[3]);
void print_results(const result_t predictions[10], int expected_class = -1);

// Test case structure
struct TestCase {
    float red;
    float green;
    float blue;
    int expected_class;
    const char* color_name;
};

// External test cases
extern TestCase test_cases[];

#endif