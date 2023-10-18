#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

float Temp;
float Hum;
DHT dht14(D4,DHT11);


void setup() {
Temp = 0;
Hum = 0;
Serial.begin(9600);
dht14.begin();
lcd.begin();

}

void loop() {
  Hum = (dht14.readHumidity());
  Temp = (dht14.readTemperature( ));

  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("Temp: ");
  lcd.print(Temp);
  lcd.print(" C");

  lcd.setCursor(0,1);
  lcd.print("Hum: ");
  lcd.print(Hum);
  lcd.print(" %");
  delay(1000);

  Serial.print("Temp :");
  Serial.print(Temp);
  Serial.print(" Hum: ");
  Serial.println(Hum);

  delay(1000);

}