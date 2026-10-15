#include <stdio.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xmyproject.h"
#include "sleep.h"

// Base addresses
#define MYPROJECT_BASEADDR XPAR_MYPROJECT_0_BASEADDR
#define LED_GPIO_BASEADDR XPAR_AXI_GPIO_0_BASEADDR
#define RGB_GPIO_BASEADDR XPAR_AXI_GPIO_1_BASEADDR  
#define BUTTON_GPIO_BASEADDR XPAR_AXI_GPIO_2_BASEADDR

// Scaler parameters
const float SCALER_MEAN[3] = {496.28f, 641.282f, 192.652f};
const float SCALER_SCALE[3] = {481.60535047f, 478.45545506f, 217.77323733f};

// Global variables
XGpio Gpio_LED, Gpio_RGB, Gpio_Button;
XMyproject MyprojectInstance;

// Test cases (100 samples)
typedef struct {
    int red;
    int green;
    int blue;
    int expected_class;
    const char* color_name;
} TestCase;

TestCase test_cases[] = {
    // Black samples (Class 0)
    {0, 0, 0, 0, "Black"}, {10, 10, 10, 0, "Black"}, {20, 20, 20, 0, "Black"},
    {5, 5, 5, 0, "Black"}, {15, 15, 15, 0, "Black"}, {25, 25, 25, 0, "Black"},
    {8, 8, 8, 0, "Black"}, {12, 12, 12, 0, "Black"}, {18, 18, 18, 0, "Black"},
    {22, 22, 22, 0, "Black"},
    
    // Blue samples (Class 1)
    {0, 340, 335, 1, "Blue"}, {15, 360, 355, 1, "Blue"}, {30, 390, 385, 1, "Blue"},
    {5, 330, 325, 1, "Blue"}, {20, 370, 365, 1, "Blue"}, {35, 400, 395, 1, "Blue"},
    {8, 345, 340, 1, "Blue"}, {25, 380, 375, 1, "Blue"}, {32, 395, 390, 1, "Blue"},
    {12, 355, 350, 1, "Blue"},
    
    // Green samples (Class 2)
    {41, 945, 9, 2, "Green"}, {55, 975, 18, 2, "Green"}, {70, 1005, 27, 2, "Green"},
    {45, 955, 12, 2, "Green"}, {60, 985, 21, 2, "Green"}, {75, 1015, 30, 2, "Green"},
    {48, 965, 14, 2, "Green"}, {65, 995, 24, 2, "Green"}, {72, 1008, 28, 2, "Green"},
    {52, 980, 16, 2, "Green"},
    
    // Orange samples (Class 3)
    {986, 546, 11, 3, "Orange"}, {1000, 560, 25, 3, "Orange"}, {1015, 575, 40, 3, "Orange"},
    {990, 550, 15, 3, "Orange"}, {1005, 565, 30, 3, "Orange"}, {1020, 580, 45, 3, "Orange"},
    {995, 555, 20, 3, "Orange"}, {1010, 570, 35, 3, "Orange"}, {1025, 585, 50, 3, "Orange"},
    {985, 548, 12, 3, "Orange"},
    
    // Pink samples (Class 4)
    {327, 109, 147, 4, "Pink"}, {340, 125, 160, 4, "Pink"}, {355, 140, 175, 4, "Pink"},
    {330, 115, 150, 4, "Pink"}, {345, 130, 165, 4, "Pink"}, {360, 145, 180, 4, "Pink"},
    {335, 120, 155, 4, "Pink"}, {350, 135, 170, 4, "Pink"}, {365, 150, 185, 4, "Pink"},
    {325, 110, 145, 4, "Pink"},
    
    // Purple samples (Class 5)
    {30, 224, 254, 5, "Purple"}, {45, 240, 270, 5, "Purple"}, {60, 255, 285, 5, "Purple"},
    {35, 230, 260, 5, "Purple"}, {50, 245, 275, 5, "Purple"}, {65, 260, 290, 5, "Purple"},
    {40, 235, 265, 5, "Purple"}, {55, 250, 280, 5, "Purple"}, {70, 265, 295, 5, "Purple"},
    {32, 228, 258, 5, "Purple"},
    
    // Red samples (Class 6)
    {835, 128, 0, 6, "Red"}, {850, 140, 15, 6, "Red"}, {865, 155, 30, 6, "Red"},
    {840, 130, 5, 6, "Red"}, {855, 145, 20, 6, "Red"}, {870, 160, 35, 6, "Red"},
    {845, 135, 10, 6, "Red"}, {860, 150, 25, 6, "Red"}, {875, 165, 40, 6, "Red"},
    {830, 125, 2, 6, "Red"},
    
    // Teal samples (Class 7)
    {0, 594, 126, 7, "Teal"}, {15, 610, 140, 7, "Teal"}, {30, 625, 155, 7, "Teal"},
    {5, 600, 130, 7, "Teal"}, {20, 615, 145, 7, "Teal"}, {35, 630, 160, 7, "Teal"},
    {10, 605, 135, 7, "Teal"}, {25, 620, 150, 7, "Teal"}, {40, 635, 165, 7, "Teal"},
    {8, 602, 132, 7, "Teal"},
    
    // White samples (Class 8)
    {889, 1459, 693, 8, "White"}, {905, 1475, 710, 8, "White"}, {920, 1490, 725, 8, "White"},
    {895, 1465, 700, 8, "White"}, {910, 1480, 715, 8, "White"}, {925, 1495, 730, 8, "White"},
    {900, 1470, 705, 8, "White"}, {915, 1485, 720, 8, "White"}, {930, 1500, 735, 8, "White"},
    {885, 1455, 690, 8, "White"},
    
    // Yellow samples (Class 9)
    {948, 1257, 30, 9, "Yellow"}, {960, 1270, 45, 9, "Yellow"}, {975, 1285, 60, 9, "Yellow"},
    {950, 1260, 35, 9, "Yellow"}, {965, 1275, 50, 9, "Yellow"}, {980, 1290, 65, 9, "Yellow"},
    {955, 1265, 40, 9, "Yellow"}, {970, 1280, 55, 9, "Yellow"}, {985, 1295, 70, 9, "Yellow"},
    {945, 1255, 28, 9, "Yellow"}
};

const char* color_names[] = {
    "Black", "Blue", "Green", "Orange", "Pink",
    "Purple", "Red", "Teal", "White", "Yellow"
};

// Convert float to 12-bit fixed point
u32 float_to_fixed12(float value) {
    int scaled = (int)(value * 64.0f);
    return scaled & 0xFFF;
}

void scale_input(int red, int green, int blue, u32 scaled_input[3]) {
    scaled_input[0] = float_to_fixed12((red - SCALER_MEAN[0]) / SCALER_SCALE[0]);
    scaled_input[1] = float_to_fixed12((green - SCALER_MEAN[1]) / SCALER_SCALE[1]);
    scaled_input[2] = float_to_fixed12((blue - SCALER_MEAN[2]) / SCALER_SCALE[2]);
}

void set_leds(int color_class) {
    XGpio_DiscreteWrite(&Gpio_LED, 1, color_class);
}

void set_rgb_led(int color_class) {
    u8 rgb_value = 0;
    switch(color_class) {
        case 0: rgb_value = 0x00; break; // Black - all off
        case 1: rgb_value = 0x01; break; // Blue
        case 2: rgb_value = 0x02; break; // Green
        case 3: rgb_value = 0x04; break; // Orange (Red + Green)
        case 4: rgb_value = 0x05; break; // Pink (Red + Blue)
        case 5: rgb_value = 0x03; break; // Purple (Red + Blue)
        case 6: rgb_value = 0x04; break; // Red
        case 7: rgb_value = 0x03; break; // Teal (Green + Blue)
        case 8: rgb_value = 0x07; break; // White (all on)
        case 9: rgb_value = 0x06; break; // Yellow (Red + Green)
    }
    XGpio_DiscreteWrite(&Gpio_RGB, 1, rgb_value);
}

// Run inference
int run_inference(int red, int green, int blue) {
    u32 scaled_input[3];
    scale_input(red, green, blue, scaled_input);
    
    // Write inputs to the NN IP
    word_type input_words[2];
    input_words[0] = (scaled_input[1] << 16) | scaled_input[0];
    input_words[1] = scaled_input[2];
    
    XMyproject_Write_input_layer_Words(&MyprojectInstance, 0, input_words, 2);
    
    // Start inference
    XMyproject_Start(&MyprojectInstance);
    
    // Wait for completion
    while (!XMyproject_IsDone(&MyprojectInstance)) {
        // Wait for inference to complete
    }
    
    // Read prediction
    u32 prediction = XMyproject_Get_predicted_class(&MyprojectInstance);
    
    return prediction & 0xF;
}

// Single test forward
void single_test_forward() {
    static int test_index = 0;
    int total_tests = sizeof(test_cases) / sizeof(TestCase);
    
    int predicted_class = run_inference(
        test_cases[test_index].red, 
        test_cases[test_index].green, 
        test_cases[test_index].blue);
    
    xil_printf("Test %d/%d:\r\n", test_index + 1, total_tests);
    xil_printf("  Input: R=%d, G=%d, B=%d\r\n", 
               test_cases[test_index].red, 
               test_cases[test_index].green, 
               test_cases[test_index].blue);
    xil_printf("  Expected: %s\r\n", test_cases[test_index].color_name);
    xil_printf("  Predicted: %s\r\n", color_names[predicted_class]);
    xil_printf("  Result: %s\r\n", 
               (predicted_class == test_cases[test_index].expected_class) ? "✓ PASS" : "✗ FAIL");
    
    set_leds(predicted_class);
    set_rgb_led(predicted_class);
    
    test_index = (test_index + 1) % total_tests;
}

// Single test backward
void single_test_backward() {
    static int test_index = 0;
    int total_tests = sizeof(test_cases) / sizeof(TestCase);
    
    // Move backward
    test_index = (test_index - 1 + total_tests) % total_tests;
    
    int predicted_class = run_inference(
        test_cases[test_index].red, 
        test_cases[test_index].green, 
        test_cases[test_index].blue);
    
    xil_printf("Test %d/%d (Backward):\r\n", test_index + 1, total_tests);
    xil_printf("  Input: R=%d, G=%d, B=%d\r\n", 
               test_cases[test_index].red, 
               test_cases[test_index].green, 
               test_cases[test_index].blue);
    xil_printf("  Expected: %s\r\n", test_cases[test_index].color_name);
    xil_printf("  Predicted: %s\r\n", color_names[predicted_class]);
    xil_printf("  Result: %s\r\n", 
               (predicted_class == test_cases[test_index].expected_class) ? "✓ PASS" : "✗ FAIL");
    
    set_leds(predicted_class);
    set_rgb_led(predicted_class);
}

// Complete accuracy test
void complete_accuracy_test() {
    int total_tests = sizeof(test_cases) / sizeof(TestCase);
    int correct_predictions = 0;
    int class_correct[10] = {0};
    int class_total[10] = {0};
    
    xil_printf("\r\n=== Complete Accuracy Test ===\r\n");
    xil_printf("Testing all %d samples...\r\n", total_tests);
    
    for (int i = 0; i < total_tests; i++) {
        int predicted_class = run_inference(
            test_cases[i].red, test_cases[i].green, test_cases[i].blue);
        
        int is_correct = (predicted_class == test_cases[i].expected_class);
        
        // Update statistics
        class_total[test_cases[i].expected_class]++;
        if (is_correct) {
            correct_predictions++;
            class_correct[test_cases[i].expected_class]++;
        }
        
        // Show progress every 10 tests
        if ((i + 1) % 10 == 0) {
            xil_printf("Completed %d/%d tests...\r\n", i + 1, total_tests);
            set_leds((i / 10) % 10);
            set_rgb_led((i / 10) % 10);
        }
    }
    
    // Calculate and display results
    float accuracy = (float)correct_predictions / total_tests * 100.0;
    
    xil_printf("\r\n=== Final Results ===\r\n");
    printf("Overall Accuracy: %d/%d (%.2f%%)\r\n", 
               correct_predictions, total_tests, accuracy);
    
    xil_printf("\r\nPer-Class Accuracy:\r\n");
    for (int i = 0; i < 10; i++) {
        if (class_total[i] > 0) {
            float class_acc = (float)class_correct[i] / class_total[i] * 100.0;
            printf("  %-8s: %2d/%2d (%.1f%%)\r\n", color_names[i], class_correct[i], class_total[i], class_acc);
            // xil_printf("  %-8s: %2d/%2d (%.1f%%)\r\n", 
            //            color_names[i], class_correct[i], class_total[i], class_acc);
        }
    }
    
    // Set final LED based on overall accuracy
    if (accuracy >= 90.0) {
        set_leds(8); // White for excellent
        set_rgb_led(8);
        xil_printf("\r\n🎉 Excellent Accuracy! 🎉\r\n");
    } else if (accuracy >= 80.0) {
        set_leds(2); // Green for good
        set_rgb_led(2);
        xil_printf("\r\n✅ Good Accuracy!\r\n");
    } else if (accuracy >= 70.0) {
        set_leds(9); // Yellow for average
        set_rgb_led(9);
        xil_printf("\r\n⚠️  Average Accuracy\r\n");
    } else {
        set_leds(6); // Red for poor
        set_rgb_led(6);
        xil_printf("\r\n❌ Needs Improvement\r\n");
    }
    xil_printf("\n");
}

// LED test function
void test_leds() {
    xil_printf("\r\n=== LED Test ===\r\n");
    xil_printf("Testing all LEDs and colors...\r\n");
    
    for (int i = 0; i < 10; i++) {
        xil_printf("Color: %s\r\n", color_names[i]);
        set_leds(i);
        set_rgb_led(i);
        usleep(500000); // 0.5 second delay
    }
    
    // Blink all LEDs
    xil_printf("Blinking all LEDs...\r\n");
    for (int i = 0; i < 5; i++) {
        set_leds(0xFF); // All LEDs on
        set_rgb_led(7); // White RGB
        usleep(200000);
        set_leds(0x00); // All LEDs off
        set_rgb_led(0); // RGB off
        usleep(200000);
    }
    
    // Reset to off
    set_leds(0);
    set_rgb_led(0);
    xil_printf("LED test complete.\r\n");
}

int main() {
    int status;
    
    xil_printf("=== Final Color Classifier ===\r\n");
    xil_printf("Total test samples: %d\r\n", sizeof(test_cases) / sizeof(TestCase));
    
    // Initialize Neural Network IP
    status = XMyproject_Initialize(&MyprojectInstance, MYPROJECT_BASEADDR);
    if (status != XST_SUCCESS) {
        xil_printf("ERROR: Failed to initialize NN IP\r\n");
        return -1;
    }
    xil_printf("NN IP initialized successfully\r\n");
    
    // Initialize GPIO
    XGpio_Initialize(&Gpio_LED, LED_GPIO_BASEADDR);
    XGpio_Initialize(&Gpio_RGB, RGB_GPIO_BASEADDR);
    XGpio_Initialize(&Gpio_Button, BUTTON_GPIO_BASEADDR);
    XGpio_SetDataDirection(&Gpio_LED, 1, 0x00);
    XGpio_SetDataDirection(&Gpio_RGB, 1, 0x00);
    XGpio_SetDataDirection(&Gpio_Button, 1, 0xFF);
    
    xil_printf("GPIO initialized successfully\r\n");
    
    xil_printf("\r\n=== Control Menu ===\r\n");
    xil_printf("Button 0: Single Test Forward\r\n");
    xil_printf("Button 1: Single Test Backward\r\n");
    xil_printf("Button 2: Complete Accuracy Test\r\n");
    xil_printf("Button 3: LED Test\r\n\r\n");
    
    u32 last_buttons = 0;
    
    while(1) {
        u32 buttons = XGpio_DiscreteRead(&Gpio_Button, 1);
        
        if ((buttons != 0) && (last_buttons == 0)) {
            xil_printf("=== Button Press: 0x%02X ===\r\n", buttons);
            
            if (buttons & 0x01) {
                single_test_forward();
            }
            else if (buttons & 0x02) {
                single_test_backward();
            }
            else if (buttons & 0x04) {
                complete_accuracy_test();
            }
            else if (buttons & 0x08) {
                test_leds();
            }
            
            xil_printf("--- Ready ---\r\n\r\n");
            usleep(300000); // Debounce
        }
        
        last_buttons = buttons;
        usleep(10000);
    }
    
    return 0;
}