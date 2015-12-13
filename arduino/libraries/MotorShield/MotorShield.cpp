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

  pinMode(MSHLD_PWM2A_PIN, OUTPUT);
  pinMode(MSHLD_PWM2B_PIN, OUTPUT);
  pinMode(MSHLD_PWM0A_PIN, OUTPUT);
  pinMode(MSHLD_PWM0B_PIN, OUTPUT);
  // устанавливаем enabled
  enabled();
  stopMotor();
}

/** включаем мотор shild */
void  MotorShield::enabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, LOW);
}

/** выключаем мотор shild */
void  MotorShield::disabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, HIGH);
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
 * ”станавливаем скорость двигатель M1:
 * speed > 0 - вперЄд;
 * speed < 0 - назад;
 * значени€ от -255 до +255
 */
uint8_t MotorShield::setM1(int speed) {
  setSpeed(speed,
    MSHLD_UP_M1A_MASK, MSHLD_DOWN_M1A_MASK,
    MSHLD_UP_M1B_MASK, MSHLD_DOWN_M1B_MASK,
    MSHLD_PWM2A_PIN
  );
  return MSHLD_PWM2A_PIN;
}

/**
 * ”станавливаем скорость двигатель M2:
 * speed > 0 - вперЄд;
 * speed < 0 - назад;
 * значени€ от -255 до +255
 */
uint8_t MotorShield::setM2(int speed) {
  setSpeed(speed,
    MSHLD_UP_M2A_MASK, MSHLD_DOWN_M2A_MASK,
    MSHLD_UP_M2B_MASK, MSHLD_DOWN_M2B_MASK,
    MSHLD_PWM2B_PIN
  );
  return MSHLD_PWM2B_PIN;
}

/**
 * ”станавливаем скорость двигатель M3:
 * speed > 0 - вперЄд;
 * speed < 0 - назад;
 * значени€ от -255 до +255
 */
uint8_t MotorShield::setM3(int speed) {
  setSpeed(speed,
    MSHLD_UP_M4A_MASK, MSHLD_DOWN_M4A_MASK,
    MSHLD_UP_M4B_MASK, MSHLD_DOWN_M4B_MASK,
    MSHLD_PWM0A_PIN
  );
  return MSHLD_PWM0A_PIN;
}

/**
 * ”станавливаем скорость двигатель M4:
 * speed > 0 - вперЄд;
 * speed < 0 - назад;
 * значени€ от -255 до +255
 */
uint8_t MotorShield::setM4(int speed) {
  setSpeed(speed,
    MSHLD_UP_M3A_MASK, MSHLD_DOWN_M3A_MASK,
    MSHLD_UP_M3B_MASK, MSHLD_DOWN_M3B_MASK,
    MSHLD_PWM0B_PIN
  );
  return MSHLD_PWM0B_PIN;
}

/**
 * устанавливаем скорость и направление абстрактного мотора
 */
void MotorShield::setSpeed(
  int speed,            // скорость
  uint8_t upMask_A,     // маска установки клемы A
  uint8_t downMask_A,   // маска сн€ти€ клемы A
  uint8_t upMask_B,     // маска установки клемы B
  uint8_t downMask_B,   // маска сн€ти€ клемы B
  uint8_t powerPin      // пин дл€ установки скорости
) {

  Serial.print("speed = ");
  Serial.print(speed, DEC);
  Serial.print("; Masks = ");
  Serial.print(upMask_A, BIN);
  Serial.print(" ; ");
  Serial.print(downMask_A, BIN);
  Serial.print(" ; ");
  Serial.print(upMask_B, BIN);
  Serial.print(" ; ");
  Serial.print(downMask_B, BIN);
  Serial.print("; powerPin = ");
  Serial.print(powerPin);
  Serial.println(" ; ");
  
  // устанавливаем направление
  uint8_t mask = motorMask;
  if (speed > 0) {
    mask = (mask & downMask_B) | upMask_A;
  } else if (speed == 0) {
    mask = mask & downMask_A & downMask_B;
  } else {
    speed = -speed;
    mask = (mask & downMask_A) | upMask_B;
  }
  if (mask != motorMask) {
    motorMask = mask;
    setBitMask();
  }

  // устанавливаем скорость
  if (speed > 255) speed = 255;
//  analogWrite(powerPin, speed);
}

/** ¬ыводим маску моторов в 74HC595 */
void MotorShield::setBitMask() {
/*
  Serial.print(motorMask, BIN);
  Serial.print(" - ");
  Serial.println(motorMask, HEX);
*/
  // устанавливаем синхронизацию "защелки" на LOW
  digitalWrite(MSHLD_DIR_LATCH_PIN, LOW);
  // передаем последовательно на MSHLD_DIR_SER_PIN
  shiftOut(MSHLD_DIR_SER_PIN, MSHLD_DIR_CLK_PIN, MSBFIRST, motorMask);
  //"защелкиваем" регистр, тем самым устанавлива€ значени€ на выходах
  digitalWrite(MSHLD_DIR_LATCH_PIN, HIGH);
}

