#define DEGREE_TO_RADIAN 0.0174533
#define DEGREE_CORRECTION -2

/** количество измерений (от 10° до 170°). */
#define DISTANCE_BEFORE_COUNT 17
/** градус начальной точки. */
#define DISTANCE_BEFORE_START -10
static float distanceBefore[DISTANCE_BEFORE_COUNT];

/**
 * анализ ситуации и принятие решений
 */
void robotAnalyse() {
  scanSituation();
  RobotAnalyseCheck check = checkForward(8, 15.0);
  int dist;
  int grad;
  switch (check) {
    case NO_BLOCK:       // нет препятствий, можно двигать
      dist = getForward(8);      
      addRobotCommand(MOTOR_FORWARD, (dist > 40 ? 40 : dist) * .75);
      addRobotCommand(ROBOT_ANALYSE, 0);
      return;
    case BLOCK_FRONT:  // помеха спереди
      addRobotCommand(MOTOR_RIGHT, 60);
      addRobotCommand(ROBOT_ANALYSE, 0);
      return;
    case BLOCK_RIGHT:    // помеха справа
      for (int i = 9; i < DISTANCE_BEFORE_COUNT; i++) {
        if (checkRight(i, 10.0)) {
          grad = (i - 8) * 10;
          addRobotCommand(((grad <= 30) ? MOTOR_FORWARD_LEFT : MOTOR_LEFT), grad + 20);
          addRobotCommand(ROBOT_ANALYSE, 0);
          return;
        }
      }
      addRobotCommand(MOTOR_LEFT, 90);
      addRobotCommand(ROBOT_ANALYSE, 0);
      return;
    case BLOCK_LEFT:     // помеха слева
      for (int i = 7; i >= 0; i--) {
        if (checkLeft(i, 10.0)) {
          grad = (8 - i) * 10;
          addRobotCommand(((grad <= 30) ? MOTOR_FORWARD_RIGHT : MOTOR_RIGHT), grad + 20);
          addRobotCommand(ROBOT_ANALYSE, 0);
          return;
        }
      }
      addRobotCommand(MOTOR_RIGHT, 90);
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
    if (i >= 0 && i < DISTANCE_BEFORE_COUNT && distanceBefore[i] > 0) {
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
    if (i >= 0 && distanceBefore[i] > 0) {
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
    if (i < DISTANCE_BEFORE_COUNT && distanceBefore[i] > 0) {
      float rel = distanceBefore[i] * cos(angle * DEGREE_TO_RADIAN);
      if (rel < min) min = rel;
    }
  }
  return min > dist;
}

void scanSituation() {
  headMovement();
  // дополнительная проверка
  float distance[5];
  for (int i = 0; i < DISTANCE_BEFORE_COUNT; i++) {
    distance[0] = distanceBefore[i];
    if (distance[0] < 15.0) {
      float aver = distance[0];
      float sigm = 0;
      for (int j = 1; j < 5; j++) {
        distance[j] = loadDistance(i * 10 + DISTANCE_BEFORE_START);
        aver += distance[j];
      }
      aver /= 5;
      for (int j = 0; j < 5; j++) {
        sigm += sq(aver - distance[j]);
      }
      sigm = sqrt(sigm / 5);
      Serial.print("aver = ");
      Serial.print(aver);
      Serial.print("; sigm = ");
      Serial.println(sigm);
      distanceBefore[i] = (sigm > 3) ? -1 : aver;
    }
  }
  for (int i = 0; i < DISTANCE_BEFORE_COUNT; i++) {
    Serial.print(distanceBefore[i]);
    Serial.print(";");
  }
  Serial.println("");
}

/**
 * движение головой
 */
void headMovement() {
  if (vSer.read() < 90) {
    for (int i = 0; i < DISTANCE_BEFORE_COUNT; i++) {
      distanceBefore[i] = loadDistance(i * 10 + DISTANCE_BEFORE_START);
    }
  } else {
    for (int i = DISTANCE_BEFORE_COUNT - 1; i >= 0; i--) {
      distanceBefore[i] = loadDistance(i * 10 + DISTANCE_BEFORE_START);
    }
  }
}

/**
 * Считываем расстояние на некотором повороте головы
 * degree - угол поворота
 * return - расстояние в см
 */
float loadDistance(int degree) {
  headToDegree(degree);
  return showDistance(degree);
}

/**
 * медленное перемещение головы в определённое положение
 */
void headToDegree(int degre) {
  int degree = degre + DEGREE_CORRECTION;
  int i = vSer.read();
  if (i > degree) {
    for (; i >= degree; i--) {
      vSer.write(i);
      delay(10);
    }
  } else {
    for (; i <= degree; i++) {
      vSer.write(i);
      delay(10);
    }
  }
}
