#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "myproject.h"
#include "nnet_utils/nnet_helpers.h"

#include "ap_int.h"

using namespace std;

// Define number of samples
#define NUM_SAMPLES 30

// Label encoding: same as training order
// 0:black,1:blue,2:green,3:orange,4:pink,5:purple,6:red,7:teal,8:white,9:yellow
const char* label_names[10] = {
    "black","blue","green","orange","pink","purple","red","teal","white","yellow"
};

int main() {
    // Extended Scaled RGB test vectors
    input_t rgb_inputs[NUM_SAMPLES][3] = {
        // Original test vectors
        { 1.04, -0.08, -0.80 },  // orange
        { 0.90,  0.08, -0.76 },  // orange
        { 0.95, -0.18, -0.90 },  // orange
        { 1.10,  0.05, -0.84 },  // orange
        { -0.48, -0.94,  0.14 }, // pink
        { 1.46,  2.05,  3.18 },  // white
        { 1.59,  1.95,  2.97 },  // white
        { -0.55, -0.93,  0.17 }, // purple
        { -1.00, -0.57,  0.97 }, // blue
        { -0.76,  0.69, -0.72 }, // green
        { 1.05, -0.04, -0.91 },  // orange
        { -0.89,  0.02, -0.52 }, // teal
        { -0.84,  0.24, -0.04 }, // teal
        { 1.46, -0.48, -0.47 },  // red
        { 1.46,  1.52, -0.23 },  // yellow
        { -0.26,  0.99, -0.25 }, // green
        
        // Additional test vectors covering all classes
        { -0.85, -0.85, -0.85 }, // black (low values)
        { 0.15,  0.45,  0.85 },  // blue (high blue)
        { 0.25,  0.85,  0.35 },  // green (high green)
        { 0.95,  0.35,  0.15 },  // orange (high red, medium green)
        { 0.85,  0.45,  0.75 },  // pink (balanced RGB)
        { 0.75,  0.35,  0.85 },  // purple (high blue, medium red)
        { 0.95,  0.25,  0.25 },  // red (high red)
        { 0.25,  0.75,  0.75 },  // teal (high green+blue)
        { 0.90,  0.90,  0.90 },  // white (high all)
        { 0.95,  0.85,  0.25 },  // yellow (high red+green)
        
        // Edge cases
        { 0.00,  0.00,  0.00 },  // black (zeros)
        { -1.50, -1.50, -1.50 }, // black (very low)
        { 2.00,  2.00,  2.00 },  // white (very high)
        { 0.50, -1.20,  0.80 },  // mixed
    };

    // Ground-truth encoded labels
    ap_uint<4> expected_labels[NUM_SAMPLES] = {
        // Original expected labels
        3,3,3,3,4,8,8,5,1,2,3,7,7,6,9,2,
        
        // Additional expected labels
        0, // black
        1, // blue
        2, // green
        3, // orange
        4, // pink
        5, // purple
        6, // red
        7, // teal
        8, // white
        9, // yellow
        
        // Edge case expected labels
        0, // black (zeros)
        0, // black (very low)
        8, // white (very high)
        1, // blue (guess)
    };

    int correct = 0;

    cout << "=== ChromaCore Neural Network Testbench (Array Interface) ===" << endl;
    cout << "Testing " << NUM_SAMPLES << " samples" << endl;
    cout << "===========================================" << endl;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        ap_uint<4> pred_class;
        
        // Call myproject with array interface
        myproject(rgb_inputs[i], pred_class);

        cout << "Sample " << i+1 << " -> "
             << "RGB_scaled = (" << rgb_inputs[i][0] << ", "
             << rgb_inputs[i][1] << ", "
             << rgb_inputs[i][2] << ")  "
             << "Pred: " << label_names[(int)pred_class]
             << " (code=" << (int)pred_class << ")  "
             << "Expected: " << label_names[(int)expected_labels[i]];

        if (pred_class == expected_labels[i]) {
            cout << " \033[1;32mCORRECT\033[0m" << endl; // Green color
            correct++;
        } else {
            cout << " \033[1;31mWRONG\033[0m" << endl;   // Red color
        }
    }

    cout << "\n===========================================" << endl;
    cout << "Final Results:" << endl;
    cout << "Correct: " << correct << "/" << NUM_SAMPLES << endl;
    cout << "Accuracy: " << (100.0 * correct / NUM_SAMPLES) << "%" << endl;
    
    // Performance summary
    if (correct == NUM_SAMPLES) {
        cout << "\033[1;32m🎉 PERFECT ACCURACY! 🎉\033[0m" << endl;
    } else if (correct >= NUM_SAMPLES * 0.8) {
        cout << "\033[1;33mGood performance!\033[0m" << endl;
    } else {
        cout << "\033[1;31mNeeds improvement\033[0m" << endl;
    }

    return 0;
}