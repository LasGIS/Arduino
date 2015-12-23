#define LEFT_FORWARD_FACTOR 30.0
#define RIGHT_FORWARD_FACTOR 30.0
#define LEFT_BACKWARD_FACTOR 30.0
#define RIGHT_BACKWARD_FACTOR 30.0
#define ANGLE_FACTOR 0.225

static RobotCommand robotCommands[10];
static int currenRobotCommand = 0;

/** разбираем строку в команду */
RobotCommand* addRobotCommand(char buf[], int len) {
  RobotCommand* command = new RobotCommand();
  int cnt = 0;
  if (buf[0] == 'f') {
    if (buf[1] == 'l') {
      command->type = MOTOR_FORWARD_LEFT;
      cnt = 2;
    } else if (buf[1] == 'r') {
      command->type = MOTOR_FORWARD_RIGHT;
      cnt = 2;
    } else {
      command->type = MOTOR_FORWARD;
      cnt = 1;
    }
  } else if (buf[0] == 'b') {
    if (buf[1] == 'l') {
      command->type = MOTOR_BACKWARD_LEFT;
      cnt = 2;
    } else if (buf[1] == 'r') {
      command->type = MOTOR_BACKWARD_RIGHT;
      cnt = 2;
    } else {
      command->type = MOTOR_BACKWARD;
      cnt = 1;
    }
  } else if (buf[0] == 'l') {
    command->type = MOTOR_LEFT;
    cnt = 1;
  } else if (strcmp(buf, "run") == 0) {
    command->type = ROBOT_ANALYSE;
    return command;
  } else if (buf[0] == 'r') {
    command->type = MOTOR_RIGHT;
    cnt = 1;
  } else if (buf[0] == 's') { // 
    command->type = ROBOT_SCANING;
    return command;
  }
  if (cnt > 0 && len > cnt) {
    command->param = atoi(buf + cnt);
    return command;
  }
  return NULL;
}

void action(RobotCommand com) {
  //Serial.println(mShield.motorMask, BIN);
  mShield.waitBusy();
  switch (com.type) {
    case MOTOR_FORWARD:
      mShield.leftMotor(255, (int) (com.param * LEFT_FORWARD_FACTOR));
      mShield.rightMotor(255, (int) (com.param * RIGHT_FORWARD_FACTOR));
      break;
    case MOTOR_BACKWARD:
      mShield.leftMotor(-255, (int) (com.param * LEFT_BACKWARD_FACTOR));
      mShield.rightMotor(-255, (int) (com.param * RIGHT_BACKWARD_FACTOR));
      break;
    case MOTOR_FORWARD_LEFT:
      mShield.rightMotor(255, (int) (com.param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR));
      break;
    case MOTOR_FORWARD_RIGHT:
      mShield.leftMotor(255, (int) (com.param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR));
      break;

    case MOTOR_LEFT:
      mShield.rightMotor(255, (int) (com.param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR / 2.));
      mShield.leftMotor(-255, (int) (com.param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR / 2.));
      break;
    case MOTOR_RIGHT:
      mShield.rightMotor(-255, (int) (com.param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR / 2.));
      mShield.leftMotor(255, (int) (com.param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR / 2.));
      break;
    case MOTOR_BACKWARD_LEFT:
      mShield.leftMotor(-255, (int) (com.param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR));
      break;
    case MOTOR_BACKWARD_RIGHT:
      mShield.rightMotor(-255, (int) (com.param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR));
      break;
    case ROBOT_SCANING: // сканирование обстановки
      shimmiDance();
      break;
    case ROBOT_ANALYSE: // анализ ситуации и принятие решений
      robotAnalyse();
      break;
  }
}

/**
 *
 */
void shimmiDance() {
  int i = vSer.read();
  for (i++; i <= 180; i++) {
    vSer.write(i);
    delay(10);
  }
  for (i--; i >= 0; i--) {
    vSer.write(i);
    if (i % 10 == 0) {
      showDistance(i);
    } else {
      delay(10);
    }
  }
  for (i++; i <= 180; i++) {
    vSer.write(i);
    if (i % 10 == 0) {
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

/**
 *
 */
void robotAnalyse() {
  
}
