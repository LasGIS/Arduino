#include <Servo.h>
#include "MotorShield.h"
#include <LiquidCrystal_I2C.h>

Servo hSer;
Servo vSer;
MotorShield mShield(MSHLD_M1, MSHLD_M4);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  Serial.begin(9600);
  // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  lcd.print("Start Robo 1.0.1");

  hSer.attach(MSHLD_PWM1A_PIN);
  vSer.attach(MSHLD_PWM1B_PIN);
  testDrive('1');
}

void loop() {
  //delay(2000);
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
      for (int gear = 5; gear >= -5; gear--) {
        mShield.motor(MSHLD_M1, gear, 1000);
        delay(100);
      }
      break;
    
    case '2':
      Serial.println("M2 test drive");
      for (int gear = 5; gear >= -5; gear--) {
        mShield.motor(MSHLD_M2, gear, 1000);
        delay(100);
      }
      break;
    
    case '3':
      Serial.println("M3 test drive");
      for (int gear = 5; gear >= -5; gear--) {
        mShield.motor(MSHLD_M3, gear, 1000);
        delay(100);
      }
      break;
    
    case '4':
      Serial.println("M4 test drive");
      for (int gear = 5; gear >= -5; gear--) {
        mShield.motor(MSHLD_M4, gear, 1000);
        delay(100);
      }
      break;
    
    case 'l':
      Serial.println("левый мотор");
      for (int gear = 5; gear >= -5; gear--) {
        mShield.leftMotor(gear, 1000);
        delay(100);
      }
      break;
    
    case 'r':
      Serial.println("правый мотор");
      for (int gear = 5; gear >= -5; gear--) {
        mShield.rightMotor(gear, 1000);
        delay(100);
      }
      break;
  }
  mShield.stopMotors();
}

/**
 *
 */
void testDrive1() {
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

