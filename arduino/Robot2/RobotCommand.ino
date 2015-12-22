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
  } else if (buf[0] == 'r') {
    command->type = MOTOR_RIGHT;
    cnt = 1;
  } else if (buf[0] == 's') { // strcmp(buf, "scan")
    command->type = ROBOT_SCANING;
    return command;
  }
  if (cnt > 0 && len > cnt) {
    command->param = atoi(buf + cnt);
    return command;
  }
  return NULL;
}
