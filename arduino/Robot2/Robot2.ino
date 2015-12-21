#include <Servo.h>
//#include <LiquidCrystal_I2C.h>
#include "MotorShield.h"
#include "RobotCommand.h"

Servo hSer;
Servo vSer;
MotorShield mShield(MSHLD_M1, MSHLD_M4);
//LiquidCrystal_I2C lcd(0x3F, 16, 2);

/* пины Ультразвукового дальномера */
int echoPin = A3; 
int trigPin = A2;

void setup() {
  Serial.begin(9600);
/*
  // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  lcd.print("Start Robo 1.0.1");
*/
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
  char buf[50];
  int cnt = Serial.readBytesUntil(';', buf, 50) - 1;
  if (cnt >= 0 && cnt < 50) {
    buf[cnt] = 0;
  }
  Serial.print("\"");
  Serial.print(buf);
  Serial.print("\"");
  RobotCommand* command = addRobotCommand(buf, cnt);
  if (command != NULL) {
    drive(*command);
  }
}

void drive(RobotCommand com) {
  //Serial.println(mShield.motorMask, BIN);
  mShield.waitBusy();
  switch (com.type) {
    case MOTOR_FORWARD:
      mShield.leftMotor(255, 1000);
      mShield.rightMotor(255, 1000);
      break;
    case MOTOR_BACKWARD:
      mShield.leftMotor(-255, 1000);
      mShield.rightMotor(-255, 1000);
      break;
    case MOTOR_FORWARD_LEFT:
      mShield.rightMotor(5, 300);
      break;
    case MOTOR_FORWARD_RIGHT:
      mShield.leftMotor(5, 300);
      break;

    case MOTOR_LEFT:
      mShield.rightMotor(5, 150);
      mShield.leftMotor(-5, 150);
      break;
    case MOTOR_RIGHT:
      mShield.rightMotor(-5, 150);
      mShield.leftMotor(5, 150);
      break;
    case MOTOR_BACKWARD_LEFT:
      mShield.leftMotor(-5, 300);
      break;
    case MOTOR_BACKWARD_RIGHT:
      mShield.rightMotor(-5, 300);
      break;
/*
    case ROBOT_SCANING: // сканирование обстановки
      shimmiDance();
      break;
*/
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
      Serial.println("Left MOTOR");
      for (int gear = 5; gear >= -5; gear--) {
        mShield.waitBusy();
        mShield.leftMotor(gear, 1000);
        Serial.print("gear = ");
        Serial.println(gear);
//        delay(1010);
      }
      break;
    
    case 'r':
      Serial.println("Right MOTOR");
      for (int gear = 5; gear >= -5; gear--) {
        mShield.waitBusy();
        mShield.rightMotor(gear, 1000);
        Serial.print("gear = ");
        Serial.println(gear);
//        delay(1010);
      }
      break;
    
    case 's':
      shimmiDance();
      break;
  }
  mShield.stopMotors();
}

/** Скорость от передачи (5 передача - самая высокая) */
static const int MSHLD_GEAR_SPEED[] = {-255, -170, -113, 0, 113, 170, 255};

/**
 *
 */
void testDriveMotor(int motoNum) {
  Serial.print("M");
  Serial.print(motoNum + 1);
  Serial.println(" test drive");
  for (int i = 0; i < 7; i++) {
    mShield.waitBusy();
    int speed = MSHLD_GEAR_SPEED[i];
    mShield.motor(motoNum, speed, 1000);
    Serial.print("speed = ");
    Serial.println(speed);
    //delay(1010);
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

