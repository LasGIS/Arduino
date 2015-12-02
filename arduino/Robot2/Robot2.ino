#include <Servo.h>
#include "MotorShield.h"
//#include "DigitPanel.h"
//#include "DHT.h"

Servo hSer;
Servo vSer;
MotorShield mShield;

// Пин подключен к сервоприводу
const int motorPin = 5;

void setup() {
  Serial.begin(9600);
  hSer.attach(motorPin);
  vSer.attach(motorPin);
}

void loop() {
}

void serialEvent() {
  char buf[10];
  int cnt = Serial.readBytes(buf, 10) - 1;
  if (cnt >= 0 && cnt < 10) {
    buf[cnt] = 0;
  }
  panel.setValue(String(buf));
  Serial.println("\"" + panel.getValue() + "\"");
  boolean isDigits = true;
  for (int i = 0; i < cnt; i++) {
    if (!isDigit(buf[i])) {
      isDigits = false;
    }
  }
  if (isDigits) {
    int degre = panel.getValue().toInt();
    if (degre >= 0 && degre <= 180) {
      vSer.write(degre);
    }
  } else {
    shimmiDance();
  }

  delay(2000);
}

void shimmiDance() {
  int i = vSer.read();
  for (int j = 0; j < 3; j++) {
    for (i++; i <= 180; i++) {
      panel.setValue(String(i, DEC));
      vSer.write(i);
      //Serial.println(i);
      if (i % 90 == 0) {
        delay(1000);
      } else if (i % 30 == 0) {
        delay(300);
      } else {
        delay(30);
      }
    }
    for (i--; i >= 0; i--) {
      panel.setValue(String(i, DEC));
      vSer.write(i);
      //Serial.println(i);
      if (i % 90 == 0) {
        delay(1000);
      } else if (i % 30 == 0) {
        delay(300);
      } else {
        delay(30);
      }
    }
  }
}

