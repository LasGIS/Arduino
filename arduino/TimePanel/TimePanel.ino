#include <MsTimer2.h>

String panelValue = "";

void setup() {
  Serial.begin(9600);
  initPanel();
  MsTimer2::set(5, showPanel);
  MsTimer2::start();
}

void loop() {
  for (double val = -11.0; val < 11.0; val += .01) {
    panelValue = String(val, 2);
    Serial.print(val);
    Serial.println(" -> " + panelValue);
    delay(100);
  }
  panelValue = "B.0.b.a.";
  delay(10000);
}

