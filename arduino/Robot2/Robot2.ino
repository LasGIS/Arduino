#include <Servo.h>
#include "MotorShield.h"

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
  mShield.rightMotorForward();
  delay(1000);
  mShield.rightMotorBackward();
  delay(1000);
  mShield.leftMotorForward();
  delay(1000);
  mShield.leftMotorBackward();
  delay(1000);
  mShield.rightMotorPower(256);
  delay(1000);
  mShield.leftMotorPower(256);
  delay(1000);
  mShield.stopMotor();
  delay(1000);
}

void serialEvent() {
  char buf[10];
  int cnt = Serial.readBytes(buf, 10) - 1;
  if (cnt >= 0 && cnt < 10) {
    buf[cnt] = 0;
  }
  Serial.print("\"");
  Serial.print(buf);
  Serial.print("\"");
  boolean isDigits = true;
  for (int i = 0; i < cnt; i++) {
    if (!isDigit(buf[i])) {
      isDigits = false;
    }
  }
  if (isDigits) {
    int degre = int(buf);
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

