#ifndef ROBOT2_H
#define ROBOT2_H

#include <Arduino.h>
#include <Servo.h>
#include "MotorShield.h"
#include "RobotCommand.h"

/* определяем используемые функции */
/******** Analyse.cpp ********/
/**
 * анализ ситуации и принятие решений
 */
void robotAnalyse();
/** проверить проход вперёд (^^) */
RobotAnalyseCheck checkForward(int, float);
/** получить дистанцию спереди (^^) */
float getForward(int centerDegre);
/** проверка дистанции справа => */
bool checkRight(int centerDegre, float dist);
/** проверка дистанции слева <= */
bool checkLeft(int centerDegre, float dist);
void scanSituation();
/** движение головой */
void headMovement();
/**
 * Считываем расстояние на некотором повороте головы
 * degree - угол поворота
 * return - расстояние в см
 */
float loadDistance(int degree);
/**
 * медленное перемещение головы в определённое положение
 */
void headToDegree(int degre);

/******** RobotCommand.cpp ********/
/** получаем ссылку на команду для заполнения. */
RobotCommand* getRobotCommand4Load();
/** получаем ссылку на первую команду, готовую к выполнению. */
RobotCommand* getRobotCommand4Run();
/** останавливаем всё */
void robotStop();
/** разбираем строку в команду */
void addRobotCommand(char buf[], int len);
void addRobotCommand(RobotCommandType type, int param);
/** выполняем команду */
void action(RobotCommand* command);

/******** USRangeFinder.cpp ********/
float showDistance(int degre);

#endif // ROBOT2_H
