#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "iPhone ของ Bank";
const char* password = "11111111";

WiFiClient client;
HTTPClient http;
DHT dht(D4, DHT11); 

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 
  dht.begin(); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  static unsigned long lastTime = 0;
  if ((millis() - lastTime) > 15000) {
    float humidity = dht.readHumidity(); 
    float temperature = dht.readTemperature(); 

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor");
    } else {
      Serial.print("Humidity: ");
      Serial.println(humidity);
      Serial.print("Temperature: ");
      Serial.println(temperature);

      StaticJsonDocument<200> jsonDocument;
      jsonDocument["humidity"] = humidity;
      jsonDocument["temperature"] = temperature;
      jsonDocument["timestamp"] = String(millis() / 1000.0, 2) + "sec";

      String jsonData;
      serializeJson(jsonDocument, jsonData);

      http.begin(client, "http://172.20.10.4:3000/sensors");
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(jsonData);

      if (httpResponseCode > 0) {
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        String payload = http.getString();
        Serial.println("Returned payload:");
        Serial.println(payload);
      } else {
        Serial.println("Error on sending POST: " + String(httpResponseCode));
      }
      http.end();
    }
    lastTime = millis();
  }
}
