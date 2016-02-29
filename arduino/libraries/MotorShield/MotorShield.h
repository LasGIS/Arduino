#ifndef MotorShield_h
#define MotorShield_h

#include <Arduino.h>
#include "MotorShieldDefine.h"

/**
 * ����� �������� ��������.
 * ������ ������� count, ���� ������ � ��������� ���������� � ������
 */
class Speedometer {
public:
  uint8_t countPin;      // ��� �������� ��������
  uint8_t val;           // ��������� �������
  uint16_t count;        // �������
  /* ����� ��������� ����� �������� ��� ��������� ��������,
     ���� ������� ����� �� �������. */
  long lastTime;

  Speedometer(uint8_t countPin);
  void clean();
  // �������� �� ��������� �������
  bool check(long time);
};

/**
 * ����� �������� ��� ���������� ��������� ������.
 */
class DcMotor {
public:
  uint8_t upMask_A;      // ����� ��������� ����� A
  uint8_t downMask_A;    // ����� ������ ����� A
  uint8_t upMask_B;      // ����� ��������� ����� B
  uint8_t downMask_B;    // ����� ������ ����� B
  uint8_t powerPin;      // ��� ��� ��������� ��������
  // ������� ��������
  Speedometer* speedometer = NULL;

  volatile long startTime;     // ������ ������ ������
  volatile long endTime;       // � ��� ����� ����� ������ ������������
  volatile uint16_t endCount;  // ��� ���� ��������� �������� ����� ������ ������������
  volatile uint16_t currPower; // ������� ��������� ������ (���������� �� 0 �� 511)
  volatile uint8_t currGear;   // ������� �������� ������ (�� 0 �� 5) 0 - ���������

  volatile bool busy = false;

  DcMotor(
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

/** ������� ����� ���������� �������. */
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
  void setSpeed(bool, DcMotor*);
  void setSpeed(bool, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
  void setBitMask(uint8_t);
};

#endif