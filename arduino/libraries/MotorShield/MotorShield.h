#ifndef MotorShield_h
#define MotorShield_h

#include <Arduino.h>

#define FORWARD true
#define BACKWARD false

// Пин DIR_SER   / DS    / 14 входу 74HC595
#define MSHLD_DIR_SER_PIN   8

// Пин DIR_CLK   / SH_CP / 11 входу 74HC595
#define MSHLD_DIR_CLK_PIN   4

// Пин DIR_LATCH / ST_CP / 12 входу 74HC595
#define MSHLD_DIR_LATCH_PIN 12

// Пин DIR_EN    / OE    / 13 входу 74HC595
#define MSHLD_DIR_EN_PIN    7

// Пин PWM0A --- мощьность M4 (PIN 4, PIN 5)
#define MSHLD_PWM0A_PIN     6
// Пин PWM0B --- мощьность M3 (PIN 1, PIN 2)
#define MSHLD_PWM0B_PIN     5

// Пин PWM1A --- Сервомотор № 2
#define MSHLD_PWM1A_PIN     9
// Пин PWM1B --- Сервомотор № 1
#define MSHLD_PWM1B_PIN     10

// Пин PWM2A --- мощьность M1 (PIN 4, PIN 5)
#define MSHLD_PWM2A_PIN     11
// Пин PWM2B --- мощьность M2 (PIN 1, PIN 2)
#define MSHLD_PWM2B_PIN     3

// Пин счетчика левого мотора
#define MSHLD_LEFT_COUNT_PIN A0
// Пин счетчика правого мотора
#define MSHLD_RIGHT_COUNT_PIN A1

// маски моторов
#define MSHLD_UP_M3A_MASK  B00000001
#define MSHLD_UP_M2A_MASK  B00000010
#define MSHLD_UP_M1A_MASK  B00000100
#define MSHLD_UP_M1B_MASK  B00001000
#define MSHLD_UP_M2B_MASK  B00010000
#define MSHLD_UP_M4A_MASK  B00100000
#define MSHLD_UP_M3B_MASK  B01000000
#define MSHLD_UP_M4B_MASK  B10000000

#define MSHLD_DOWN_M3A_MASK  B11111110
#define MSHLD_DOWN_M2A_MASK  B11111101
#define MSHLD_DOWN_M1A_MASK  B11111011
#define MSHLD_DOWN_M1B_MASK  B11110111
#define MSHLD_DOWN_M2B_MASK  B11101111
#define MSHLD_DOWN_M4A_MASK  B11011111
#define MSHLD_DOWN_M3B_MASK  B10111111
#define MSHLD_DOWN_M4B_MASK  B01111111

#define MSHLD_M1 0
#define MSHLD_M2 1
#define MSHLD_M3 2
#define MSHLD_M4 3

#define MSHLD_DEL_TIME 1

/**
 * Класс счётчика скорости.
 * Только считаем count, весь анализ и коррекция происходит в моторе
 */
class Speedometer {
public:
  uint8_t countPin;      // пин счётчика скорости
  uint8_t val;           // показание датчика
  uint16_t count;        // счётчик
  Speedometer(uint8_t countPin);
  void clean();
  // проверка на изменение датчика
  bool check();
};

/**
 * класс содержит все внутренние параметры мотора.
 */
class DcMotor {
public:
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

  volatile bool busy = false;

  DcMotor(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
  bool speedCorrection();
};

/** главный класс держателей моторов. */
class MotorShield {

public:
  volatile uint8_t motorMask;

private:
  uint8_t leftMotorNum;
  uint8_t rightMotorNum;
  static MotorShield* _activeMotorShieldObject;

public:
  MotorShield(uint8_t, uint8_t);
  void enabled();
  void disabled();
  void stopMotors();

  void stopMotor(uint8_t);
  void motor(uint8_t, int8_t, int16_t);
  bool isBusy();
  bool waitBusy();

  void leftMotor(int8_t, int16_t);
  void leftMotorStop();
  void rightMotor(int8_t, int16_t);
  void rightMotorStop();

private:
  static inline void handle_interrupt();
  void timeAction();
  void setSpeed(bool, int, DcMotor*);
  void setSpeed(bool, int, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
  void setBitMask(uint8_t);
};

#endif