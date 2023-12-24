#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

// WiFi parameters
#define WLAN_SSID       "FrameMeRy"
#define WLAN_PASS       "framemery"

// MQTT parameters
const char* mqttServer = "172.20.10.2";
const int mqttPort = 1883;
const char* mqttTopic = "id";

String incomingString;

int scanCount1 = 0;  // Counter for ID 1
int scanCount2 = 0;  // Counter for ID 2
int scanCount3 = 0;  // Counter for ID 3

WiFiClient espClient;
PubSubClient client(espClient);

// Set your time zone here (replace 7 with your UTC offset)
const long utcOffsetInSeconds = 7 * 3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(115200);
  Serial.println(F("ESP8266 Example"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // Initialize NTP Client with time zone
  timeClient.begin();

  // Set up MQTT client
  client.setServer(mqttServer, mqttPort);
  // If your MQTT broker requires authentication, use the following line:
  // client.setCredentials(mqttUser, mqttPassword);

}

void loop() {
  if (Serial.available()) {
    incomingString = Serial.readString();
    processID(incomingString.charAt(0));
  }

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}

void processID(char id) {
  String studentid, firstname, lastname, timestamp, jobPosition, status;

  // Dummy information based on received ID
  if (id == '1') {
    studentid = "64102080";
    firstname = "จิรกิตติ์";
    lastname = "เอียดเหตุ";
    jobPosition = "นักศึกษา";
    scanCount1++;
    status = (scanCount1 % 2 == 0) ? "ออก" : "เข้า";
  } else if (id == '2') {
    studentid = "64125735";
    firstname = "ธนวัฒน์";
    lastname = "กองสีสังข์";
    jobPosition = "นักศึกษา";
    scanCount2++;
    status = (scanCount2 % 2 == 0) ? "ออก" : "เข้า";
  } else if (id == '3') {
    studentid = "64110455";
    firstname = "ภัครศักดิ์";
    lastname = "ผลสนอง";
    jobPosition = "นักศึกษา";
    scanCount3++;
    status = (scanCount3 % 2 == 0) ? "ออก" : "เข้า";
  }

  timestamp = getCurrentDateTime();

  Serial.println("ID: " + String(id));
  Serial.println("StdId: " + studentid);
  Serial.println("Firstname: " + firstname);
  Serial.println("Lastname: " + lastname);
  Serial.println("Position: " + jobPosition);
  Serial.println("Timestamp: " + timestamp);
  Serial.println("Status: " + status);

  sendToMQTT(id, studentid, firstname, lastname, jobPosition, timestamp, status);
    // Send LINE Notify
  sendToLINE(studentid, firstname, lastname, jobPosition, timestamp, status);
}



String getCurrentDateTime() {
  // Update NTP time
  timeClient.update();

  // Get current date and time
  String dateTime = timeClient.getFormattedTime();
  return dateTime;
}

void sendToMQTT(char id, String studentid, String firstname, String lastname, String jobPosition, String timestamp, String status) {
  // Create JSON payload
  String jsonPayload = "{\"uid\":\"" + String(id) + "\","
                      "\"studentid\":\"" + studentid + "\","
                      "\"firstname\":\"" + firstname + "\","
                      "\"lastname\":\"" + lastname + "\","
                      "\"position\":\"" + jobPosition + "\","
                      "\"timestamp\":\"" + timestamp + "\","
                      "\"status\":\"" + status + "\"}";

  // Publish to MQTT topic
  client.publish(mqttTopic, jsonPayload.c_str());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println(F("connected"));
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
