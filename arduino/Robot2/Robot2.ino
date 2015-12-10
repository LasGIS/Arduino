#include <Servo.h>
#include "MotorShield.h"

Servo hSer;
Servo vSer;
MotorShield mShield;

void setup() {
  Serial.begin(9600);
  hSer.attach(MSHLD_PWM1A_PIN);
  vSer.attach(MSHLD_PWM1B_PIN);
}
/*
00000100
00001000
00001010
00011000
00000000
*/
void loop() {
  delay(2000);
  //shimmiDance();
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
    testDrive(buf[0]);
/*   
    if (degre >= 0 && degre <= 180) {
      vSer.write(degre);
    } else {
//    shimmiDance();
    }
*/
  }
  delay(1000);
}

void testDrive(char motor) {
  Serial.println(motor);
  switch (motor) {
    case '1':
      Serial.println("M1 test drive");
      for (int speed = 260; speed > -260; speed--) {
        mShield.setM1(speed);
        delay(10);
      }
      break;
    
    case '2':
      Serial.println("M2 test drive");
      for (int speed = 260; speed > -260; speed--) {
        mShield.setM2(speed);
        delay(10);
      }
      break;
    
    case '3':
      Serial.println("M3 test drive");
      for (int speed = 260; speed > -260; speed--) {
        mShield.setM3(speed);
        delay(10);
      }
      break;
    
    case '4':
      Serial.println("M4 test drive");
      for (int speed = 260; speed > -260; speed--) {
        mShield.setM4(speed);
        delay(10);
      }
      break;
  }
  mShield.stopMotor();
}

void shimmiDance() {
  int i = vSer.read();
  for (int j = 0; j < 3; j++) {
    for (i++; i <= 180; i++) {
      vSer.write(i);
      Serial.println(i);
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
      Serial.println(i);
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

