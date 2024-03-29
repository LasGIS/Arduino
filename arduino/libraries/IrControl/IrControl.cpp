/**
 * IrControl - ������ ��� ����������� �� ������
 */
#include <IrControl.h>

IrControl* IrControl::_activeIrControlObject = 0;

#ifdef FIRST_KEYBOARD
IrControlKey irControlKeyMap[] {
  {'q', 0xFFA25D, 220}, // Quit
  {'_', 0xFF629D, 233},
  {'m', 0xFFE21D, 247}, // Menu
  {'t', 0xFF22DD, 262}, // Test
  {'+', 0xFF02FD, 277}, // <+>
  {'b', 0xFFC23D, 294}, // Return
  {'<', 0xFFE01F, 311}, // Back
  {'p', 0xFFA857, 330}, // Play
  {'>', 0xFF906F, 349}, // Forward
  {'0', 0xFF6897, 370},
  {'-', 0xFF9867, 392}, // <->
  {'C', 0xFFB04F, 415}, // Clear
  {'1', 0xFF30CF, 440},
  {'2', 0xFF18E7, 466},
  {'3', 0xFF7A85, 494},
  {'4', 0xFF10EF, 523},
  {'5', 0xFF38C7, 554},
  {'6', 0xFF5AA5, 587},
  {'7', 0xFF42BD, 622},
  {'8', 0xFF4AB5, 659},
  {'9', 0xFF52AD, 698},
};
#endif
#ifdef PLAY_KEYBOARD
IrControlKey irControlKeyMap[] {
  {'Q', 0xFFA25D, 220}, // Quit
  {'S', 0xFF629D, 233}, // Stop
  {'U', 0xFFE21D, 247}, // mUte
  {'M', 0xFF22DD, 262}, // Mode
  {'R', 0xFF02FD, 277}, // Return
  {'E', 0xFFC23D, 294}, // EQ
  {'<', 0xFFE01F, 311}, // tape Back
  {'>', 0xFFA857, 330}, // tape Forward
  {'P', 0xFF906F, 349}, // tape Play
  {'-', 0xFF6897, 370}, // <-> ����������
  {'+', 0xFF9867, 392}, // <+> ����������
  {'0', 0xFFB04F, 415},
  {'1', 0xFF30CF, 440},
  {'2', 0xFF18E7, 466},
  {'3', 0xFF7A85, 494},
  {'4', 0xFF10EF, 523},
  {'5', 0xFF38C7, 554},
  {'6', 0xFF5AA5, 587},
  {'7', 0xFF42BD, 622},
  {'8', 0xFF4AB5, 659},
  {'9', 0xFF52AD, 698},
};
#endif
#ifdef RUBBER_KEYBOARD
IrControlKey irControlKeyMap[] {
  {'Q', 0xFFA25D, 220}, // Quit
  {'E', 0xFF629D, 233}, // mEnu
  {'U', 0xFFE21D, 247}, // mUte
  {'M', 0xFF22DD, 262}, // Mode
  {'+', 0xFF02FD, 277}, // <+> ����������
  {'R', 0xFFC23D, 294}, // Return
  {'<', 0xFFE01F, 311}, // tape Back
  {'P', 0xFFA857, 330}, // tape Play
  {'>', 0xFF906F, 349}, // tape Forward
  {'0', 0xFF6897, 370},
  {'-', 0xFF9867, 392}, // <-> ����������
  {'K', 0xFFB04F, 415}, // OK
  {'1', 0xFF30CF, 440},
  {'2', 0xFF18E7, 466},
  {'3', 0xFF7A85, 494},
  {'4', 0xFF10EF, 523},
  {'5', 0xFF38C7, 554},
  {'6', 0xFF5AA5, 587},
  {'7', 0xFF42BD, 622},
  {'8', 0xFF4AB5, 659},
  {'9', 0xFF52AD, 698},
};
#endif

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
  if ((time < IR_CONTROL_MINIMAL_TIME) || (time > IR_CONTROL_MAXIMAL_TIME) || _hasCode) {
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
        /* ������� � ����������� �������� */
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
 * ���������� ���� ������������� val � ������� �����
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
 * �������� ����� �� ����
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
 * �������� ControlKey �� ����
 */
IrControlKey* IrControl::toControlKey(long code) {
  for (unsigned int i = 0; i < sizeof(irControlKeyMap) / sizeof(IrControlKey); i++) {
    if (irControlKeyMap[i].code == code) {
      return irControlKeyMap + i;
    }
  }
  return 0;
}
