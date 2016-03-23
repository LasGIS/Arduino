#ifndef TwoMotor_h
#define TwoMotor_h

#include <Arduino.h>
#include "MotorShieldDefine.h"
/**
 * ����� �������� ��������.
 * ������ ������� count, ���� ������ � ��������� ���������� � ������
 */
class Speedometer {
public:
  uint8_t countPin;      // ��� �������� ��������
  uint8_t val;           // ��������� ��������
  uint16_t count;        // �������
  bool isChange;         // ������� ����, ��� �������� ���������
  /* ����� ��������� ����� �������� ��� ��������� ��������,
     ���� ������� ����� �� �������. */
  long lastTime;
  long lastTimeDown;     // ������ ������� ��������
  long timeInterval;     // �������� �� ������� ����� ��������� ��������

  Speedometer(uint8_t countPin);
  void interrupt();
  void clean();
};

/**
 * ����� �������� ��� ���������� ��������� ������.
 */
class DcMotor : public Speedometer {
public:
  char name;
  uint8_t upMask_A;      // ����� ��������� ����� A
  uint8_t downMask_A;    // ����� ������ ����� A
  uint8_t upMask_B;      // ����� ��������� ����� B
  uint8_t downMask_B;    // ����� ������ ����� B
  uint8_t powerPin;      // ��� ��� ��������� ��������

  volatile long startTime;     // ������ ������ ������
  volatile long endTime;       // � ��� ����� ����� ������ ������������
  volatile uint16_t endCount;  // ��� ���� ��������� �������� ����� ������ ������������
  volatile uint16_t currPower; // ������� ��������� ������ (���������� �� 0 �� 511)
  volatile uint8_t currGear;   // ������� �������� ������ (�� 0 �� 5) 0 - ���������
  bool isForward;              // ����������� ��������

  volatile bool busy = false;

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

/** ������� ����� ���������� �������. */
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
    bool isForward,       // ����������� ��������
    uint16_t speed,       // ���������� ��������
    uint8_t upMask_A,     // ����� ��������� ����� A
    uint8_t downMask_A,   // ����� ������ ����� A
    uint8_t upMask_B,     // ����� ��������� ����� B
    uint8_t downMask_B    // ����� ������ ����� B
  );
  void setBitMask(uint8_t);
};

#endif