/**
 * ���������� � �������� � ������� �� 4 ������������ �����.
 */
#include <MotorShield.h>
#include <MsTimer2.h>

//int speedMask[] = {2, 3, 5, 7, 10, 16};
//  0x8080, 0x0842, 0x2492, 0xa52a, 0xdad6, 0xffff

/** �������� �� �������� (5 �������� - ����� �������) */
//static const int MSHLD_GEAR_SPEED[] = {/*34*/0, 50, 75, 113, 170, 255};

/** ����������� ���� ���������� �������. */
static DcMotor MSHLD_MOTORS[4] = {
  DcMotor(MSHLD_UP_M1A_MASK, MSHLD_DOWN_M1A_MASK, MSHLD_UP_M1B_MASK, MSHLD_DOWN_M1B_MASK, MSHLD_PWM2A_PIN),
  DcMotor(MSHLD_UP_M2A_MASK, MSHLD_DOWN_M2A_MASK, MSHLD_UP_M2B_MASK, MSHLD_DOWN_M2B_MASK, MSHLD_PWM2B_PIN),
  DcMotor(MSHLD_UP_M4A_MASK, MSHLD_DOWN_M4A_MASK, MSHLD_UP_M4B_MASK, MSHLD_DOWN_M4B_MASK, MSHLD_PWM0A_PIN),
  DcMotor(MSHLD_UP_M3A_MASK, MSHLD_DOWN_M3A_MASK, MSHLD_UP_M3B_MASK, MSHLD_DOWN_M3B_MASK, MSHLD_PWM0B_PIN)
};

static Speedometer LEFT_SPEEDOMETR(MSHLD_LEFT_COUNT_PIN);
static Speedometer RIGHT_SPEEDOMETR(MSHLD_RIGHT_COUNT_PIN);

MotorShield* MotorShield::_activeMotorShieldObject = 0;

/**
 * ����������� ����������
 */
Speedometer::Speedometer(uint8_t _countPin) {
  countPin = _countPin;
  pinMode(countPin, INPUT);
  clean();
}

/**
 * �������� �� ��������� �������
 */
bool Speedometer::check() {
  uint8_t newVal = digitalRead(countPin);
  if (newVal != val) {
    count++;
    val = newVal;
    return true;
  }
  return false;
}

/** ���������� ��������� */
void Speedometer::clean() {
  count = 0;
  val = digitalRead(countPin);
}

/**
 * ����������� ������
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
 * ����������� � ���������������
 */
MotorShield::MotorShield(uint8_t _leftMotorNum, uint8_t _rightMotorNum) {
  //������������� ����� OUTPUT
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
  pinMode(MSHLD_LEFT_COUNT_PIN, INPUT);
  pinMode(MSHLD_RIGHT_COUNT_PIN, INPUT);

  leftMotorNum = _leftMotorNum;
  rightMotorNum = _rightMotorNum;
  // ��� �������� �������
  (MSHLD_MOTORS + leftMotorNum)->speedometer = &LEFT_SPEEDOMETR;
  (MSHLD_MOTORS + rightMotorNum)->speedometer = &RIGHT_SPEEDOMETR;

  // ������������� enabled
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

/********************************************
 * ��������� ����������� ������ �����������
 */
void MotorShield::timeAction() {
  long time = millis();
  for (int i = 0; i < 4; i++) {
    DcMotor* motor = MSHLD_MOTORS + i;
    Speedometer* speedometer = motor->speedometer;
    if (speedometer) {
      speedometer->check();
    }
    if (motor->time > 0 && motor->time < time) {
      Serial.print("stopMotor(");
      Serial.print(i);
      if (speedometer) {
        Serial.print("); count = ");
        Serial.print(speedometer->count);
        Serial.println(";");
      } else {
        Serial.println(")");
      }
      stopMotor(i);
    }
  }
}

/** �������� ������ shild */
void MotorShield::enabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, LOW);
}

/** ��������� ������ shild */
void MotorShield::disabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, HIGH);
}

/** ������������� ��� ��� �����. */
void MotorShield::stopMotors() {
  setBitMask(0);
  analogWrite(MSHLD_PWM2A_PIN, 0);
  analogWrite(MSHLD_PWM2B_PIN, 0);
  analogWrite(MSHLD_PWM0A_PIN, 0);
  analogWrite(MSHLD_PWM0B_PIN, 0);
  for (int i = 0; i < 4; i++) {
    stopMotor(i);
  }
}

/**
 * ������������� ���������� �����.
 */
void MotorShield::stopMotor(uint8_t nMotor) {
  DcMotor* motor = MSHLD_MOTORS + nMotor;
  setBitMask(motorMask & motor->downMask_A & motor->downMask_B);
  analogWrite(motor->powerPin, 0);
  motor->time = 0;
  motor->busy = false;
}

/**
 * ��������� ������
 */
bool MotorShield::isBusy() {
  bool busy = false;
  for (int i = 0; i < 4; i++) {
    busy = busy || MSHLD_MOTORS[i].busy;
  }
  return busy;
}

/**
 * ������� ���������
 */
bool MotorShield::waitBusy() {
  for (int i = 0; i < 1000; i++) {
    if (!isBusy()) {
      return true;
    }
    delay(MSHLD_DEL_TIME);
  }
  return false;
}

/** ������������� ����� �����. */
void MotorShield::leftMotorStop() {
  stopMotor(leftMotorNum);
}

/** ������������� ������ �����. */
void MotorShield::rightMotorStop() {
  stopMotor(rightMotorNum);
}

/** ������������� �������� ��� ������ ������ */
void MotorShield::leftMotor(int speed, long time) {
  motor(leftMotorNum, speed, time);
}

/** ������������� �������� ��� ������� ������ */
void MotorShield::rightMotor(int speed, long time) {
  motor(rightMotorNum, speed, time);
}

/**
 * ������������� �������� <gear> ��� ��������� nMotor:
 * speed > 0 - �����;
 * speed < 0 - �����;
 * �������� �� -5 �� +5
 */
void MotorShield::motor(uint8_t nMotor, int speed, long time) {
  DcMotor* motor = MSHLD_MOTORS + nMotor;
  motor->time = millis() + time;
  motor->busy = true;
  // ����������� ����������
  Speedometer* speedometer = motor->speedometer;
  if (speedometer) {
    speedometer->clean();
  }
  setSpeed(speed, motor);
//  Serial.print("time = ");
//  Serial.println(motor->time, DEC);
}

/**
 * ������������� �������� � ����������� ����������� ������
 */
void MotorShield::setSpeed(int speed, DcMotor* motor) {
  setSpeed(speed,
    motor->upMask_A,     // ����� ��������� ����� A
    motor->downMask_A,   // ����� ������ ����� A
    motor->upMask_B,     // ����� ��������� ����� B
    motor->downMask_B,   // ����� ������ ����� B
    motor->powerPin      // ��� ��� ��������� ��������
  );
}

/**
 * ������������� �������� � ����������� ������������ ������
 */
void MotorShield::setSpeed(
  int speed,            // ��������
  uint8_t upMask_A,     // ����� ��������� ����� A
  uint8_t downMask_A,   // ����� ������ ����� A
  uint8_t upMask_B,     // ����� ��������� ����� B
  uint8_t downMask_B,   // ����� ������ ����� B
  uint8_t powerPin      // ��� ��� ��������� ��������
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
  // ������������� �����������
  uint8_t mask = motorMask;
  if (speed > 0) {
    mask = (mask & downMask_B) | upMask_A;
  } else if (speed == 0) {
    mask = mask & downMask_A & downMask_B;
  } else {
    speed = -speed;
    mask = (mask & downMask_A) | upMask_B;
  }
  setBitMask(mask);

  // ������������� ��������
  if (speed > 255) speed = 255;
  analogWrite(powerPin, speed);
}

/** ������� ����� ������� � 74HC595 */
void MotorShield::setBitMask(uint8_t mask) {
  if (mask != motorMask) {
    motorMask = mask;
  /*
    Serial.print("motorMask = ");
    Serial.println(motorMask, BIN);
  */
    // ������������� ������������� "�������" �� LOW
    digitalWrite(MSHLD_DIR_LATCH_PIN, LOW);
    // �������� ��������������� �� MSHLD_DIR_SER_PIN
    shiftOut(MSHLD_DIR_SER_PIN, MSHLD_DIR_CLK_PIN, MSBFIRST, motorMask);
    //"�����������" �������, ��� ����� ������������ �������� �� �������
    digitalWrite(MSHLD_DIR_LATCH_PIN, HIGH);
  }
}

