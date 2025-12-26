// DS18B20 Temperature Sensor with ESP32

#include <OneWire.h>
#include <DallasTemperature.h>

// Define pins
#define ONE_WIRE_BUS 32 // GPIO32 for DS18B20

// Setup oneWire instance to communicate with any OneWire devices (DS18B20)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200); // Start Serial Monitor
  sensors.begin();      // Initialize the sensor
  Serial.println("DS18B20 Temperature Sensor Setup Complete!");
}

void loop() {
  // Request temperature readings from DS18B20
  sensors.requestTemperatures(); 
  
  // Get temperature in Celsius
  float temperature = sensors.getTempCByIndex(0);
  
  // Display temperature on Serial Monitor
  if (temperature != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
  } else {
    Serial.println("Error: Could not read temperature data.");
  }

  delay(2000); // Delay 2 seconds before the next reading
}
