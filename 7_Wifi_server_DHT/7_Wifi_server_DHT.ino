#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Wi-Fi credentials
const char* ssid = "abcde";       // Wi-Fi SSID
const char* password = "1234567@21"; // Wi-Fi password

// DHT Sensor settings
#define DHTPIN 4         // GPIO pin where the DHT11 is connected
#define DHTTYPE DHT11    // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);

// Web server running on port 80
WiFiServer server(80);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize DHT sensor
  dht.begin();

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  // Check for incoming client requests
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client connected.");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Read temperature and humidity
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Generate HTML content
    String html = "<!DOCTYPE html><html>";
    html += "<head><title>ESP32 DHT11 Web Server</title>";
    html += "<meta http-equiv='refresh' content='2'>"; // Auto-refresh every 2 seconds
    html += "<style>body { font-family: Arial; text-align: center; margin: 0; padding: 20px; }";
    html += "h1 { color: #333; } p { font-size: 18px; }</style></head>";
    html += "<body>";
    html += "<h1>ESP32 DHT11 Web Server</h1>";
    html += "<p>Temperature: " + String(temperature) + " &deg;C</p>";
    html += "<p>Humidity: " + String(humidity) + " %</p>";
    html += "</body></html>";

    // Send the response
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    client.print(html);

    delay(1); // Allow the client to receive the response
    client.stop(); // Close the connection
    Serial.println("Client disconnected.");
  }
}
