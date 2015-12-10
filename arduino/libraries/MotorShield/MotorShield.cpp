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
  stopMotor();
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
 * ������������� �������� ��������� M1:
 * speed > 0 - �����;
 * speed < 0 - �����;
 * �������� �� -255 �� +255
 */
void MotorShield::setM1(int speed) {
  setSpeed(speed,
    MSHLD_UP_M1A_MASK, MSHLD_DOWN_M1A_MASK,
	MSHLD_UP_M1B_MASK, MSHLD_DOWN_M1B_MASK,
	MSHLD_PWM2A_PIN
  );
}

/**
 * ������������� �������� ��������� M2:
 * speed > 0 - �����;
 * speed < 0 - �����;
 * �������� �� -255 �� +255
 */
void MotorShield::setM2(int speed) {
  setSpeed(speed,
    MSHLD_UP_M2A_MASK, MSHLD_DOWN_M2A_MASK,
	MSHLD_UP_M2B_MASK, MSHLD_DOWN_M2B_MASK,
	MSHLD_PWM2B_PIN
  );
}

/**
 * ������������� �������� ��������� M3:
 * speed > 0 - �����;
 * speed < 0 - �����;
 * �������� �� -255 �� +255
 */
void MotorShield::setM3(int speed) {
  setSpeed(speed,
    MSHLD_UP_M4A_MASK, MSHLD_DOWN_M4A_MASK,
	MSHLD_UP_M4B_MASK, MSHLD_DOWN_M4B_MASK,
	MSHLD_PWM0A_PIN
  );
}

/**
 * ������������� �������� ��������� M4:
 * speed > 0 - �����;
 * speed < 0 - �����;
 * �������� �� -255 �� +255
 */
void MotorShield::setM4(int speed) {
  setSpeed(speed,
    MSHLD_UP_M3A_MASK, MSHLD_DOWN_M3A_MASK,
	MSHLD_UP_M3B_MASK, MSHLD_DOWN_M3B_MASK,
	MSHLD_PWM0B_PIN
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
  if (mask != motorMask) {
    motorMask = mask;
    setBitMask();
  }

  // ������������� ��������
  if (speed > 255) speed = 255;
  analogWrite(powerPin, speed);
}

/** ������� ����� ������� � 74HC595 */
void MotorShield::setBitMask() {
/*
  Serial.print(motorMask, BIN);
  Serial.print(" - ");
  Serial.println(motorMask, HEX);
*/
  // ������������� ������������� "�������" �� LOW
  digitalWrite(MSHLD_DIR_LATCH_PIN, LOW);
  // �������� ��������������� �� MSHLD_DIR_SER_PIN
  shiftOut(MSHLD_DIR_SER_PIN, MSHLD_DIR_CLK_PIN, MSBFIRST, motorMask);
  //"�����������" �������, ��� ����� ������������ �������� �� �������
  digitalWrite(MSHLD_DIR_LATCH_PIN, HIGH);
}

