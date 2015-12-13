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

class MotorShield {

public:

private:
  uint8_t motorMask;

public:
  MotorShield();
  void enabled();
  void disabled();
  void stopMotor();
  uint8_t setM1(int);
  uint8_t setM2(int);
  uint8_t setM3(int);
  uint8_t setM4(int);

private:
  void setSpeed(int, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
  void setBitMask();
};

#endif