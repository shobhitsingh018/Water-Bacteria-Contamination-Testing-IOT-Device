#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ThingSpeak.h>

// Wi-Fi credentials
const char* ssid = "SHOBHIT";
const char* password = "bunny1234";

// ThingSpeak API credentials
unsigned long channelID = 2777484;  // Replace with your channel ID
const char* writeAPIKey = "A4C2JUQ4L49A14WD";

// TCS34725 setup
Adafruit_TCS34725 tcs;

unsigned long lastSendTime = 0;
unsigned long interval = 1 * 60 * 1000; // 3 minutes interval

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");

  // Set up time with the correct timezone for New Delhi (UTC +5:30)
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov"); // UTC+5:30 is 19800 seconds ahead

  if (tcs.begin()) {
    Serial.println("TCS34725 Initialized");
  } else {
    Serial.println("TCS34725 not found. Check connections.");
    while (1);
  }
}

void loop() {
  if (millis() - lastSendTime >= interval) {
    lastSendTime = millis();

    // Fetch real-time timestamp
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);

    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c);
    float colorTemp = tcs.calculateColorTemperature(r, g, b);
    float lux = tcs.calculateLux(r, g, b);

    // Print RGB values along with timestamp and lux
    Serial.print("Timestamp: ");
    Serial.println(timestamp);
    Serial.print("RGB Values: R=");
    Serial.print(r);
    Serial.print(" G=");
    Serial.print(g);
    Serial.print(" B=");
    Serial.print(b);
    Serial.print(" C=");
    Serial.println(c);
    Serial.print("Color Temp: ");
    Serial.print(colorTemp);
    Serial.println(" K");
    Serial.print("Lux: ");
    Serial.println(lux);

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://api.thingspeak.com/update";
      url += "?api_key=" + String(writeAPIKey);
      url += "&field1=" + String(r);  // Red value
      url += "&field2=" + String(g);  // Green value
      url += "&field3=" + String(b);  // Blue value
      url += "&field4=" + String(c);  // Clear value
      url += "&field5=" + String(colorTemp);
      url += "&field6=" + String(lux);
      url += "&created_at=" + String(timestamp); // Add timestamp

      http.begin(url);
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.println("Data sent to ThingSpeak successfully");
      } else {
        Serial.print("Error sending data: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}