//#include <ESP8266HTTPClient.h>

#define PINOUT 14

void setup() {
//  Serial.begin(74880);
  Serial.begin(115200);
  pinMode(PINOUT, OUTPUT);
}

void loop() {
  static int i = 0;
  digitalWrite(PINOUT, HIGH);
  delay(500);
  digitalWrite(PINOUT, LOW);
  delay(500);
//  Serial.println(sin((i * PI) / 18)*7);
  i++;
}
/*
void serialEvent() {
  char buf[10];
  int j = 0;
  while (Serial.available() > 0) {
    int cnt = Serial.readBytes(buf, 10);
    for (int i = 0; i < cnt; i++, j++) {
      Serial.write(buf[i]);
    }
    Serial.println(" - Сам такой:");
  }
  delay(20);
}
*/