#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Wi-Fi credentials
const char* ssid = "abcde";        // Replace with your Wi-Fi SSID
const char* password = "1234567@21"; // Replace with your Wi-Fi password

// DHT Sensor configuration
#define DHTPIN 4         // GPIO pin connected to DHT11
#define DHTTYPE DHT11    // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);

// IoThingsS API endpoint and key
const char* serverName = "https://api.iothings.co.in/api/v1/devicedata";
const char* apiKey = "1d2246c4-bcaf-4458-b454-83b318adf4e3";

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
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if sensor readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(5000); // Retry after 5 seconds
    return;
  }

  // Log sensor data
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Send data to IoThingsS Cloud
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Specify the URL
    http.begin(serverName);

    // Set headers
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    String jsonPayload = String("{\"key\":\"") + apiKey +
                         "\",\"temp\":\"" + temperature +
                         "\",\"humidity\":\"" + humidity + "\"}";

    // Print payload for debugging
    Serial.println("Sending payload: " + jsonPayload);

    // Send POST request
    int httpResponseCode = http.POST(jsonPayload);

    // Handle server response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("Response: " + response);
    } else {
      Serial.print("Error in sending POST: ");
      Serial.println(httpResponseCode);
    }

    // End HTTP connection
    http.end();
  } else {
    Serial.println("Wi-Fi not connected!");
  }

  // Wait before sending data again
  delay(60000); // Send data every 60 seconds
}
