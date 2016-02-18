/**
 * Подключаем и работаем с панелью на 4 светодиодных цифры.
 */
#include <MotorShield.h>
#include <MsTimer2.h>

/** Начальное напряжение от передачи (5 передача - самая высокая) */
static const int MSHLD_GEAR_VOLTAGE[] = {70, 90, 105, 120, 170, 255};
/** Скорость в мм/мсек от передачи (5 передача - самая высокая) */
static const float MSHLD_GEAR_FACTOR[]  = {0.03, 0.050, 0.075, 0.113, 0.170, 0.255};
/** фактор рзмеров - сколько каунтов нужно отсчитать для перемещения на 1 мм. */
#define MSHLD_COUNT_FACTOR 0.04

/** Настраиваем пины конкретных моторов. */
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
 * Конструктор спидометра
 */
Speedometer::Speedometer(uint8_t _countPin) {
  countPin = _countPin;
  pinMode(countPin, INPUT);
  clean();
}

/**
 * проверка на изменение датчика
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

/** сбрасываем параметры */
void Speedometer::clean() {
  count = 0;
  val = digitalRead(countPin);
}

/**
 * Конструктор мотора
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
  startTime = 0L;
  endTime = 0L;
}

/**
 * Корректируем напряжение на двигателе, если это необходимо.
 */
bool DcMotor::speedCorrection() {
  return false;
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
  pinMode(MSHLD_LEFT_COUNT_PIN, INPUT);
  pinMode(MSHLD_RIGHT_COUNT_PIN, INPUT);

  leftMotorNum = _leftMotorNum;
  rightMotorNum = _rightMotorNum;
  // Пин счетчика моторов
  (MSHLD_MOTORS + leftMotorNum)->speedometer = &LEFT_SPEEDOMETR;
  (MSHLD_MOTORS + rightMotorNum)->speedometer = &RIGHT_SPEEDOMETR;

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

/********************************************
 * процедура запускается каждую милисекунду
 */
void MotorShield::timeAction() {
  long time = millis();
  for (int i = 0; i < 4; i++) {
    DcMotor* motor = MSHLD_MOTORS + i;
    Speedometer* speedometer = motor->speedometer;
    bool isStop = false;

    if (speedometer) {
      // проверяем счётчик скорости.
      if (speedometer->check()) {
        isStop = motor->speedCorrection();
      }
    } else {
      // если у нас нет счётчика скорости, то ориентируемся по времени.
      isStop = motor->endTime > 0 && motor->endTime < time;
    }

    if (isStop) {
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

/** включаем моторы shild */
void MotorShield::enabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, LOW);
}

/** выключаем моторы shild */
void MotorShield::disabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, HIGH);
}

/** Останавливаем все что можно. */
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
 * Останавливаем конкретный мотор.
 */
void MotorShield::stopMotor(uint8_t nMotor) {
  DcMotor* motor = MSHLD_MOTORS + nMotor;
  setBitMask(motorMask & motor->downMask_A & motor->downMask_B);
  analogWrite(motor->powerPin, 0);
  motor->startTime = 0;
  motor->endTime = 0;
  motor->endCount = 0;
  motor->busy = false;
}

/**
 * проверяем моторы
 */
bool MotorShield::isBusy() {
  bool busy = false;
  for (int i = 0; i < 4; i++) {
    busy = busy || MSHLD_MOTORS[i].busy;
  }
  return busy;
}

/**
 * Ожидаем окончания
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

/** Останавливаем левый мотор. */
void MotorShield::leftMotorStop() {
  stopMotor(leftMotorNum);
}

/** Останавливаем правый мотор. */
void MotorShield::rightMotorStop() {
  stopMotor(rightMotorNum);
}

/**
 * Устанавливаем скорость для левого мотора
 * @speed скорость мотора в 
 */
void MotorShield::leftMotor(int8_t speed, int16_t dist) {
  motor(leftMotorNum, speed, dist);
}

/** Устанавливаем скорость для правого мотора */
void MotorShield::rightMotor(int8_t speed, int16_t dist) {
  motor(rightMotorNum, speed, dist);
}

/**
 * Устанавливаем скорость <gear> для двигателя nMotor:
 * @nMotor номер мотора
 * @gear скорость мотора в передачах:
 *  gear > 0 - вперёд;
 *  gear < 0 - назад;
 *  значения от -5 до +5
 * @dist дистанция в мм, которую надо преодолеть 
 */
void MotorShield::motor(uint8_t nMotor, int8_t gear, int16_t dist) {
  DcMotor* motor = MSHLD_MOTORS + nMotor;
  long currTime = millis();
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

  motor->startTime = currTime;
  motor->endTime = currTime + dist / MSHLD_GEAR_FACTOR[absGear];
  motor->endCount = dist * MSHLD_COUNT_FACTOR;
  motor->busy = true;

  // настраиваем спидомерер
  Speedometer* speedometer = motor->speedometer;
  if (speedometer) {
    speedometer->clean();
  }

  setSpeed(isForward, MSHLD_GEAR_VOLTAGE[absGear], motor);
}

/**
 * устанавливаем скорость и направление конкретного мотора
 */
void MotorShield::setSpeed(bool isForward, int speed, DcMotor* motor) {
  setSpeed(isForward, speed,
    motor->upMask_A,     // маска установки клемы A
    motor->downMask_A,   // маска снятия клемы A
    motor->upMask_B,     // маска установки клемы B
    motor->downMask_B,   // маска снятия клемы B
    motor->powerPin      // пин для установки скорости
  );
}

/**
 * устанавливаем скорость и направление абстрактного мотора
 */
void MotorShield::setSpeed(
  bool isForward,       // направление движения
  int speed,            // абсолютная скорость
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
  if (isForward) {
    mask = (mask & downMask_B) | upMask_A;
  } else if (speed == 0) {
    mask = mask & downMask_A & downMask_B;
  } else {
    mask = (mask & downMask_A) | upMask_B;
  }
  setBitMask(mask);

  // устанавливаем скорость
  if (speed > 255) speed = 255;
  analogWrite(powerPin, speed);
}

/** Выводим маску моторов в 74HC595 */
void MotorShield::setBitMask(uint8_t mask) {
  if (mask != motorMask) {
    motorMask = mask;
  /*
    Serial.print("motorMask = ");
    Serial.println(motorMask, BIN);
  */
    // устанавливаем синхронизацию "защелки" на LOW
    digitalWrite(MSHLD_DIR_LATCH_PIN, LOW);
    // передаем последовательно на MSHLD_DIR_SER_PIN
    shiftOut(MSHLD_DIR_SER_PIN, MSHLD_DIR_CLK_PIN, MSBFIRST, motorMask);
    //"защелкиваем" регистр, тем самым устанавливая значения на выходах
    digitalWrite(MSHLD_DIR_LATCH_PIN, HIGH);
  }
}

