/**
 * ���������� ���������
 */
#include "TwoMotor.h"
#include "MsTimer2.h"


/** ��������� ���������� �� �������� (5 �������� - ����� �������) */
static const uint16_t MSHLD_GEAR_VOLTAGE[6] = {33, 50, 74, 111, 167, 250};
/** �������� � ��/���� �� �������� (5 �������� - ����� �������) */
static const float MSHLD_GEAR_FACTOR[6] = {0.094, 0.141, 0.212, 0.317, 0.476, 0.600};
/** ������ ������� - ������� ������� ����� ��������� ��� ����������� �� 1 ��. */
#define MSHLD_COUNT_FACTOR 0.2
#define MSHLD_SPEED_FACTOR 1.02

/** ����������� ���� ���������� �������.
static DcMotor MSHLD_MOTORS[4] = {
  DcMotor('A', MSHLD_UP_M1A_MASK, MSHLD_DOWN_M1A_MASK, MSHLD_UP_M1B_MASK, MSHLD_DOWN_M1B_MASK, MSHLD_PWM2A_PIN),
  DcMotor('B', MSHLD_UP_M2A_MASK, MSHLD_DOWN_M2A_MASK, MSHLD_UP_M2B_MASK, MSHLD_DOWN_M2B_MASK, MSHLD_PWM2B_PIN),
  DcMotor('C', MSHLD_UP_M4A_MASK, MSHLD_DOWN_M4A_MASK, MSHLD_UP_M4B_MASK, MSHLD_DOWN_M4B_MASK, MSHLD_PWM0A_PIN),
  DcMotor('D', MSHLD_UP_M3A_MASK, MSHLD_DOWN_M3A_MASK, MSHLD_UP_M3B_MASK, MSHLD_DOWN_M3B_MASK, MSHLD_PWM0B_PIN)
};
*/

static DcMotor MSHLD_LEFT_MOTOR ('L', MSHLD_UP_M4A_MASK, MSHLD_DOWN_M4A_MASK, MSHLD_UP_M4B_MASK, MSHLD_DOWN_M4B_MASK, MSHLD_PWM0A_PIN, MSHLD_LEFT_COUNT_PIN );
static DcMotor MSHLD_RIGHT_MOTOR('R', MSHLD_UP_M3A_MASK, MSHLD_DOWN_M3A_MASK, MSHLD_UP_M3B_MASK, MSHLD_DOWN_M3B_MASK, MSHLD_PWM0B_PIN, MSHLD_RIGHT_COUNT_PIN);

void leftSpeedometrInterrupt(void) {
  MSHLD_LEFT_MOTOR.interrupt();
}

void rightSpeedometrInterrupt(void) {
  MSHLD_RIGHT_MOTOR.interrupt();
}

TwoMotor* TwoMotor::_activeMotorShieldObject = 0;

/**
 * ����������� ����������
 */
Speedometer::Speedometer(uint8_t _countPin) {
  countPin = _countPin;
  pinMode(countPin, INPUT);
  clean();
}

/** ���������� ��������� */
void Speedometer::clean() {
  count = 0;
  lastTime = 0L;
  lastTimeDown = 0L;
  timeInterval = 0L;
  val = digitalRead(countPin);
}

/** ������������ ���������� �� ��������� �������� */
void Speedometer::interrupt() {
  long time = micros();
  uint8_t newVal = digitalRead(countPin);
  // �������� �� ������� 
  if (newVal != val) {
    val = newVal;
    if (time - lastTime > 500) {
      //Serial.print(".");
      lastTime = time;
      count++;
      isChange = true;
      if (val == LOW) {
        timeInterval = time - lastTimeDown;
        lastTimeDown = time;
      }
    }
  } else if (time - lastTime > 20000) {
    // ������� ����� ��������� � ����� ���������
    lastTime = time;
    isChange = true;
  } else {
    isChange = false;
  }
}

/**
 * ����������� ������
 */
DcMotor::DcMotor(
  char _name,
  uint8_t _upMask_A,
  uint8_t _downMask_A,
  uint8_t _upMask_B,
  uint8_t _downMask_B,
  uint8_t _powerPin,
  uint8_t _countPin
) : Speedometer(_countPin) {
  name = _name;
  upMask_A   = _upMask_A;
  downMask_A = _downMask_A;
  upMask_B   = _upMask_B;
  downMask_B = _downMask_B;
  powerPin   = _powerPin;
//  countPin = _countPin;
  startTime = 0L;
  endTime = 0L;
  endCount = 0;
  currPower = 0;
  currGear = 0;
  busy = false;
}

/**
 * ������������ ���������� �� ���������, ���� ��� ����������.
 * @time ������� �����
 */
bool DcMotor::speedCorrection(long time) {
  uint16_t power = MSHLD_GEAR_VOLTAGE[currGear];
  int16_t countMust = (int16_t) (endCount * (time - startTime) / (endTime - startTime));
#ifdef MSHLD_DEBUG_MODE
  Serial.print("countMust=");
  Serial.print(countMust);
  Serial.print("; count=");
  Serial.print(count);
  Serial.print("; time=");
  Serial.print(time);
  Serial.print("; timeInterval=");
  Serial.print(timeInterval);
#endif
  int dcount = countMust - count;
  if (dcount > 2 && count == 0) {
    currPower = 255;
  } else {
    currPower = power + dcount * 5;
  }
  if (power != currPower) {
    setPower();
  }
#ifdef MSHLD_DEBUG_MODE
  Serial.println();
#endif
  return count >= endCount || time > endTime;
}

/** ������������� ��������. */
void DcMotor::setPower() {
  if (currPower > 255) {
    currPower = 255;
  } else if (currPower > 0 && currPower < 50) {
    currPower = 50;
  }
#ifdef MSHLD_DEBUG_MODE
  Serial.print("; Power=");
  Serial.print(currPower);
#endif
  analogWrite(powerPin, currPower);
}

/**
 * ��������� ��������� ������.
 */
void DcMotor::showStartParameters() {
#ifdef MSHLD_DEBUG_MODE
  Serial.print("time=");
  Serial.print(endTime - startTime);
  Serial.print("; endCount=");
  Serial.print(endCount);
  Serial.print("; Power=");
  Serial.print(currPower);
  Serial.print("; currGear = ");
  Serial.println(currGear);
#endif
}

/**
 * ����������� � ���������������
 */
TwoMotor::TwoMotor() {
  //������������� ����� OUTPUT
  pinMode(MSHLD_DIR_LATCH_PIN, OUTPUT);
  pinMode(MSHLD_DIR_CLK_PIN, OUTPUT);
  pinMode(MSHLD_DIR_SER_PIN, OUTPUT);
  pinMode(MSHLD_DIR_EN_PIN, OUTPUT);

  pinMode(MSHLD_LEFT_COUNT_PIN, INPUT);
  pinMode(MSHLD_RIGHT_COUNT_PIN, INPUT);

  leftMotor = &MSHLD_LEFT_MOTOR;
  rightMotor = &MSHLD_RIGHT_MOTOR;

  // ������������� enabled
  enabled();
  stopMotors();

  attachInterrupt(digitalPinToInterrupt(MSHLD_LEFT_COUNT_PIN), leftSpeedometrInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MSHLD_RIGHT_COUNT_PIN), rightSpeedometrInterrupt, CHANGE);

  TwoMotor::_activeMotorShieldObject = this;
  MsTimer2::set(MSHLD_DEL_TIME, TwoMotor::handle_interrupt);
  MsTimer2::start();
}

void TwoMotor::handle_interrupt() {
  if (_activeMotorShieldObject) {
    _activeMotorShieldObject->timeAction();
  }
}

/********************************************
 * ��������� ����������� ������ ����������� *
 ********************************************/
void TwoMotor::timeAction() {
  timeAction(leftMotor);
  timeAction(rightMotor);
}

/***
 * ��������� ���������� ������
 */
void TwoMotor::timeAction(DcMotor* motor) {
  long time = millis();
  if (motor->endTime == 0) {
    return;
  }
  bool isStop = false;

  // ��������� ������� ��������.
  if (motor->isChange) {
    motor->isChange = false;
    Serial.print(",");
    isStop = motor->speedCorrection(time);
  } else {
    motor->isChange = false;
  }

  if (isStop) {
#ifdef MSHLD_DEBUG_MODE
    Serial.print("stopMotor(");
    Serial.print(motor->name);
    Serial.print("); count = ");
    Serial.print(motor->count);
    Serial.print(";\t RealTime = ");
    Serial.print(time - motor->startTime);
    Serial.print(";\t Time = ");
    Serial.print(time);
    Serial.println(";");
#endif
    stopMotor(motor);
  }
}

/** �������� ������ shild */
void TwoMotor::enabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, LOW);
}

/** ��������� ������ shild */
void TwoMotor::disabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, HIGH);
}

/** ������������� ��� ��� �����. */
void TwoMotor::stopMotors() {
  setBitMask(0);
  leftMotorStop();
  rightMotorStop();
}

/**
 * ������������� ���������� �����.
 */
void TwoMotor::stopMotor(DcMotor* motor) {
  setBitMask(motorMask & motor->downMask_A & motor->downMask_B);
  analogWrite(motor->powerPin, 0);
  motor->startTime = 0;
  motor->endTime = 0;
  motor->endCount = 0;
  motor->currPower = 0;
  motor->currGear = 0;
  motor->busy = false;
  // ����������� ����������
  motor->clean();
}

/**
 * ��������� ������
 */
bool TwoMotor::isBusy() {
  return leftMotor->busy || rightMotor->busy;
}

/**
 * ������� ���������
 */
bool TwoMotor::waitBusy() {
  for (int i = 0; i < 10000; i++) {
    if (!isBusy()) {
      return true;
    }
    delay(WAIT_BUSY_TIME);
  }
  return false;
}

/** ������������� ����� �����. */
void TwoMotor::leftMotorStop() {
  stopMotor(leftMotor);
}

/** ������������� ������ �����. */
void TwoMotor::rightMotorStop() {
  stopMotor(rightMotor);
}

/**
 * ������������� �������� ��� ������ ������
 * @speed �������� ������ � 
 */
void TwoMotor::leftMotorStart(int8_t gear, int16_t dist) {
  motor(leftMotor, gear, dist);
}

/** ������������� �������� ��� ������� ������ */
void TwoMotor::rightMotorStart(int8_t gear, int16_t dist) {
  motor(rightMotor, gear, dist);
}

/**
 * ������������� �������� <gear> ��� ���������:
 * @motor ��� �����
 * @gear �������� ������ � ���������:
 *  gear > 0 - �����;
 *  gear < 0 - �����;
 *  �������� �� -5 �� +5
 * @dist ��������� � ��, ������� ���� ���������� 
 */
void TwoMotor::motor(DcMotor* motor, int8_t gear, int16_t dist) {
  long startTime = millis();
  bool isForward;
  int8_t absGear;

  if (gear >= 0) {
    absGear = gear;
    isForward = true;
  } else {
    absGear = -gear;
    isForward = false;
  }
  if (absGear > 5) {
    absGear = 5;
  }

  motor->startTime = startTime;
  motor->endTime = startTime + dist / MSHLD_GEAR_FACTOR[absGear];
  motor->endCount = dist * MSHLD_COUNT_FACTOR;
  motor->currPower = 255; // MSHLD_GEAR_VOLTAGE[absGear];
  motor->isForward = isForward;
  motor->currGear = absGear;
  motor->busy = true;
  motor->showStartParameters();

  // ����������� ����������
  motor->clean();

  setSpeed(motor);
  motor->setPower();
}

/**
 * ������������� �������� � ����������� ����������� ������
 */
void TwoMotor::setSpeed(DcMotor* motor) {
  setSpeed(
    motor->isForward,    // ����������� ��������
    motor->currPower,    // ���������� ��������
    motor->upMask_A,     // ����� ��������� ����� A
    motor->downMask_A,   // ����� ������ ����� A
    motor->upMask_B,     // ����� ��������� ����� B
    motor->downMask_B    // ����� ������ ����� B
  );
}

/**
 * ������������� �������� � ����������� ������������ ������
 */
void TwoMotor::setSpeed(
  bool isForward,       // ����������� ��������
  uint16_t speed,       // ���������� ��������
  uint8_t upMask_A,     // ����� ��������� ����� A
  uint8_t downMask_A,   // ����� ������ ����� A
  uint8_t upMask_B,     // ����� ��������� ����� B
  uint8_t downMask_B    // ����� ������ ����� B
) {
  // ������������� �����������
  uint8_t mask = motorMask;
  if (isForward) {
    mask = (mask & downMask_B) | upMask_A;
  } else if (speed == 0) {
    mask = mask & downMask_A & downMask_B;
  } else {
    mask = (mask & downMask_A) | upMask_B;
  }
  setBitMask(mask);
}

/** ������� ����� ������� � 74HC595 */
void TwoMotor::setBitMask(uint8_t mask) {
  if (mask != motorMask) {
    motorMask = mask;
#ifdef MSHLD_DEBUG_MODE
    Serial.print("motorMask = ");
    Serial.println(mask, BIN);
#endif
    // ������������� ������������� "�������" �� LOW
    digitalWrite(MSHLD_DIR_LATCH_PIN, LOW);
    // �������� ��������������� �� MSHLD_DIR_SER_PIN
    shiftOut(MSHLD_DIR_SER_PIN, MSHLD_DIR_CLK_PIN, MSBFIRST, mask);
    //"�����������" �������, ��� ����� ������������ �������� �� �������
    digitalWrite(MSHLD_DIR_LATCH_PIN, HIGH);
  }
}
