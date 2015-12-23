#define LEFT_FORWARD_FACTOR 30.0
#define RIGHT_FORWARD_FACTOR 30.0
#define LEFT_BACKWARD_FACTOR 30.0
#define RIGHT_BACKWARD_FACTOR 30.0
#define ANGLE_FACTOR 0.225

#define ROBOT_COMMANDS_BUF_SIZE 10

static RobotCommand robotCommands[ROBOT_COMMANDS_BUF_SIZE];
/** последняя записанная команда. */
static uint8_t lastRobotCommand = -1;
/** первая команда, готовая к выполнению. */
static uint8_t firstRobotCommand = -1;

/** получаем ссылку на команду для заполнения. */
RobotCommand* getLastRobotCommand() {
  int index = lastRobotCommand; index++;
  if (index >= ROBOT_COMMANDS_BUF_SIZE) index = 0;
  lastRobotCommand = index;
  return &robotCommands[lastRobotCommand];
}

/** получаем ссылку на первая команда, готовую к выполнению. */
RobotCommand* getFirstRobotCommand() {
  int index = firstRobotCommand; index++;
  if (index >= ROBOT_COMMANDS_BUF_SIZE) index = 0;
  if (!robotCommands[index].isDone) {
    firstRobotCommand = index;
    return &robotCommands[firstRobotCommand];
  }
  return NULL;
}

/** разбираем строку в команду */
RobotCommand* addRobotCommand(char buf[], int len) {
  RobotCommand* command = getLastRobotCommand();
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
    command->isDone = false;
    return command;
  } else if (buf[0] == 'r') {
    command->type = MOTOR_RIGHT;
    cnt = 1;
  } else if (buf[0] == 's') { // 
    command->type = ROBOT_SCANING;
    command->isDone = false;
    return command;
  }
  if (cnt > 0 && len > cnt) {
    command->param = atoi(buf + cnt);
    command->isDone = false;
    return command;
  }
  return NULL;
}

void action(RobotCommand* command) {
  //Serial.println(mShield.motorMask, BIN);
  mShield.waitBusy();
  switch (command->type) {
    case MOTOR_FORWARD:
      mShield.leftMotor(255, (int) (command->param * LEFT_FORWARD_FACTOR));
      mShield.rightMotor(255, (int) (command->param * RIGHT_FORWARD_FACTOR));
      break;
    case MOTOR_BACKWARD:
      mShield.leftMotor(-255, (int) (command->param * LEFT_BACKWARD_FACTOR));
      mShield.rightMotor(-255, (int) (command->param * RIGHT_BACKWARD_FACTOR));
      break;
    case MOTOR_FORWARD_LEFT:
      mShield.rightMotor(255, (int) (command->param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR));
      break;
    case MOTOR_FORWARD_RIGHT:
      mShield.leftMotor(255, (int) (command->param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR));
      break;

    case MOTOR_LEFT:
      mShield.rightMotor(255, (int) (command->param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR / 2.));
      mShield.leftMotor(-255, (int) (command->param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR / 2.));
      break;
    case MOTOR_RIGHT:
      mShield.rightMotor(-255, (int) (command->param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR / 2.));
      mShield.leftMotor(255, (int) (command->param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR / 2.));
      break;
    case MOTOR_BACKWARD_LEFT:
      mShield.leftMotor(-255, (int) (command->param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR));
      break;
    case MOTOR_BACKWARD_RIGHT:
      mShield.rightMotor(-255, (int) (command->param * RIGHT_FORWARD_FACTOR * ANGLE_FACTOR));
      break;
    case ROBOT_SCANING: // сканирование обстановки
      shimmiDance();
      break;
    case ROBOT_ANALYSE: // анализ ситуации и принятие решений
      robotAnalyse();
      break;
  }
  command->isDone = true;
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
