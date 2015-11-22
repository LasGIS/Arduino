/**
 * IrControl - модуль для подключения ИК пульта
 */
#include <IrControl.h>

IrControl* IrControl::_activeIrControlObject = 0;

IrControlKey irControlKeyMap[] {
  {'q', 0xFFA25D, 220}, // quit
  {'m', 0xFFE21D, 233}, // menu
  {'b', 0xFFC23D, 247}, // back
  {'t', 0xFF22DD, 262}, // test
  {'p', 0xFFA857, 277}, // play

  {'<', 0xFFE01F, 294},
  {'+', 0xFF02FD, 311},
  {'>', 0xFF906F, 330},
  {'-', 0xFF9867, 349},
  {'C', 0xFFB04F, 370},
  {'0', 0xFF6897, 392},
  {'1', 0xFF30CF, 415},
  {'2', 0xFF18E7, 440},
  {'3', 0xFF7A85, 466},
  {'4', 0xFF10EF, 494},
  {'5', 0xFF38C7, 523},
  {'6', 0xFF5AA5, 554},
  {'7', 0xFF42BD, 587},
  {'8', 0xFF4AB5, 622},
  {'9', 0xFF52AD, 659}
};

/*IrControlKey::IrControlKey(char _key, long _code) {
  key = _key;
  code = _code;
}*/

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

/**
 * получаем ControlKey из кода
 */
IrControlKey* IrControl::toControlKey(long code) {
  for (unsigned int i = 0; i < sizeof(irControlKeyMap) / sizeof(IrControlKey); i++) {
    if (irControlKeyMap[i].code == code) {
	  return irControlKeyMap + i;
	}
  }
  return 0;
}
