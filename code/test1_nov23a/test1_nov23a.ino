#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD object with I2C address 0x27 and a 16x2 display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize the LCD
  lcd.init();         // Initialize the LCD for ESP32 (use init() instead of begin())
  lcd.backlight();    // Turn on the backlight

  // Display "Device is Starting" message
  lcd.setCursor(0, 0);  
  lcd.print("Device is Starting");
  
  // Wait for 10 seconds
  delay(10000);  // 10000 milliseconds = 10 seconds

  // Clear the screen and display the next message
  lcd.clear();
  lcd.setCursor(0, 0);  
  lcd.print("Start testing the");
  lcd.setCursor(0, 1);  // Move to the second line
  lcd.print("water now");

  // Add your additional setup code here (e.g., sensor initialization)
}

void loop() {
  // Add your loop code here
}
