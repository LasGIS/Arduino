#include <Servo.h>
#include "MotorShield.h"
#include <LiquidCrystal_I2C.h>

Servo hSer;
Servo vSer;
MotorShield mShield;
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  Serial.begin(9600);
  // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  lcd.print("Start Robo 1.0");

  hSer.attach(MSHLD_PWM1A_PIN);
  vSer.attach(MSHLD_PWM1B_PIN);
  //testDrive('1');
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
  uint8_t pinl;
  uint8_t pinr;
  int speed;
  int level;
  switch (motor) {
    case '1':
      testDrive1();
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

#define MSHLD_DEL_TIME 9
int speedMask[] = {2, 3, 5, 7, 10, 16};
/*int speedMask[] = {
  0x8080, 0x0842, 0x2492, 0xa52a, 0xdad6, 0xffff
};*/

/**
 *
 */
void testDrive1() {
  uint8_t pinl;
  uint8_t pinr;
  int speed;
  int level;
// [2|3|5|7|10|16]
  Serial.println("M1 test drive");
  pinl = mShield.setM1(255);
  pinr = mShield.setM4(255);
  for (int i = 0; i < 6; i++) {
    speed = speedMask[i];
    Serial.print("speed = ");
    Serial.println(speed, DEC);
    for (int tm = 0; tm < 50; tm++) {
//      for (int k = 0; k < 16; k++) {
        digitalWrite(pinl, HIGH);
        digitalWrite(pinr, HIGH);
        delay(MSHLD_DEL_TIME * speed);
        if (speed < 16) {
          digitalWrite(pinl, LOW);
          digitalWrite(pinr, LOW);
          delay(MSHLD_DEL_TIME * (16 - speed));
        }
//      }
    }
  }
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

