#ifndef MotorShield_h
#define MotorShield_h

#include <Arduino.h>

// Пин DIR_SER   / DS    / 14 входу 74HC595
#define MSHLD_DIR_SER_PIN   8

// Пин DIR_CLK   / SH_CP / 11 входу 74HC595
#define MSHLD_DIR_CLK_PIN   4

// Пин DIR_LATCH / ST_CP / 12 входу 74HC595
#define MSHLD_DIR_LATCH_PIN 12

// Пин DIR_EN    / OE    / 13 входу 74HC595
#define MSHLD_DIR_EN_PIN    7

// Пин PWM0A
#define MSHLD_PWM0A_PIN     6
// Пин PWM0B
#define MSHLD_PWM0B_PIN     5

// Пин PWM1A
#define MSHLD_PWM1A_PIN     9
// Пин PWM1B
#define MSHLD_PWM1B_PIN     10

// Пин PWM2A
#define MSHLD_PWM2A_PIN     11
// Пин PWM2B
#define MSHLD_PWM2B_PIN     3

class MotorShield {

public:
  static volatile byte panel[4];

private:
  //Пин подключен к ST_CP входу 74HC595
  int latchPin = 8;
  //Пин подключен к SH_CP входу 74HC595
  int clockPin = 12;
  //Пин подключен к DS входу 74HC595
  int dataPin = 11;
  // 4 Пина для управления цифрами
  int digit[4] = {10,9,7,4};

  String panelValue;
  static MotorShield* _activeMotorShieldObject;

public:
  MotorShield(int _latchPin, int _clockPin, int _dataPin, int* _digits, int _count);
  void setValue(String value);
  String getValue();

private:
  static inline void handle_interrupt();
  void showSegment();
  void resetAllDigit();
  byte getChar(char chr, boolean isPnt);
  void setNumBit(uint8_t mask);
};

#endif