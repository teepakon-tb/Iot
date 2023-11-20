#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

const char* ssid = "iPhone ของ Bank";
const char* password = "11111111";
const char* mqtt_server = "172.20.10.4";

const int ledPin = D6; 

WiFiClient espClient;
PubSubClient client(espClient);

float Temp;
float Hum;
bool isLedOn = false;

DHT dht(D4, DHT11);

unsigned long lastLoopTime = 0;
const unsigned long loopInterval = 5000;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Subscribe to the LED control topic
      client.subscribe("led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - lastLoopTime >= loopInterval) {
    Temp = dht.readTemperature();
    Hum = dht.readHumidity();

    IPAddress localIP = WiFi.localIP();

    StaticJsonDocument<200> doc;
    doc["temperature"] = Temp;
    doc["humidity"] = Hum;
    doc["ip"] = localIP.toString();

    String jsonStr;
    serializeJson(doc, jsonStr);

    client.publish("dht11", jsonStr.c_str());
    delay(5000);
    
    lastLoopTime = currentMillis;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == "led") {
    if (message == "on") {
      digitalWrite(ledPin, HIGH);
      isLedOn = true;
    } else if (message == "off") {
      digitalWrite(ledPin, LOW);
      isLedOn = false;
    }
    // Send LED status to "led/status" topic
    client.publish("led/status", isLedOn ? "on" : "off");
  }
}
