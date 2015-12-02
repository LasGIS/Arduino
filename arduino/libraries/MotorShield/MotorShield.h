#ifndef MotorShield_h
#define MotorShield_h

#include <Arduino.h>

// ��� DIR_SER   / DS    / 14 ����� 74HC595
#define MSHLD_DIR_SER_PIN   8

// ��� DIR_CLK   / SH_CP / 11 ����� 74HC595
#define MSHLD_DIR_CLK_PIN   4

// ��� DIR_LATCH / ST_CP / 12 ����� 74HC595
#define MSHLD_DIR_LATCH_PIN 12

// ��� DIR_EN    / OE    / 13 ����� 74HC595
#define MSHLD_DIR_EN_PIN    7

// ��� PWM0A
#define MSHLD_PWM0A_PIN     6
// ��� PWM0B
#define MSHLD_PWM0B_PIN     5

// ��� PWM1A
#define MSHLD_PWM1A_PIN     9
// ��� PWM1B
#define MSHLD_PWM1B_PIN     10

// ��� PWM2A
#define MSHLD_PWM2A_PIN     11
// ��� PWM2B
#define MSHLD_PWM2B_PIN     3

class MotorShield {

public:
  static volatile byte panel[4];

private:
  //��� ��������� � ST_CP ����� 74HC595
  int latchPin = 8;
  //��� ��������� � SH_CP ����� 74HC595
  int clockPin = 12;
  //��� ��������� � DS ����� 74HC595
  int dataPin = 11;
  // 4 ���� ��� ���������� �������
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