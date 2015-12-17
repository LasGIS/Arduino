#include <Servo.h>
#include "MotorShield.h"
#include <LiquidCrystal_I2C.h>

Servo hSer;
Servo vSer;
MotorShield mShield(MSHLD_M1, MSHLD_M4);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

/* пины Ультразвукового дальномера */
int echoPin = A3; 
int trigPin = A2; 

void setup() {
  Serial.begin(9600);
  // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  lcd.print("Start Robo 1.0.1");
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

  hSer.attach(MSHLD_PWM1A_PIN);
  vSer.attach(MSHLD_PWM1B_PIN);
  // testDrive('1');
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
  if (buf[0] == 'T') {
    for (int i = 1; i < cnt; i++) {
      testDrive(buf[i]);
    }
  } else if (buf[0] == 'D') {
    for (int i = 1; i < cnt; i++) {
      drive(buf[i]);
    }
  }
/*   
    if (degre >= 0 && degre <= 180) {
      vSer.write(degre);
    } else {
//    shimmiDance();
    }
*/
//  delay(1000);
}

void drive(char command) {
  switch (command) {
    case 'f':
      mShield.leftMotor(5, 1000);
      mShield.rightMotor(5, 1000);
      delay(1000);
      break;
    case 'b':
      mShield.leftMotor(-5, 1000);
      mShield.rightMotor(-5, 1000);
      delay(1000);
      break;
    case 'r':
      mShield.leftMotor(5, 300);
      //mShield.rightMotor(3, 300);
      delay(300);
      break;
    case 'l':
      //mShield.leftMotor(3, 300);
      mShield.rightMotor(5, 300);
      delay(300);
      break;
    case 's': // сканирование обстановки
      shimmiDance();
      break;
  }
}

void testDrive(char motor) {
  Serial.println(motor);
  switch (motor) {
    case '1':
      testDriveMotor(MSHLD_M1);
      break;
    
    case '2':
      testDriveMotor(MSHLD_M2);
      break;
    
    case '3':
      testDriveMotor(MSHLD_M3);
      break;
    
    case '4':
      testDriveMotor(MSHLD_M4);
      break;
    
    case 'l':
      Serial.println("левый мотор");
      for (int gear = 5; gear >= -5; gear--) {
        mShield.leftMotor(gear, 1000);
        Serial.print("gear = ");
        Serial.println(gear);
        delay(1100);
      }
      break;
    
    case 'r':
      Serial.println("правый мотор");
      for (int gear = 5; gear >= -5; gear--) {
        mShield.rightMotor(gear, 1000);
        Serial.print("gear = ");
        Serial.println(gear);
        delay(1100);
      }
      break;
    
    case 's':
      shimmiDance();
      break;
  }
  mShield.stopMotors();
}

/**
 *
 */
void testDriveMotor(int motoNum) {
  Serial.print("M");
  Serial.print(motoNum + 1);
  Serial.println(" test drive");
  for (int gear = 5; gear >= -5; gear--) {
    mShield.motor(motoNum, gear, 1000);
    Serial.print("gear = ");
    Serial.println(gear);
    delay(1100);
  }
}

#define WAIT_TIME_DISTANCE 100

void shimmiDance() {
  int i = vSer.read();
  for (i++; i <= 180; i++) {
    vSer.write(i);
    delay(10);
  }
  for (i--; i >= 0; i--) {
    vSer.write(i);
    if (i % 10 == 0) {
      delay(WAIT_TIME_DISTANCE);
      showDistance(i);
    } else {
      delay(10);
    }
  }
  for (i++; i <= 180; i++) {
    vSer.write(i);
    if (i % 10 == 0) {
      delay(WAIT_TIME_DISTANCE);
      showDistance(i);
    } else {
      delay(10);
    }
  }
  for (i--; i >= 90; i--) {
    vSer.write(i);
    delay(10);
  }
}

