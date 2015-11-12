/**
 * IrControl - модуль для подключения ИК пульта
 */
#include <IrControl.h>

IrControl* IrControl::_activeIrControlObject = 0;

IrControlKey irControlKeyMap[] {
  IrControlKey('s', 0xFFA857),
  IrControlKey('r', 0xFFE01F),
  IrControlKey('f', 0xFF02FD),
  IrControlKey('l', 0xFF906F),
  IrControlKey('k', 0xFFC23D)
};

IrControlKey::IrControlKey(char _key, long _code) {
  key = _key;
  code = _code;
}

IrControl::IrControl(int pin) {
  irPin = pin;
  startTime = 0;
  pinMode(irPin, INPUT);
  start();
}

void IrControl::start() {
  attachInterrupt(digitalPinToInterrupt(irPin), IrControl::handle_interrupt, CHANGE);
  _activeIrControlObject = this;
}

void IrControl::stop() {
  detachInterrupt(digitalPinToInterrupt(irPin));
  _activeIrControlObject = 0;
}

void IrControl::handle_interrupt() {
  if (_activeIrControlObject) {
	_activeIrControlObject->interrupt();
  }
}

void IrControl::interrupt() {
  long time = micros() - startTime;
  startTime += time;
  if (time < IR_CONTROL_MINIMAL_TIME || time > IR_CONTROL_MAXIMAL_TIME) {
    return;
  }
  byte value = digitalRead(irPin);
  if (value == HIGH && time > 3000 && time <= IR_CONTROL_MAXIMAL_TIME) {
    noInterrupts();
    int count = IR_CONTROL_MAXIMAL_COUNT;
    if (
      //(count = wait(LOW)) < IR_CONTROL_MAXIMAL_COUNT &&
      (count = wait(HIGH)) < IR_CONTROL_MAXIMAL_COUNT
    ) {
      decode();
    }
    interrupts();
  }
}

bool IrControl::decode() {
  long value = 0;
  for (int i = 0; i < 32; i++) {
    int count = wait(LOW);
    if (count < IR_CONTROL_MINIMAL_COUNT || count >= IR_CONTROL_BOUND_COUNT) {
      return false;
    }
    count = wait(HIGH);
    if (count < IR_CONTROL_MINIMAL_COUNT) {
      return false;
    } else if (count >= IR_CONTROL_MAXIMAL_COUNT) {
      if (i == 16) {
        /* вариант с половинными пультами */
        code = value;
        _hasCode = true;
        return true;
      } else {
        return false;
      }
    }
    value <<= 1;
    value |= (count > IR_CONTROL_BOUND_COUNT) ? 1 : 0;
  }
  code = value;
  _hasCode = true;
  return true;
}

bool IrControl::hasCode() {
    return _hasCode;
}

unsigned long IrControl::getCode() {
    _hasCode = false;
    return code;
}

/**
 * пропускаем пока соответствует val и считаем время
 */
int IrControl::wait(byte val) {
  int count = 0;
  while (digitalRead(irPin) == val && count < IR_CONTROL_MAXIMAL_COUNT) {
    delayMicroseconds(10);
    count ++;
  }
  return count;
}

/**
 * получаем ключь из кода
 */
char IrControl::toKey(long code) {
  for (unsigned int i = 0; i < sizeof(irControlKeyMap) / sizeof(IrControlKey); i++) {
    if (irControlKeyMap[i].code == code) {
	  return irControlKeyMap[i].key;
	}
  }
  return 0;
}

