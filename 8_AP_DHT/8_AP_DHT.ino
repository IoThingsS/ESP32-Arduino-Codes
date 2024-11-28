#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// DHT Sensor configuration
#define DHTPIN 4         // GPIO pin connected to DHT11
#define DHTTYPE DHT11    // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);

// Hotspot credentials
const char* ssid = "ESP32_Hotspot";       // Name of the Wi-Fi network
const char* password = "12345678";        // Password for the network

// Web server on port 80
WiFiServer server(80);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Start the DHT sensor
  dht.begin();

  // Configure ESP32 as an access point
  Serial.println("Starting Hotspot...");
  WiFi.softAP(ssid, password);

  // Print IP address
  Serial.print("Access Point IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Start the web server
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  // Check if a client is connected
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client connected.");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Read sensor data
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // HTML response
    String html = "<!DOCTYPE html><html>";
    html += "<head><title>ESP32 Hotspot DHT11</title>";
    html += "<meta http-equiv='refresh' content='2'>"; // Refresh every 2 seconds
    html += "<style>body { font-family: Arial; text-align: center; margin: 0; padding: 20px; background-color: #f3f3f3; }";
    html += "h1 { color: #333; } p { font-size: 20px; }</style></head>";
    html += "<body>";
    html += "<h1>ESP32 Hotspot DHT11 Sensor</h1>";
    html += "<p>Temperature: " + String(temperature) + " &deg;C</p>";
    html += "<p>Humidity: " + String(humidity) + " %</p>";
    html += "</body></html>";

    // Send response to the client
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    client.print(html);

    delay(1); // Allow time for the client to receive data
    client.stop(); // Disconnect the client
    Serial.println("Client disconnected.");
  }
}
