#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ThingSpeak.h>

// Replace with your Wi-Fi credentials
const char* ssid = "SHOBHIT";
const char* password = "bunny1234";

// ThingSpeak API credentials
unsigned long channelID = 2769119;  // Channel ID as an unsigned long
const char* writeAPIKey = "RDM9DS2S05GYS2N0";

// DS18B20 temperature sensor
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Pin definitions
#define TDS_PIN 35
#define TURBIDITY_PIN 36
#define PH_PIN 39
#define TCS3200_S0 25
#define TCS3200_S1 26
#define TCS3200_S2 27
#define TCS3200_S3 14
#define TCS3200_OUT 34

WiFiClient client;  // Declare WiFiClient to be used by ThingSpeak

void setup() {
  // Start Serial Monitor for debugging
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  
  // Initialize ThingSpeak with WiFiClient object
  ThingSpeak.begin(client);  // Pass the WiFiClient object to ThingSpeak

  // Initialize DS18B20 sensor
  sensors.begin();

  // Setup pins for TCS3200
  pinMode(TCS3200_S0, OUTPUT);
  pinMode(TCS3200_S1, OUTPUT);
  pinMode(TCS3200_S2, OUTPUT);
  pinMode(TCS3200_S3, OUTPUT);
  pinMode(TCS3200_OUT, INPUT);
  digitalWrite(TCS3200_S0, HIGH);
  digitalWrite(TCS3200_S1, LOW);

  Serial.println("Setup complete!");
}

void loop() {
  // Measure temperature
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  // Measure TDS
  int tdsValue = analogRead(TDS_PIN);

  // Measure turbidity
  int turbidityValue = analogRead(TURBIDITY_PIN);

  // Measure pH
  int phValue = analogRead(PH_PIN);
  float voltage = phValue * (5.0 / 1023.0); // Convert to voltage (assuming 5V reference)
  float pH = 7 + ((2.5 - voltage) / 0.18); // Calculate pH based on sensor characteristics

  // TCS3200 (color sensor)
  digitalWrite(TCS3200_S2, LOW);
  digitalWrite(TCS3200_S3, LOW);
  int red = pulseIn(TCS3200_OUT, LOW);
  digitalWrite(TCS3200_S3, HIGH);
  int blue = pulseIn(TCS3200_OUT, LOW);
  digitalWrite(TCS3200_S2, HIGH);
  int green = pulseIn(TCS3200_OUT, LOW);

  // Display the data to the Serial Monitor for debugging
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C | TDS: ");
  Serial.print(tdsValue);
  Serial.print(" | Turbidity: ");
  Serial.print(turbidityValue);
  Serial.print(" | pH: ");
  Serial.print(pH);
  Serial.print(" | Red: ");
  Serial.print(red);
  Serial.print(" | Green: ");
  Serial.print(green);
  Serial.print(" | Blue: ");
  Serial.println(blue);

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temperature);   // Field 1 = Temperature
  ThingSpeak.setField(2, tdsValue);      // Field 2 = TDS
  ThingSpeak.setField(3, turbidityValue); // Field 3 = Turbidity
  ThingSpeak.setField(4, pH);            // Field 4 = pH
  ThingSpeak.setField(5, red);           // Field 5 = Red (TCS3200)
  ThingSpeak.setField(6, green);         // Field 6 = Green (TCS3200)
  ThingSpeak.setField(7, blue);          // Field 7 = Blue (TCS3200)

  // Update the ThingSpeak channel
  int responseCode = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (responseCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.print("Error sending data to ThingSpeak. Response code: ");
    Serial.println(responseCode);
  }

  // Wait before the next iteration (2.5 seconds)
  delay(2500);
}
