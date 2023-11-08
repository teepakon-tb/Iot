#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <DHT.h>

const char* ssid = "iPhone ของ Bank";
const char* password = "11111111";

unsigned long channelID = 2336979;
const char* writeKey = "K6IZ6AL8RFVBQJ2C";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

WiFiClient client;

DHT dht(D4, DHT11); 

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 
  ThingSpeak.begin(client);
  dht.begin(); 
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(5000);
      }
      Serial.println("\nConnected.");
    }

    float Humidity = dht.readHumidity(); 
    float Temperature = dht.readTemperature(); 

    if (isnan(Humidity) || isnan(Temperature)) {
      Serial.println("Failed to read from DHT sensor");
    } else {

      Serial.print("Humidity");
      Serial.println(Humidity);
      Serial.print("Temperature");
      Serial.println(Temperature);   

      ThingSpeak.setField(1, Humidity);
      ThingSpeak.setField(2, Temperature);

      int x = ThingSpeak.writeFields(channelID, writeKey);
      if (x == 200) {
        Serial.println("Channel update successful.");
      } else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
    }

    lastTime = millis();
  }
}
