// ======================
// ESP32 Color Detection
// ======================

// Pin Definitions
#define LED_R 16
#define LED_G 17
#define LED_B 18
#define LDR_PIN 34
#define BUTTON_PIN 4

// Settings
const int SETTLE_MS = 50;
const int calibratedPWM_R = 255;
const int calibratedPWM_G = 225;
const int calibratedPWM_B = 255;
const int ADC_MAX = 4095;

// Global variables
bool systemReady = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// ------------------------------------------------
// LED Control Functions
// ------------------------------------------------
void setupLEDC() {
  // Configure LEDC PWM using ledcAttach (your core's function)
  ledcAttach(LED_R, 5000, 8);  // pin, freq, resolution
  ledcAttach(LED_G, 5000, 8);
  ledcAttach(LED_B, 5000, 8);
}

void ledOff() {
  ledcWrite(LED_R, 0);
  ledcWrite(LED_G, 0);
  ledcWrite(LED_B, 0);
}

// ------------------------------------------------
// LDR Reading Functions
// ------------------------------------------------
int readLDRWithLED(int ledPin, int pwmValue) {
  ledcWrite(ledPin, pwmValue);
  delay(100);  // Longer delay for dark objects
  
  // Take more samples
  long total = 0;
  for(int i = 0; i < 10; i++) {
    total += analogRead(LDR_PIN);
    delay(5);
  }
  
  int ldrValue = total / 10;
  ledcWrite(ledPin, 0);
  delay(50);
  
  return ldrValue;
}


// ------------------------------------------------
// Button Functions
// ------------------------------------------------
bool isButtonPressed() {
  static bool lastButtonState = HIGH;
  bool reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {  // Button pressed (LOW due to pull-up)
      lastButtonState = reading;
      return true;
    }
  }
  
  lastButtonState = reading;
  return false;
}

void waitForButtonPress() {  
  while(true) {
    if(isButtonPressed()) {
      // Wait for button release
      while(digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }
      delay(100);  // Extra debounce
      break;
    }
    delay(50);
  }
}

// ------------------------------------------------
// Setup and Main Loop
// ------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);  // Wait for serial to initialize
  
  // Initialize pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Setup ADC
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  // Setup LEDs
  setupLEDC();
  ledOff();

  Serial.println("SETUP INITIALIZED");
  
  systemReady = true;
}

void loop() {
  if(!systemReady) return;
  
  waitForButtonPress();
  
  int valR = readLDRWithLED(LED_R, calibratedPWM_R);
  int valG = readLDRWithLED(LED_G, calibratedPWM_G);
  int valB = readLDRWithLED(LED_B, calibratedPWM_B);

  Serial.print(valR);
  Serial.print(", ");
  Serial.print(valG);
  Serial.print(", ");
  Serial.println(valB);
  
  delay(200);
}