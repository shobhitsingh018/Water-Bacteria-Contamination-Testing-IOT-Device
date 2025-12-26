#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD I2C address and size
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust 0x27 if your I2C address is different

void setup() {
  // Initialize LCD
  lcd.begin(16, 2); // 16 columns and 2 rows
  lcd.print("Hello, ESP32!");
}

void loop() {
  // Your code here
}
