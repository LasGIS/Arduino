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
  bool isChange;         // признак того, что счётччик поменялся
  /* Время последней смены счётчика или последней проверки,
     если счетчик долго не менялся. */
  long lastTime;
  long lastTimeDown;     // момент падения счётчика
  long timeInterval;     // интервал по времени между падениями счётчика

  Speedometer(uint8_t countPin);
  void interrupt();
  void clean();
};

/**
 * класс содержит все внутренние параметры мотора.
 */
class DcMotor : public Speedometer {
public:
  char name;
  uint8_t upMask_A;      // маска установки клемы A
  uint8_t downMask_A;    // маска снятия клемы A
  uint8_t upMask_B;      // маска установки клемы B
  uint8_t downMask_B;    // маска снятия клемы B
  uint8_t powerPin;      // пин для установки скорости

  volatile long startTime;     // Начало работы мотора
  volatile long endTime;       // В это время мотор должен остановиться
  volatile uint16_t endCount;  // при этом показании счётчика мотор должен остановиться
  volatile uint16_t currPower; // текущая мощность мотора (напряжение от 0 до 255)
  volatile uint8_t currGear;   // текущая передача мотора (от 0 до 5) 0 - выключена
  bool isForward;              // направление движения

  volatile bool busy = false;  // мотор занят (работает)

  DcMotor(
    char name,
    uint8_t _upMask_A,
    uint8_t _downMask_A,
    uint8_t _upMask_B,
    uint8_t _downMask_B,
    uint8_t _powerPin,
    uint8_t _countPin
  );
  bool speedCorrection(long time);
  void setPower();
  void showStartParameters();
};

/** главный класс держателей моторов. */
class TwoMotor {

public:
  volatile uint8_t motorMask;

private:
  DcMotor* leftMotor;
  DcMotor* rightMotor;
  static TwoMotor* _activeMotorShieldObject;

public:
  TwoMotor();
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
  void setSpeed(DcMotor*);
  void setSpeed(
    bool isForward,       // направление движения
    uint16_t speed,       // абсолютная скорость
    uint8_t upMask_A,     // маска установки клемы A
    uint8_t downMask_A,   // маска снятия клемы A
    uint8_t upMask_B,     // маска установки клемы B
    uint8_t downMask_B    // маска снятия клемы B
  );
  void setBitMask(uint8_t);
};

#endif
