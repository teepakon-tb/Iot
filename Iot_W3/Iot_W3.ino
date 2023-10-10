int x = 0;
float y_sin = 0.0;
float y_cos = 0.0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  x = x + 1;
  y_sin = sin(x * 0.5);  
  y_cos = cos(x * 0.5);  
  
  // ปรับค่าให้อยู่ในช่วง -3 ถึง 3
  y_sin = (y_sin * 1);
  y_cos = (y_cos * 1);

  Serial.print("sin:");
  Serial.print(y_sin);
  Serial.print(" cos:");
  Serial.println(y_cos);

  // เพิ่มเส้นตรงที่ y = 0
  Serial.print("zero:");
  Serial.println(0);
  
  delay(10); 
}
