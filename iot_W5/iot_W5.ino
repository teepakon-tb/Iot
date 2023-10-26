#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

ESP8266WebServer server(80);
DHT dht(D4, DHT11);
float Temp;
float Hum;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int ledState = LOW;

void init_wifi(String ssid, String password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  init_wifi("BANK 2.4", "tb123456");
  pinMode(D6, OUTPUT);

  dht.begin();
  lcd.begin();
  server.on("/", HTTP_GET, [&]() {
    Temp = dht.readTemperature();
    Hum = dht.readHumidity();
    String response = "<html><head><style>body{text-align:center;}</style></head><body>";
    response += "<h1>Node as Server</h1>";
    response += "<p>Temperature: <span id='temp'>" + String(Temp, 2) + "C</span><br>Humidity: <span id='hum'>" + String(Hum, 2) + "%</span></p>";
    response += "<a href='/on'><button style='background-color: blue; color: white;'>Turn On LED</button></a>";
    response += "<a href='/off'><button style='background-color: red; color: white;'>Turn Off LED</button></a>";
    response += "<script>setInterval(updateData, 1000); function updateData() { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { var data = JSON.parse(this.responseText); document.getElementById('temp').innerHTML = data.temp + 'C'; document.getElementById('hum').innerHTML = data.hum + '%'; } }; xhttp.open('GET', '/data', true); xhttp.send(); }</script>";
    response += "</body></html>";
    server.send(200, "text/html", response);
  });

  server.on("/data", HTTP_GET, []() {
    Temp = dht.readTemperature();
    Hum = dht.readHumidity();
    String data = "{\"temp\": " + String(Temp, 2) + ", \"hum\": " + String(Hum, 2) + "}";
    server.send(200, "application/json", data);
  });

  server.on("/on", HTTP_GET, []() {
    ledState = HIGH;
    digitalWrite(D6, HIGH);
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.on("/off", HTTP_GET, []() {
    ledState = LOW;
    digitalWrite(D6, LOW);
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(Temp);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(Hum);
  lcd.print(" %");
  delay(1000);
}
