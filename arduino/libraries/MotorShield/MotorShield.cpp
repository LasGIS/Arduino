/**
 * Подключаем и работаем с панелью на 4 светодиодных цифры.
 */
#include <MotorShield.h>
#include <MsTimer2.h>

//int speedMask[] = {2, 3, 5, 7, 10, 16};
//  0x8080, 0x0842, 0x2492, 0xa52a, 0xdad6, 0xffff

/** Скорость от передачи (5 передача - самая высокая) */
static const int MSHLD_GEAR_SPEED[] = {/*34*/0, 50, 75, 113, 170, 255};

/** Настраиваем пины конкретных моторов. */
static const DcMotor MSHLD_MOTORS[4] = {
  DcMotor(MSHLD_UP_M1A_MASK, MSHLD_DOWN_M1A_MASK, MSHLD_UP_M1B_MASK, MSHLD_DOWN_M1B_MASK, MSHLD_PWM2A_PIN),
  DcMotor(MSHLD_UP_M2A_MASK, MSHLD_DOWN_M2A_MASK, MSHLD_UP_M2B_MASK, MSHLD_DOWN_M2B_MASK, MSHLD_PWM2B_PIN),
  DcMotor(MSHLD_UP_M4A_MASK, MSHLD_DOWN_M4A_MASK, MSHLD_UP_M4B_MASK, MSHLD_DOWN_M4B_MASK, MSHLD_PWM0A_PIN),
  DcMotor(MSHLD_UP_M3A_MASK, MSHLD_DOWN_M3A_MASK, MSHLD_UP_M3B_MASK, MSHLD_DOWN_M3B_MASK, MSHLD_PWM0B_PIN)
};

MotorShield* MotorShield::_activeMotorShieldObject = 0;

/**
 * Конструктор с инициализатором
 */
DcMotor::DcMotor(
  uint8_t _upMask_A,
  uint8_t _downMask_A,
  uint8_t _upMask_B,
  uint8_t _downMask_B,
  uint8_t _powerPin
) {
  upMask_A   = _upMask_A;
  downMask_A = _downMask_A;
  upMask_B   = _upMask_B;
  downMask_B = _downMask_B;
  powerPin   = _powerPin;
  time = 0L;
}

/**
 * Конструктор с инициализатором
 */
MotorShield::MotorShield(uint8_t _leftMotorNum, uint8_t _rightMotorNum) {
  //устанавливаем режим OUTPUT
  pinMode(MSHLD_DIR_LATCH_PIN, OUTPUT);
  pinMode(MSHLD_DIR_CLK_PIN, OUTPUT);
  pinMode(MSHLD_DIR_SER_PIN, OUTPUT);
  pinMode(MSHLD_DIR_EN_PIN, OUTPUT);
/*
  pinMode(MSHLD_PWM2A_PIN, OUTPUT);
  pinMode(MSHLD_PWM2B_PIN, OUTPUT);
  pinMode(MSHLD_PWM0A_PIN, OUTPUT);
  pinMode(MSHLD_PWM0B_PIN, OUTPUT);
*/
  leftMotorNum = _leftMotorNum;
  rightMotorNum = _rightMotorNum;
  // устанавливаем enabled
  enabled();
  stopMotors();

  MotorShield::_activeMotorShieldObject = this;
  MsTimer2::set(MSHLD_DEL_TIME, MotorShield::handle_interrupt);
  MsTimer2::start();
}

void MotorShield::handle_interrupt() {
  if (_activeMotorShieldObject) {
    _activeMotorShieldObject->timeAction();
  }
}

/** включаем мотор shild */
void MotorShield::timeAction() {
  Serial.print("Time Action!");
  long time = millis();
  for (int i = 0; i < 4; i++) {
    DcMotor motor = MSHLD_MOTORS[i];
    if (motor.time > 0 && motor.time < time) {
      stopMotor(i);
    }
  }
}

/** включаем мотор shild */
void MotorShield::enabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, LOW);
}

/** выключаем мотор shild */
void MotorShield::disabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, HIGH);
}

/** Останавливаем все что можно. */
void MotorShield::stopMotors() {
  motorMask = 0;
  setBitMask();
  analogWrite(MSHLD_PWM2A_PIN, 0);
  analogWrite(MSHLD_PWM2B_PIN, 0);
  analogWrite(MSHLD_PWM0A_PIN, 0);
  analogWrite(MSHLD_PWM0B_PIN, 0);
  for (int i = 0; i < 4; i++) {
    stopMotor(i);
  }
}

/** Останавливаем конкретный мотор. */
void MotorShield::stopMotor(uint8_t nMotor) {
  DcMotor motor = MSHLD_MOTORS[nMotor];
  analogWrite(motor.powerPin, 0);
  motor.time = 0;
}

/** Останавливаем левый мотор. */
void MotorShield::leftMotorStop() {
  stopMotor(leftMotorNum);
}

/** Останавливаем правый мотор. */
void MotorShield::rightMotorStop() {
  stopMotor(rightMotorNum);
}

/**
 * Устанавливаем скорость <gear> для двигателя nMotor:
 * speed > 0 - вперёд;
 * speed < 0 - назад;
 * значения от -5 до +5
 */
void MotorShield::motor(uint8_t nMotor, int8_t gear, long time) {
  DcMotor motor = MSHLD_MOTORS[nMotor];
  int indGear = gear < 0 ? -gear : gear;
  if (indGear > 5) indGear = 5;
  int speed = MSHLD_GEAR_SPEED[indGear];
  setSpeed(speed, motor);
  motor.time = millis() + time;
}

/** Устанавливаем скорость для левого мотора */
void MotorShield::leftMotor(int8_t gear, long time) {
  motor(leftMotorNum, gear, time);
}

/** Устанавливаем скорость для правого мотора */
void MotorShield::rightMotor(int8_t gear, long time) {
  motor(rightMotorNum, gear, time);
}

/**
 * устанавливаем скорость и направление конкретного мотора
 */
void MotorShield::setSpeed(int speed, DcMotor motor) {
  setSpeed(speed, 
    motor.upMask_A,     // маска установки клемы A
    motor.downMask_A,   // маска снятия клемы A
    motor.upMask_B,     // маска установки клемы B
    motor.downMask_B,   // маска снятия клемы B
    motor.powerPin      // пин для установки скорости
  );
}

/**
 * устанавливаем скорость и направление абстрактного мотора
 */
void MotorShield::setSpeed(
  int speed,            // скорость
  uint8_t upMask_A,     // маска установки клемы A
  uint8_t downMask_A,   // маска снятия клемы A
  uint8_t upMask_B,     // маска установки клемы B
  uint8_t downMask_B,   // маска снятия клемы B
  uint8_t powerPin      // пин для установки скорости
) {
/*
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
*/
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
  analogWrite(powerPin, speed);
}

/** Выводим маску моторов в 74HC595 */
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
  //"защелкиваем" регистр, тем самым устанавливая значения на выходах
  digitalWrite(MSHLD_DIR_LATCH_PIN, HIGH);
}

