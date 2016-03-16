#include <Arduino.h>
#include <Servo.h>
#include "TwoMotor.h"
#include "RobotCommand.h"

Servo hSer;
Servo vSer;
TwoMotor twoMotor;

/* пины Ультразвукового дальномера */
int echoPin = A3; 
int trigPin = A2;

extern void leftSpeedometrInterrupt(void);
extern void rightSpeedometrInterrupt(void);

/** пин кнопочки
int buttonPin = 2; 
bool isButtonPressed = false;
bool isAutorun = false;
*/

void setup() {
  Serial.begin(9600);
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
      robotStop();
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
  if (!twoMotor.isBusy()) {
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

void testDrive(char motor) {
  int speed = -3;
  Serial.println(motor);
  switch (motor) {

  case 'l':
      Serial.println("Left MOTOR");
      twoMotor.waitBusy();
//      for (int speed = 5; speed >= -5; speed--) {
        twoMotor.leftMotorStart(speed, 2200);
        Serial.print("speed = ");
        Serial.println(speed);
        while (twoMotor.isBusy()) {
          delay(100);
        }
//      }
      break;
    
    case 'r':
      Serial.println("Right MOTOR");
      twoMotor.waitBusy();
//      for (int speed = 5; speed >= -5; speed--) {
        twoMotor.rightMotorStart(speed, 2200);
        Serial.print("speed = ");
        Serial.println(speed);
        while (twoMotor.isBusy()) {
          delay(100);
        }
//      }
      break;
    
    case 's':
      scanSituation();
      break;
  }
  twoMotor.stopMotors();
}
