#include <IrControl.h>

// указываем пин для ИК датчика 
IrControl control(2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (control.hasCode()) {
    Serial.println("hasCode");
    //lcd.setCursor(0, 0);
    long code = control.getCode();
    char key = control.toKey(code);
    Serial.print("IR key = ");
    Serial.print(key);
    Serial.print("; code = ");
    Serial.println(control.code, HEX);
  }
  delay(100);
}

void serialEvent() {
  char buf[10];
  int cnt = Serial.readBytes(buf, 10) - 1;
  if (cnt >= 0 && cnt < 10) {
    buf[cnt] = 0;
  }
  Serial.println(String("[") + buf + "]");
  //control.print();
  delay(1000);
}
