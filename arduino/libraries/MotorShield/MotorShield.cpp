/**
 * ���������� � �������� � ������� �� 4 ������������ �����.
 */
#include <MotorShield.h>

/**
 * ����������� � ���������������
 */
MotorShield::MotorShield() {
  //������������� ����� OUTPUT
  pinMode(MSHLD_DIR_LATCH_PIN, OUTPUT);
  pinMode(MSHLD_DIR_CLK_PIN, OUTPUT);
  pinMode(MSHLD_DIR_SER_PIN, OUTPUT);
  pinMode(MSHLD_DIR_EN_PIN, OUTPUT);
  // ������������� enabled
  digitalWrite(MSHLD_DIR_EN_PIN, LOW);
/*
  todo: ����� ������� ��� ������ enabled() � disabled()
  // ������� enabled
  digitalWrite(MSHLD_DIR_EN_PIN, HIGH);
*/
  motorMask = 0;
}

void MotorShield::stopMotor() {
  motorMask = 0;
  setBitMask();
  analogWrite(MSHLD_PWM2A_PIN, 0);
  analogWrite(MSHLD_PWM2B_PIN, 0);
  analogWrite(MSHLD_PWM0A_PIN, 0);
  analogWrite(MSHLD_PWM0B_PIN, 0);
}

/**
 * ��������� M1 �� �����
 */
void MotorShield::rightMotorForward() {
  motorMask = (motorMask & MSHLD_DOWN_M1B_MASK) | MSHLD_UP_M1A_MASK;
  setBitMask();
}

/**
 * ��������� M1 �� �����
 */
void MotorShield::rightMotorBackward() {
  motorMask = (motorMask & MSHLD_DOWN_M1A_MASK) | MSHLD_UP_M1B_MASK;
  setBitMask();
}

/**
 * ��������� M2 �� �����
 */
void MotorShield::leftMotorForward() {
  motorMask = (motorMask & MSHLD_DOWN_M2B_MASK) | MSHLD_UP_M2A_MASK;
  setBitMask();
}

/**
 * ��������� M2 �� �����
 */
void MotorShield::leftMotorBackward() {
  motorMask = (motorMask & MSHLD_DOWN_M2A_MASK) | MSHLD_UP_M2B_MASK;
  setBitMask();
}

/**
 * ��������� ��������� M1
 */
void MotorShield::rightMotorPower(int power) {
  analogWrite(MSHLD_PWM2A_PIN, power);
}

/**
 * ��������� ��������� M2
 */
void MotorShield::leftMotorPower(int power) {
  analogWrite(MSHLD_PWM2B_PIN, power);
}

/** ������� ����� ������� � 74HC595 */
void MotorShield::setBitMask() {
  Serial.print(motorMask, BIN);
  Serial.print(" - ");
  Serial.println(motorMask, HEX);
  // ������������� ������������� "�������" �� LOW
  digitalWrite(MSHLD_DIR_LATCH_PIN, LOW);
  // �������� ��������������� �� MSHLD_DIR_SER_PIN
  shiftOut(MSHLD_DIR_SER_PIN, MSHLD_DIR_CLK_PIN, MSBFIRST, motorMask);
  //"�����������" �������, ��� ����� ������������ �������� �� �������
  digitalWrite(MSHLD_DIR_LATCH_PIN, HIGH);
}

