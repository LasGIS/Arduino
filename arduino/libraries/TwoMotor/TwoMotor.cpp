/**
 * Подключаем заголовки
 */
#include "TwoMotor.h"
#include "MsTimer2.h"


/** Начальное напряжение от передачи (5 передача - самая высокая) */
static const uint16_t MSHLD_GEAR_VOLTAGE[6] = {33, 50, 74, 111, 167, 250};
/** Скорость в мм/мсек от передачи (5 передача - самая высокая) */
static const float MSHLD_GEAR_FACTOR[6] = {0.094, 0.141, 0.212, 0.317, 0.476, 0.600};
/** фактор рзмеров - сколько каунтов нужно отсчитать для перемещения на 1 мм. */
#define MSHLD_COUNT_FACTOR 0.2
#define MSHLD_SPEED_FACTOR 1.02

/** Настраиваем пины конкретных моторов. */
static DcMotor MSHLD_MOTORS[4] = {
  DcMotor('A', MSHLD_UP_M1A_MASK, MSHLD_DOWN_M1A_MASK, MSHLD_UP_M1B_MASK, MSHLD_DOWN_M1B_MASK, MSHLD_PWM2A_PIN),
  DcMotor('B', MSHLD_UP_M2A_MASK, MSHLD_DOWN_M2A_MASK, MSHLD_UP_M2B_MASK, MSHLD_DOWN_M2B_MASK, MSHLD_PWM2B_PIN),
  DcMotor('C', MSHLD_UP_M4A_MASK, MSHLD_DOWN_M4A_MASK, MSHLD_UP_M4B_MASK, MSHLD_DOWN_M4B_MASK, MSHLD_PWM0A_PIN),
  DcMotor('D', MSHLD_UP_M3A_MASK, MSHLD_DOWN_M3A_MASK, MSHLD_UP_M3B_MASK, MSHLD_DOWN_M3B_MASK, MSHLD_PWM0B_PIN)
};

static Speedometer LEFT_SPEEDOMETR(MSHLD_LEFT_COUNT_PIN);
static Speedometer RIGHT_SPEEDOMETR(MSHLD_RIGHT_COUNT_PIN);

void leftSpeedometrInterrupt(void) {
  LEFT_SPEEDOMETR.interrupt();
}

void rightSpeedometrInterrupt(void) {
  RIGHT_SPEEDOMETR.interrupt();
}

TwoMotor* TwoMotor::_activeMotorShieldObject = 0;

/**
 * Конструктор спидометра
 */
Speedometer::Speedometer(uint8_t _countPin) {
  countPin = _countPin;
  pinMode(countPin, INPUT);
  clean();
}

/** сбрасываем параметры */
void Speedometer::clean() {
  count = 0;
  lastTime = 0L;
  lastTimeDown = 0L;
  timeInterval = 0L;
  val = digitalRead(countPin);
}

/** обрабатываем прерывание на изменение счётчика */
void Speedometer::interrupt() {
  long time = micros();
  uint8_t newVal = digitalRead(countPin);
  // проверка на дребезг 
  if (newVal != val && time - lastTime > 500) {
    val = newVal;
    lastTime = time;
    count++;
    isChange = true;
    if (val == LOW) {
      timeInterval = time - lastTimeDown;
      lastTimeDown = time;
    }
  } else if (time - lastTime > 20000) {
    // счётчик долго находится в одном положении
    lastTime = time;
    isChange = true;
  }
  isChange = false;
}

/**
 * Конструктор мотора
 */
DcMotor::DcMotor(
  char _name,
  uint8_t _upMask_A,
  uint8_t _downMask_A,
  uint8_t _upMask_B,
  uint8_t _downMask_B,
  uint8_t _powerPin
) {
  name = _name;
  upMask_A   = _upMask_A;
  downMask_A = _downMask_A;
  upMask_B   = _upMask_B;
  downMask_B = _downMask_B;
  powerPin   = _powerPin;
  startTime = 0L;
  endTime = 0L;
  endCount = 0;
  currPower = 0;
  currGear = 0;
  busy = false;
}

/**
 * Корректируем напряжение на двигателе, если это необходимо.
 * @time текущее время
 */
bool DcMotor::speedCorrection(long time) {
  int16_t count = speedometer->count;
  uint16_t power = MSHLD_GEAR_VOLTAGE[currGear];
  int16_t countMust = (int16_t) (endCount * (time - startTime) / (endTime - startTime));
#ifdef MSHLD_DEBUG_MODE
  Serial.print("countMust=");
  Serial.print(countMust);
  Serial.print("; count=");
  Serial.print(count);
  Serial.print("; timeInterval=");
  Serial.print(speedometer->timeInterval);
#endif
  int dcount = countMust - count;
  if (dcount > 2) {
    if (count == 0) {
      currPower = 255;
    } else {
      currPower = power + dcount * 10;
    }
  } else if (dcount < 2) {
    if (endCount > 8 && endCount - count < 3) {
      currPower = 0;
    } else {
      currPower = power + dcount * 10;
    }
  } else {
    currPower = power;
  }
  if (power != currPower) {
    setPower();
  }
#ifdef MSHLD_DEBUG_MODE
  Serial.println();
#endif
  return count >= endCount || time > endTime;
}

/** Устанавливаем скорость. */
void DcMotor::setPower() {
#ifdef MSHLD_DEBUG_MODE
  Serial.print("; Power=");
  Serial.print(currPower);
#endif
  if (currPower > 255) currPower = 255;
  else if (currPower > 0 && currPower < 50) currPower = 50;
  analogWrite(powerPin, currPower);
}

/**
 * показывем параметры старта.
 */
void DcMotor::showStartParameters() {
#ifdef MSHLD_DEBUG_MODE
	Serial.print("time=");
	Serial.print(endTime - startTime);
	Serial.print("; endCount=");
	Serial.print(endCount);
	Serial.print("; Power=");
	Serial.println(currPower);
#endif
}

/**
 * Конструктор с инициализатором
 */
TwoMotor::TwoMotor(uint8_t _leftMotorNum, uint8_t _rightMotorNum) {
  //устанавливаем режим OUTPUT
  pinMode(MSHLD_DIR_LATCH_PIN, OUTPUT);
  pinMode(MSHLD_DIR_CLK_PIN, OUTPUT);
  pinMode(MSHLD_DIR_SER_PIN, OUTPUT);
  pinMode(MSHLD_DIR_EN_PIN, OUTPUT);

  pinMode(MSHLD_LEFT_COUNT_PIN, INPUT);
  pinMode(MSHLD_RIGHT_COUNT_PIN, INPUT);

  leftMotor = MSHLD_MOTORS + _leftMotorNum;
  rightMotor = MSHLD_MOTORS + _rightMotorNum;
  // Пин счетчика моторов
  leftMotor->speedometer = &LEFT_SPEEDOMETR;
  rightMotor->speedometer = &RIGHT_SPEEDOMETR;

  // устанавливаем enabled
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
 * процедура запускается каждую милисекунду *
 ********************************************/
void TwoMotor::timeAction() {
  timeAction(leftMotor);
  timeAction(rightMotor);
}

/***
 * обработка очередного мотора
 */
void TwoMotor::timeAction(DcMotor* motor) {
  long time = millis();
  if (motor->endTime == 0) {
	  return;
  }
  Speedometer* speedometer = motor->speedometer;
  bool isStop = false;

  if (speedometer) {
    // проверяем счётчик скорости.
    if (speedometer->isChange) {
      isStop = motor->speedCorrection(time);
    }
  } else {
    // если у нас нет счётчика скорости, то ориентируемся по времени.
    isStop = motor->endTime < time;
  }

  if (isStop) {
#ifdef MSHLD_DEBUG_MODE
    Serial.print("stopMotor(");
    Serial.print(motor->name);
    if (speedometer) {
      Serial.print("); count = ");
      Serial.print(speedometer->count);
	    Serial.print(";\t RealTime = ");
	    Serial.print(time - motor->startTime);
	    Serial.print(";\t Time = ");
	    Serial.print(time);
	    Serial.println(";");
    } else {
      Serial.println(")");
    }
#endif
    stopMotor(motor);
  }
}

/** включаем моторы shild */
void TwoMotor::enabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, LOW);
}

/** выключаем моторы shild */
void TwoMotor::disabled() {
  digitalWrite(MSHLD_DIR_EN_PIN, HIGH);
}

/** Останавливаем все что можно. */
void TwoMotor::stopMotors() {
  setBitMask(0);
  analogWrite(MSHLD_PWM2A_PIN, 0);
  analogWrite(MSHLD_PWM2B_PIN, 0);
  analogWrite(MSHLD_PWM0A_PIN, 0);
  analogWrite(MSHLD_PWM0B_PIN, 0);
  leftMotorStop();
  rightMotorStop();
}

/**
 * Останавливаем конкретный мотор.
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
  // настраиваем спидомерер
  Speedometer* speedometer = motor->speedometer;
  if (speedometer) {
    speedometer->clean();
  }
}

/**
 * проверяем моторы
 */
bool TwoMotor::isBusy() {
  bool busy = false;
  for (int i = 0; i < 4; i++) {
    busy = busy || MSHLD_MOTORS[i].busy;
  }
  return busy;
}

/**
 * Ожидаем окончания
 */
bool TwoMotor::waitBusy() {
  for (int i = 0; i < 10000; i++) {
    if (!isBusy()) {
      return true;
    }
    delay(MSHLD_DEL_TIME);
  }
  return false;
}

/** Останавливаем левый мотор. */
void TwoMotor::leftMotorStop() {
  stopMotor(leftMotor);
}

/** Останавливаем правый мотор. */
void TwoMotor::rightMotorStop() {
  stopMotor(rightMotor);
}

/**
 * Устанавливаем скорость для левого мотора
 * @speed скорость мотора в 
 */
void TwoMotor::leftMotorStart(int8_t gear, int16_t dist) {
  motor(leftMotor, gear, dist);
}

/** Устанавливаем скорость для правого мотора */
void TwoMotor::rightMotorStart(int8_t gear, int16_t dist) {
  motor(rightMotor, gear, dist);
}

/**
 * Устанавливаем скорость <gear> для двигателя:
 * @motor сам мотор
 * @gear скорость мотора в передачах:
 *  gear > 0 - вперёд;
 *  gear < 0 - назад;
 *  значения от -5 до +5
 * @dist дистанция в мм, которую надо преодолеть 
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
  motor->currPower = MSHLD_GEAR_VOLTAGE[absGear];
  motor->currGear = absGear;
  motor->busy = true;
  motor->showStartParameters();
#ifdef MSHLD_DEBUG_MODE
  Serial.print("absGear = ");
  Serial.println(absGear);
#endif

  // настраиваем спидомерер
  Speedometer* speedometer = motor->speedometer;
  if (speedometer) {
    speedometer->clean();
  }

  setSpeed(isForward, motor);
}

/**
 * устанавливаем скорость и направление конкретного мотора
 */
void TwoMotor::setSpeed(bool isForward, DcMotor* motor) {
  setSpeed(isForward, motor->currPower,
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
void TwoMotor::setSpeed(
  bool isForward,       // направление движения
  uint16_t speed,       // абсолютная скорость
  uint8_t upMask_A,     // маска установки клемы A
  uint8_t downMask_A,   // маска снятия клемы A
  uint8_t upMask_B,     // маска установки клемы B
  uint8_t downMask_B,   // маска снятия клемы B
  uint8_t powerPin      // пин для установки скорости
) {
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
void TwoMotor::setBitMask(uint8_t mask) {
  if (mask != motorMask) {
    motorMask = mask;
#ifdef MSHLD_DEBUG_MODE
    Serial.print("motorMask = ");
    Serial.println(mask, BIN);
#endif
    // устанавливаем синхронизацию "защелки" на LOW
    digitalWrite(MSHLD_DIR_LATCH_PIN, LOW);
    // передаем последовательно на MSHLD_DIR_SER_PIN
    shiftOut(MSHLD_DIR_SER_PIN, MSHLD_DIR_CLK_PIN, MSBFIRST, mask);
    //"защелкиваем" регистр, тем самым устанавливая значения на выходах
    digitalWrite(MSHLD_DIR_LATCH_PIN, HIGH);
  }
}
