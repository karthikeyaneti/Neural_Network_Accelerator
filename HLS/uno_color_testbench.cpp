#include <iostream>
#include <fstream>
#include <cmath>
#include "myproject.h"

// Scaler parameters
const float SCALER_MEAN[3] = {496.28f, 641.282f, 192.652f};
const float SCALER_SCALE[3] = {481.60535047f, 478.45545506f, 217.77323733f};

void scale_input(float red, float green, float blue, input_t scaled_input[3]) {
    scaled_input[0] = (red - SCALER_MEAN[0]) / SCALER_SCALE[0];
    scaled_input[1] = (green - SCALER_MEAN[1]) / SCALER_SCALE[1];
    scaled_input[2] = (blue - SCALER_MEAN[2]) / SCALER_SCALE[2];
}

void print_results(result_t predicted_class, int expected_class = -1) {
    const char* color_names[] = {"black", "blue", "green", "orange", "pink", 
                                "purple", "red", "teal", "white", "yellow"};
    
    std::cout << "Predicted: " << (int)predicted_class << " (" << color_names[(int)predicted_class] << ")" << std::endl;
    
    if(expected_class != -1) {
        std::cout << "Expected: " << expected_class << " (" << color_names[expected_class] << ")" << std::endl;
        std::cout << "Match: " << ((int)predicted_class == expected_class ? "YES" : "NO") << std::endl;
    }
}

struct TestCase {
    float red;
    float green;
    float blue;
    int expected_class;
    const char* color_name;
};

TestCase test_cases[] = {
    {0, 0, 0, 0, "black"},
    {0, 340, 335, 1, "blue"},
    {41, 945, 9, 2, "green"},
    {986, 546, 11, 3, "orange"},
    {327, 109, 147, 4, "pink"},
    {30, 224, 254, 5, "purple"},
    {835, 128, 0, 6, "red"},
    {0, 594, 126, 7, "teal"},
    {889, 1459, 693, 8, "white"},
    {948, 1257, 30, 9, "yellow"}
};

int main() {
    std::cout << "=== COLOR CLASSIFIER DIAGNOSTIC ===" << std::endl;
    
    // First, test with simple cases
    std::cout << "\n--- QUICK SANITY CHECK ---" << std::endl;
    
    input_t test_inputs[][3] = {
        {1.0f, 0.0f, 0.0f},  // Should not be black
        {0.0f, 1.0f, 0.0f},  // Should not be black
        {0.0f, 0.0f, 1.0f},  // Should not be black
    };
    
    for(int i = 0; i < 3; i++) {
        result_t predicted_class;
        myproject(test_inputs[i], predicted_class);
        std::cout << "Test " << i+1 << " -> Class: " << (int)predicted_class << std::endl;
    }
    
    // Now run actual tests
    std::cout << "\n--- MAIN TESTS ---" << std::endl;
    
    int total_tests = sizeof(test_cases)/sizeof(TestCase);
    int correct_predictions = 0;
    
    for(int i = 0; i < total_tests; i++) {
        TestCase tc = test_cases[i];
        
        std::cout << "\nTest " << (i+1) << ": " << tc.color_name << std::endl;
        std::cout << "RGB: [" << tc.red << ", " << tc.green << ", " << tc.blue << "]" << std::endl;
        
        input_t scaled_input[3];
        scale_input(tc.red, tc.green, tc.blue, scaled_input);
        
        std::cout << "Scaled: [" << scaled_input[0] << ", " << scaled_input[1] << ", " << scaled_input[2] << "]" << std::endl;
        
        result_t predicted_class;
        myproject(scaled_input, predicted_class);
        
        print_results(predicted_class, tc.expected_class);
        
        if((int)predicted_class == tc.expected_class) {
            correct_predictions++;
        }
    }
    
    std::cout << "\n=== SUMMARY ===" << std::endl;
    std::cout << "Accuracy: " << correct_predictions << "/" << total_tests << " (" 
              << (100.0 * correct_predictions / total_tests) << "%)" << std::endl;
    
    return 0;
}