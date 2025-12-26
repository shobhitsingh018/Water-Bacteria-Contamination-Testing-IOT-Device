#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with I2C address, columns, and rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize I2C with default pins (D21=SDA, D22=SCL)
  Wire.begin(21, 22);

  // Initialize the LCD with 16 columns and 2 rows
  lcd.begin(16, 2);
  lcd.backlight();

  // Step 1: Display "Device Starting"
  lcd.setCursor(0, 0);
  lcd.print("Device Starting");

  // Wait for 8 seconds
  delay(8000);

  // Step 2: Clear screen and display "Begin Water Testing Now"
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Begin Water");
  lcd.setCursor(0, 1);
  lcd.print("Testing Now");

  // Wait for 15 seconds
  delay(15000);

  // Step 3: Clear screen and display "E. Coli Detected"
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("E. Coli");
  lcd.setCursor(0, 1);
  lcd.print("Detected");
}

void loop() {
  // No actions needed in loop for this example
}
