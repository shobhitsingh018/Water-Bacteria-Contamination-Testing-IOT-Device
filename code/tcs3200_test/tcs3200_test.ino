#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "SHOBHIT";          // Replace with your WiFi SSID
const char* password = "bunny1234";  // Replace with your WiFi password

// ThingSpeak API
String apiKey = "DBBKEE70ZI9M37ID";         // Replace with your ThingSpeak Write API Key
const char* server = "http://api.thingspeak.com";

// TCS3200 Pins
#define S0 26
#define S1 25
#define S2 33
#define S3 32
#define OUT 27

void setup() {
  Serial.begin(115200);

  // Initialize TCS3200 pins
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  digitalWrite(S0, HIGH); // Set frequency scaling to 20%
  digitalWrite(S1, LOW);

  // Connect to WiFi
  connectToWiFi();
}

void loop() {
  Serial.println("Measuring RGB values...");

  // Read raw color values
  int red = readColor(LOW, LOW);     // Red
  int green = readColor(HIGH, HIGH); // Green
  int blue = readColor(LOW, HIGH);   // Blue

  // Display RGB values in Serial Monitor
  Serial.println("Raw RGB Values:");
  Serial.print("Red: "); Serial.println(red);
  Serial.print("Green: "); Serial.println(green);
  Serial.print("Blue: "); Serial.println(blue);

  // Send raw RGB values to ThingSpeak
  sendToThingSpeak(red, green, blue);

  delay(15000); // Wait 15 seconds (ThingSpeak API limit)
}

// Function to read color values from TCS3200
int readColor(bool S2State, bool S3State) {
  digitalWrite(S2, S2State);
  digitalWrite(S3, S3State);
  return pulseIn(OUT, LOW);
}

// Function to connect to WiFi
void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi.");
}

// Function to send data to ThingSpeak
void sendToThingSpeak(int r, int g, int b) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = server;
    url += "/update?api_key=" + apiKey + "&field1=" + String(r) + "&field2=" + String(g) + "&field3=" + String(b);

    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully.");
    } else {
      Serial.print("Error sending data: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}