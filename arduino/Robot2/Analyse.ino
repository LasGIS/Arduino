#define DEGREE_TO_RADIAN 0.0174533

static float distanceBefore[19];

/**
 * анализ ситуации и принятие решений
 */
void robotAnalyse() {
  headMovement();
  RobotAnalyseCheck check = checkForward(9, 15.0);
  int dist;
  switch (check) {
    case NO_BLOCK:       // нет препятствий, можно двигать
      dist = getForward(9);      
      addRobotCommand(MOTOR_FORWARD, (dist > 40 ? 40 : dist) * .75);
      addRobotCommand(ROBOT_ANALYSE, 0);
      return;
    case BLOCK_FRONT:  // помеха спереди
      addRobotCommand(MOTOR_RIGHT, 10);
      addRobotCommand(ROBOT_ANALYSE, 0);
      return;
    case BLOCK_RIGHT:    // помеха справа
      for (int i = 10; i <= 17; i++) {
        if (checkRight(i, 10.0)) {
          int grad = (i - 9) * 10;
          addRobotCommand((grad <= 30) ? MOTOR_FORWARD_LEFT : MOTOR_LEFT, grad + 30);
          addRobotCommand(ROBOT_ANALYSE, 0);
          return;
        }
      }
      addRobotCommand(MOTOR_LEFT, 180);
      addRobotCommand(ROBOT_ANALYSE, 0);
      return;
    case BLOCK_LEFT:     // помеха слева
      for (int i = 8; i >= 1; i--) {
        if (checkLeft(i, 10.0)) {
          int grad = (9 - i) * 10;
          addRobotCommand((grad <= 30) ? MOTOR_FORWARD_RIGHT : MOTOR_RIGHT, grad + 30);
          addRobotCommand(ROBOT_ANALYSE, 0);
          return;
        }
      }
      addRobotCommand(MOTOR_RIGHT, 180);
      addRobotCommand(ROBOT_ANALYSE, 0);
      return;
  }
}

/** проверить проход вперёд (^^) */
RobotAnalyseCheck checkForward(int centerDegre, float distance) {
  if (!checkLeft(centerDegre, 10.0)) {
    return BLOCK_LEFT;
  } else if (!checkRight(centerDegre, 10.0)) {
    return BLOCK_RIGHT;
  } else if (getForward(centerDegre) < distance) {
    return BLOCK_FRONT;
  }
  return NO_BLOCK;
}


/** получить дистанцию спереди (^^) */
float getForward(int centerDegre) {
  float min = 1000.0; 
  int angle, i;
  for (angle = 50, i = centerDegre - 4; angle <= 130; angle += 10, i++) {
    if (i >= 0 && i <= 18) {
      float rel = distanceBefore[i] * sin(angle * DEGREE_TO_RADIAN);
      if (rel < min) min = rel;
    }
  }
  return min;
}

/** проверка дистанции справа => */
bool checkRight(int centerDegre, float dist) {
  float min = 1000.0; 
  int angle, i;
  for (angle = 10, i = centerDegre - 8; angle <= 50; angle += 10, i++) {
    if (i >= 0) {
      float rel = distanceBefore[i] * cos(angle * DEGREE_TO_RADIAN);
      if (rel < min) min = rel;
    }
  }
  return min > dist;
}

/** проверка дистанции слева <= */
bool checkLeft(int centerDegre, float dist) {
  float min = 1000.0; 
  int angle, i;
  for (angle = 10, i = centerDegre + 8; angle <= 50; angle += 10, i--) {
    if (i <= 18) {
      float rel = distanceBefore[i] * cos(angle * DEGREE_TO_RADIAN);
      if (rel < min) min = rel;
    }
  }
  return min > dist;
}

/**
 * движение головой
 */
void headMovement() {
  if (vSer.read() < 90) {
    for (int i = 0; i <= 18; i++) {
      distanceBefore[i] = loadDistance(i * 10);
    }
  } else {
    for (int i = 18; i >= 0; i--) {
      distanceBefore[i] = loadDistance(i * 10);
    }
  }
  for (int i = 0; i <= 18; i++) {
    Serial.print(distanceBefore[i]);
    Serial.print(";");
  }
  Serial.println("");
}

/**
 * Считываем расстояние на некотором повороте головы
 * degre - угол поворота
 * return - расстояние в см
 */
float loadDistance(int degre) {
  int i = vSer.read();
  if (i > degre) {
    for (; i >= degre; i--) {
//      Serial.print(i);
//      Serial.println(' ');
      vSer.write(i);
      delay(10);
    }
  } else {
    for (; i <= degre; i++) {
//      Serial.print(i);
//      Serial.println(' ');
      vSer.write(i);
      delay(10);
    }
  }
  return showDistance(degre);
}
