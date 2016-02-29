#ifndef TwoMotor_h
#define TwoMotor_h

#include <Arduino.h>
#include "MotorShieldDefine.h"
/**
 * Класс счётчика скорости.
 * Только считаем count, весь анализ и коррекция происходит в моторе
 */
class Speedometer {
public:
  uint8_t countPin;      // пин счётчика скорости
  uint8_t val;           // показание счётчика
  uint16_t count;        // счётчик
  /* Время последней смены счётчика или последней проверки,
     если счетчик долго не менялся. */
  long lastTime;

  Speedometer(uint8_t countPin);
  void clean();
  // проверка на изменение датчика
  bool check(long time);
};

/**
 * класс содержит все внутренние параметры мотора.
 */
class DcMotor {
public:
  char name;
  uint8_t upMask_A;      // маска установки клемы A
  uint8_t downMask_A;    // маска снятия клемы A
  uint8_t upMask_B;      // маска установки клемы B
  uint8_t downMask_B;    // маска снятия клемы B
  uint8_t powerPin;      // пин для установки скорости
  // счётчик скорости
  Speedometer* speedometer = NULL;

  volatile long startTime;     // Начало работы мотора
  volatile long endTime;       // В это время мотор должен остановиться
  volatile uint16_t endCount;  // при этом показании счётчика мотор должен остановиться
  volatile uint16_t currPower; // текущая мощьность мотора (напряжение от 0 до 511)
  volatile uint8_t currGear;   // текущая передача мотора (от 0 до 5) 0 - выключена

  volatile bool busy = false;

  DcMotor(
    char name,
    uint8_t _upMask_A,
    uint8_t _downMask_A,
    uint8_t _upMask_B,
    uint8_t _downMask_B,
    uint8_t _powerPin
  );
  bool speedCorrection(long time);
  void setPower();
  void showStartParameters();
};

/** главный класс держателей моторов. */
class MotorShield {

public:
  volatile uint8_t motorMask;

private:
  DcMotor* leftMotor;
  DcMotor* rightMotor;
  static MotorShield* _activeMotorShieldObject;

public:
  MotorShield(uint8_t, uint8_t);
  void enabled();
  void disabled();
  void stopMotors();

  void stopMotor(DcMotor*);
  void motor(DcMotor*, int8_t, int16_t);
  bool isBusy();
  bool waitBusy();

  void leftMotorStart(int8_t, int16_t);
  void leftMotorStop();
  void rightMotorStart(int8_t, int16_t);
  void rightMotorStop();

private:
  static inline void handle_interrupt();
  void timeAction();
  void timeAction(DcMotor*);
  void setSpeed(bool, DcMotor*);
  void setSpeed(bool, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
  void setBitMask(uint8_t);
};

#endif