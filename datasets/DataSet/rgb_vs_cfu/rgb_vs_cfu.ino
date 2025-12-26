#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_TCS34725.h>

// I2C Addresses
#define LCD_I2C_ADDRESS 0x27

// Initialize peripherals
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

// Constants for Chlorine Dose Calculation
const float CT = 0.1;      // CT value for E. coli in mg·min/L
const int VOLUME = 10;     // Water volume in liters
const int TIME = 30;       // Contact time in minutes

// Thresholds for RGB Shift Detection
const uint16_t RGB_SHIFT_THRESHOLD = 20; // Minimum change to detect shift

// Variables for Initial RGB Values
uint16_t initialR = 0, initialG = 0, initialB = 0;

// Timing Variables
unsigned long startTime;           // Time when the process starts
unsigned long lastReadingTime = 0; // Time of the last reading
unsigned long readingInterval = 30000; // Phase 1 interval (30 seconds = 30,000 ms)

// Function to Initialize LCD
void initLCD() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Device Starting");
    delay(7000); // Display for 7 seconds
    lcd.clear();
}

// Function to Initialize TCS34725 Sensor
void initTCS34725() {
    if (!tcs.begin()) {
        Serial.println("TCS34725 not found!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Device Error");
        while (1); // Halt program if the sensor fails
    }
    Serial.println("TCS34725 initialized");
    tcs.getRawData(&initialR, &initialG, &initialB, nullptr);
    startTime = millis(); // Record the start time of the process
}

// Function to Read RGB Values
void readRGBValues(uint16_t &r, uint16_t &g, uint16_t &b) {
    uint16_t clear;
    tcs.getRawData(&r, &g, &b, &clear);
    Serial.print("R: "); Serial.print(r);
    Serial.print(" G: "); Serial.print(g);
    Serial.print(" B: "); Serial.println(b);
}

// Function to Detect RGB Shift
bool detectRGBShift(uint16_t r, uint16_t g, uint16_t b) {
    return (abs(r - initialR) > RGB_SHIFT_THRESHOLD ||
            abs(g - initialG) > RGB_SHIFT_THRESHOLD ||
            abs(b - initialB) > RGB_SHIFT_THRESHOLD);
}

// Main Setup Function
void setup() {
    Serial.begin(115200);

    // Initialize LCD
    initLCD();

    // Initialize TCS34725 Sensor
    initTCS34725();
}

// Main Loop Function
void loop() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Analyzing Sample");

    uint16_t r, g, b;
    readRGBValues(r, g, b);

    // Check for RGB shift
    if (detectRGBShift(r, g, b)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("RGB Shift Detected");
        delay(5000); // Display the detection message
        while (1); // Halt further processing
    }

    delay(1000); // Refresh the "Analyzing Sample" display every second
}
