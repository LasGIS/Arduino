#include "Robot.h"

#define ANGLE_FACTOR 0.583

#define ROBOT_COMMANDS_BUF_SIZE 10
#define GEAR 5

static RobotCommand robotCommands[ROBOT_COMMANDS_BUF_SIZE];
/** последняя записанная команда. */
static uint8_t lastRobotCommand = -1;
/** первая команда, готовая к выполнению. */
static uint8_t firstRobotCommand = -1;
/** передача от 0 до 5 */
static uint8_t gear = GEAR;

/** получаем ссылку на команду для заполнения. */
RobotCommand* getRobotCommand4Load() {
  int index = lastRobotCommand; index++;
  if (index >= ROBOT_COMMANDS_BUF_SIZE) index = 0;
  lastRobotCommand = index;
  Serial.print("lastRobotCommand = ");
  Serial.print(lastRobotCommand, DEC);
  Serial.println("; ");
  return &robotCommands[lastRobotCommand];
}

/** получаем ссылку на первую команду, готовую к выполнению. */
RobotCommand* getRobotCommand4Run() {
  int index = firstRobotCommand; index++;
  if (index >= ROBOT_COMMANDS_BUF_SIZE) index = 0;
  if (robotCommands[index].state == LOADED) {
    firstRobotCommand = index;
    return &robotCommands[firstRobotCommand];
  } else {
//    Serial.print("index = ");
//    Serial.print(index, DEC);
/*
    Serial.print(" robotCommands[index].state = ");
    Serial.println(robotCommands[index].state, DEC);
*/
  }
  return NULL;
}

/** останавливаем всё */
void robotStop() {
  lastRobotCommand = -1;
  firstRobotCommand = -1;
  robotCommands[0].state = EMPTY;
}

/** разбираем строку в команду */
void addRobotCommand(char buf[], int len) {
  int cnt = 0;
  RobotCommandType type;
  if (buf[0] == 'f') {
    if (buf[1] == 'l') {
      type = MOTOR_FORWARD_LEFT;
      cnt = 2;
    } else if (buf[1] == 'r') {
      type = MOTOR_FORWARD_RIGHT;
      cnt = 2;
    } else {
      type = MOTOR_FORWARD;
      cnt = 1;
    }
  } else if (buf[0] == 'b') {
    if (buf[1] == 'l') {
      type = MOTOR_BACKWARD_LEFT;
      cnt = 2;
    } else if (buf[1] == 'r') {
      type = MOTOR_BACKWARD_RIGHT;
      cnt = 2;
    } else {
      type = MOTOR_BACKWARD;
      cnt = 1;
    }
  } else if (buf[0] == 'l') {
    type = MOTOR_LEFT;
    cnt = 1;
  } else if (buf[0] == 'r') {
    type = MOTOR_RIGHT;
    cnt = 1;
  } else if (buf[0] == 'g') {
    type = MOTOR_SET_GEAR;
    cnt = 1;
  } else if (strcmp(buf, "run") == 0) {
    addRobotCommand(ROBOT_ANALYSE, 0);
    return;
  } else if (buf[0] == 's') { //
    addRobotCommand(ROBOT_SCANNING, 0);
    return;
  }
  if (cnt > 0 && len > cnt) {
    addRobotCommand(type, atoi(buf + cnt));
  }
  return;
}

void addRobotCommand(RobotCommandType type, int param) {
  RobotCommand* command = getRobotCommand4Load();
  command->type = type;
  command->param = param;
  command->state = LOADED;
}

/** выполняем команду */
void action(RobotCommand* command) {
  command->state = RUNNING;
  Serial.print((long) command, HEX);
  Serial.print(" - command(");
  Serial.print(command->type);
  Serial.print(")->param = ");
  Serial.println(command->param);
  noInterrupts();
  switch (command->type) {
    case MOTOR_FORWARD:
      twoMotor.leftMotorStart(gear, (int) (command->param));
      twoMotor.rightMotorStart(gear, (int) (command->param));
      break;
    case MOTOR_BACKWARD:
      twoMotor.leftMotorStart(-gear, (int) (command->param));
      twoMotor.rightMotorStart(-gear, (int) (command->param));
      break;
    case MOTOR_FORWARD_LEFT:
      twoMotor.rightMotorStart(gear, (int) (command->param * ANGLE_FACTOR));
      break;
    case MOTOR_FORWARD_RIGHT:
      twoMotor.leftMotorStart(gear, (int) (command->param * ANGLE_FACTOR));
      break;

    case MOTOR_LEFT:
      twoMotor.rightMotorStart(gear, (int) (command->param * ANGLE_FACTOR / 2));
      twoMotor.leftMotorStart(-gear, (int) (command->param * ANGLE_FACTOR / 2));
      break;
    case MOTOR_RIGHT:
      twoMotor.rightMotorStart(-gear, (int)(command->param * ANGLE_FACTOR / 2));
      twoMotor.leftMotorStart(gear, (int) (command->param * ANGLE_FACTOR / 2));
      break;
    case MOTOR_BACKWARD_LEFT:
      twoMotor.leftMotorStart(-gear, (int) (command->param * ANGLE_FACTOR));
      break;
    case MOTOR_BACKWARD_RIGHT:
      twoMotor.rightMotorStart(-gear, (int) (command->param * ANGLE_FACTOR));
      break;

    case MOTOR_SET_GEAR:
    case ROBOT_SCANNING:
    case ROBOT_ANALYSE:
    case ROBOT_STOP:
      break;
  }
  interrupts();
  switch (command->type) {
    case MOTOR_FORWARD:
    case MOTOR_BACKWARD:
    case MOTOR_FORWARD_LEFT:
    case MOTOR_FORWARD_RIGHT:
    case MOTOR_LEFT:
    case MOTOR_RIGHT:
    case MOTOR_BACKWARD_LEFT:
    case MOTOR_BACKWARD_RIGHT:
      break;

    case MOTOR_SET_GEAR:
      gear = (int) command->param;
      break;
    case ROBOT_SCANNING: // сканирование обстановки
      scanSituation();
      break;
    case ROBOT_ANALYSE: // анализ ситуации и принятие решений
      robotAnalyse();
      break;
    case ROBOT_STOP: // останавливаем всё
      robotStop();
      break;
  }

  twoMotor.waitBusy();
  command->state = EMPTY;
}
