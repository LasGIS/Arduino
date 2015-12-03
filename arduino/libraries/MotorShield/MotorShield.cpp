/**
 * ѕодключаем и работаем с панелью на 4 светодиодных цифры.
 */
#include <MotorShield.h>

/**
 *  онструктор с инициализатором
 */
MotorShield::MotorShield() {
  //устанавливаем режим OUTPUT
  pinMode(MSHLD_DIR_LATCH_PIN, OUTPUT);
  pinMode(MSHLD_DIR_CLK_PIN, OUTPUT);
  pinMode(MSHLD_DIR_SER_PIN, OUTPUT);
  pinMode(MSHLD_DIR_EN_PIN, OUTPUT);
  // устанавливаем enabled
  digitalWrite(MSHLD_DIR_EN_PIN, LOW);
/*
  todo: потом добавим два метода enabled() и disabled()
  // снимаем enabled
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
 * двигатель M1 на вперЄд
 */
void MotorShield::rightMotorForward() {
  motorMask = (motorMask & MSHLD_DOWN_M1B_MASK) | MSHLD_UP_M1A_MASK;
  setBitMask();
}

/**
 * двигатель M1 на назад
 */
void MotorShield::rightMotorBackward() {
  motorMask = (motorMask & MSHLD_DOWN_M1A_MASK) | MSHLD_UP_M1B_MASK;
  setBitMask();
}

/**
 * двигатель M2 на вперЄд
 */
void MotorShield::leftMotorForward() {
  motorMask = (motorMask & MSHLD_DOWN_M2B_MASK) | MSHLD_UP_M2A_MASK;
  setBitMask();
}

/**
 * двигатель M2 на назад
 */
void MotorShield::leftMotorBackward() {
  motorMask = (motorMask & MSHLD_DOWN_M2A_MASK) | MSHLD_UP_M2B_MASK;
  setBitMask();
}

/**
 * мощьность двигатель M1
 */
void MotorShield::rightMotorPower(int power) {
  analogWrite(MSHLD_PWM2A_PIN, power);
}

/**
 * мощьность двигатель M2
 */
void MotorShield::leftMotorPower(int power) {
  analogWrite(MSHLD_PWM2B_PIN, power);
}

/** ¬ыводим маску моторов в 74HC595 */
void MotorShield::setBitMask() {
  Serial.print(motorMask, BIN);
  Serial.print(" - ");
  Serial.println(motorMask, HEX);
  // устанавливаем синхронизацию "защелки" на LOW
  digitalWrite(MSHLD_DIR_LATCH_PIN, LOW);
  // передаем последовательно на MSHLD_DIR_SER_PIN
  shiftOut(MSHLD_DIR_SER_PIN, MSHLD_DIR_CLK_PIN, MSBFIRST, motorMask);
  //"защелкиваем" регистр, тем самым устанавлива€ значени€ на выходах
  digitalWrite(MSHLD_DIR_LATCH_PIN, HIGH);
}

