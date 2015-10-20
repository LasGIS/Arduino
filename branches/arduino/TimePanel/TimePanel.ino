#include <MsTimer2.h>

String panelValue = "";

void setup() {
  Serial.begin(9600);
  initPanel();
  MsTimer2::set(5, showPanel);
  MsTimer2::start();
}

void loop() {
  panelValue = timeToString();
  delay(100);
}

String timeToString() {
  unsigned long time = millis();
  time /= 1000;
  int sec = time % 60;
  time /= 60;
  int min = time % 60;
  time /= 60;
  int hour = time;
  /*
  Serial.print("hour = ");
  Serial.print(hour);
  Serial.print("; min = ");
  Serial.print(min);
  Serial.print("; sec = ");
  Serial.print(sec);
  Serial.println(";");
  */
  if (hour > 0) {
    return toTwo(hour) + "." + toTwo(min);
  } else {
    return toTwo(min) + "." + toTwo(sec);
  }
}

String toTwo(int val) {
  String str = "00";
  str = str + val;
  return str.substring(str.length() - 2);
}

/*
  for (double val = -11.0; val < 11.0; val += .01) {
    panelValue = String(val, 2);
    Serial.print(val);
    Serial.println(" -> " + panelValue);
    delay(100);
  }
  panelValue = "B.0.b.a.";
  delay(10000);
*/

