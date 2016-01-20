#include <Servo.h>
//#include <LiquidCrystal_I2C.h>
#include "MotorShield.h"
#include "RobotCommand.h"

Servo hSer;
Servo vSer;
MotorShield mShield(MSHLD_M1, MSHLD_M2);
//LiquidCrystal_I2C lcd(0x3F, 16, 2);

/* пины Ультразвукового дальномера */
int echoPin = A3; 
int trigPin = A2;

/** пин кнопочки
int buttonPin = 2; 
bool isButtonPressed = false;
bool isAutorun = false;
*/

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
//  pinMode(buttonPin, INPUT); 

  hSer.attach(MSHLD_PWM1A_PIN);
  vSer.attach(MSHLD_PWM1B_PIN);
  headToDegree(90);
  // testDrive('1');
  // addRobotCommand(ROBOT_ANALYSE, 0);
}

void loop() {
/*  
  static long buttonPressTime = 1000000000l;
  if (digitalRead(buttonPin)) {
    isButtonPressed = true;
    buttonPressTime = millis();
  } else if (isButtonPressed
    && (millis() - buttonPressTime > 100)
    && !digitalRead(buttonPin)
  ) {
    if (isAutorun) {
      addRobotCommand(ROBOT_STOP, 0);
      Serial.println("ROBOT_STOP");
      isAutorun = false;
    } else {
      addRobotCommand(ROBOT_ANALYSE, 0);
      Serial.println("ROBOT_ANALYSE");
      isAutorun = true;
    }
    isButtonPressed = false;
  }
*/
  if (!mShield.isBusy()) {
    RobotCommand* command = getRobotCommand4Run();
    if (command != NULL) {
      action(command);
    }
  }
  delay(20);
}

void serialEvent() {
  char buf[50];
  int cnt = Serial.readBytesUntil(';', buf, 50);
  if (cnt >= 0 && cnt < 50) {
    buf[cnt] = 0;
  }
  Serial.print('"');
  Serial.print(buf);
  Serial.println('"');
  if (buf[0] == 't') {
    for (int i = 1; i < cnt; i++) {
      testDrive(buf[i]);
    }
  }
//  RobotCommand* command = 
  addRobotCommand(buf, cnt);
/*  if (command != NULL) {
    action(command);
  }*/
}

/** Скорость от передачи (5 передача - самая высокая) */
static const int MSHLD_GEAR_SPEED[] = {-255, -170, -113, 0, 113, 170, 255};

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
      for (int i = 0; i < 7; i++) {
        mShield.waitBusy();
        int speed = MSHLD_GEAR_SPEED[i];
        mShield.leftMotor(speed, 1000);
        Serial.print("speed = ");
        Serial.println(speed);
//        delay(1010);
      }
      break;
    
    case 'r':
      Serial.println("Right MOTOR");
      for (int i = 0; i < 7; i++) {
        mShield.waitBusy();
        int speed = MSHLD_GEAR_SPEED[i];
        mShield.rightMotor(speed, 1000);
        Serial.print("speed = ");
        Serial.println(speed);
//        delay(1010);
      }
      break;
    
    case 's':
      scanSituation();
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
  for (int i = 0; i < 7; i++) {
    mShield.waitBusy();
    int speed = MSHLD_GEAR_SPEED[i];
    mShield.motor(motoNum, speed, 1000);
    Serial.print("speed = ");
    Serial.println(speed);
    //delay(1010);
  }
}
