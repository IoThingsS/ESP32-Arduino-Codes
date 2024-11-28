#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "abcd";       // Wi-Fi SSID (network name)
const char* password = "1234567@21"; // Wi-Fi password

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Start connecting to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  // Wait until the ESP32 is connected to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Connection successful
  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print ESP32's IP address
}

void loop() {
  // Optional: Blink an LED to indicate Wi-Fi is connected
  delay(1000);
}
