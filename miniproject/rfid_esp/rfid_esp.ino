#include <ESP8266WiFi.h>

const char* ssid = "FrameMeRy"; // ชื่อ Wi-Fi network
const char* password = "framemery"; // รหัสผ่าน Wi-Fi network

String incomingString;

int scanCount1 = 0;  // Counter for ID 1
int scanCount2 = 0;  // Counter for ID 2
int scanCount3 = 0;  // Counter for ID 3

void setup() {
  Serial.begin(115200); // เริ่มต้น Serial communication ที่ 115200 bps

  // เชื่อมต่อ Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize code here...
}

void loop() {
  if (Serial.available()) {
    incomingString = Serial.readString();
    processID(incomingString.charAt(0));
  }
  // Add the rest of your code here...
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

  // Display received ID and processed information
  Serial.println("Received ID: " + String(id));
  Serial.println("StdId: " + studentid);
  Serial.println("Firstname: " + firstname);
  Serial.println("Lastname: " + lastname);
  Serial.println("Position: " + jobPosition);
  Serial.println("Status: " + status);
}
