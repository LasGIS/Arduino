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
RobotCommand* getRobotCommand4Load() {
  int index = lastRobotCommand; index++;
  if (index >= ROBOT_COMMANDS_BUF_SIZE) index = 0;
  lastRobotCommand = index;
  Serial.print("lastRobotCommand = ");
  Serial.print(lastRobotCommand, DEC);
  Serial.print("; ");
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
  } else if (strcmp(buf, "run") == 0) {
    addRobotCommand(ROBOT_ANALYSE, 0);
    return;
  } else if (buf[0] == 'r') {
    type = MOTOR_RIGHT;
    cnt = 1;
  } else if (buf[0] == 's') { // 
    addRobotCommand(ROBOT_SCANING, 0);
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
  Serial.println((long) command, HEX);
  Serial.println(mShield.motorMask, BIN);
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
      headMovement();
      break;
    case ROBOT_ANALYSE: // анализ ситуации и принятие решений
      robotAnalyse();
      break;
  }
  mShield.waitBusy();
  command->state = EMPTY;
}
