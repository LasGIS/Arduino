#include <Servo.h>
//#include <LiquidCrystal_I2C.h>
#include "MotorShield.h"
#include "RobotCommand.h"

Servo hSer;
Servo vSer;
MotorShield mShield(MSHLD_M1, MSHLD_M3);
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
  int cnt = Serial.readBytesUntil(';', buf, 50);
  if (cnt >= 0 && cnt < 50) {
    buf[cnt] = 0;
  }
  Serial.println(buf);
  if (buf[0] == 't') {
    for (int i = 1; i < cnt; i++) {
      testDrive(buf[i]);
    }
  }
  RobotCommand* command = addRobotCommand(buf, cnt);
  if (command != NULL) {
    action(command);
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
