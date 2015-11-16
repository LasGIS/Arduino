#include <IrTest2.h>

// указываем пин для ИК датчика 
IrTest2 test(2);

void setup() {
  Serial.begin(9600);
  test.start();
}

void loop() {
  delay(100);
}

void serialEvent() {
  char buf[10];
  int cnt = Serial.readBytes(buf, 10) - 1;
  if (cnt >= 0 && cnt < 10) {
    buf[cnt] = 0;
  }
  Serial.println(String("[") + buf + "]");
  //test.print();
  delay(1000);
}
