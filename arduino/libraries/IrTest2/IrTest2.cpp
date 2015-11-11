/**
 * IrTest - модуль для проверки ИК пульта
 */

#include <IrTest2.h>

IrTest2* IrTest2::_test2ActiveObject=0;

IrTest2::IrTest2(int pin) {
  irPin = pin;
  pinMode(irPin, INPUT);
}

void IrTest2::start() {
  attachInterrupt(
    digitalPinToInterrupt(irPin), IrTest2::handle_interrupt, CHANGE
  );
  _test2ActiveObject = this;
}

void IrTest2::stop() {
  detachInterrupt(digitalPinToInterrupt(irPin));
  _test2ActiveObject = 0;
}

void IrTest2::handle_interrupt() {
  if (_test2ActiveObject) {
    noInterrupts();
    _test2ActiveObject->event();
    interrupts();
  }
}

/**
 * Читаем и Показываем содержимое.
 */ 
void IrTest2::event() {
  long time = millis();
  for (int i = 0; i < IR_TEST_READ_BUF_SIZE; i++) {
    short val = 0;
    for (int k = 0; k < 16; k++) {
      val = val << 1;
      val |= digitalRead(irPin);
      delayMicroseconds(10);
    }
    readBuf[i] = val;
  }
  time = millis() - time;

  Serial.print("------- ");
  Serial.print(time);
  Serial.println(" -------");
  int i;
  for (i = 0; i < IR_TEST_READ_BUF_SIZE; i++) {
    short val = readBuf[i];
    //Serial.println(readBuf[i] & 0xffff, BIN);
    for (int k = 0; k < 16; k++) {
      Serial.print((val & 0x8000) ? '#' : '_');
      val = val << 1;
    }
    if (i % 8 == 7) {
      Serial.println(" |");
    }
  }
  if (i % 8 != 0) {
    Serial.println(" |");
  }
  Serial.print("------- ");
  Serial.print(time);
  Serial.println(" -------");
}

/*
void IrTest2::read() {
  char outChars[30];
  long time = millis();
  for (int i = 0; i < IR_TEST_READ_BUF_SIZE; i++) {
    readBuf[i] = digitalRead(irPin);
    delayMicroseconds(100);
  }
  time = millis() - time;

  Serial.print("------- ");
  Serial.print(time);
  Serial.println(" ------------------------");
  for (int i = 0; i < IR_TEST_READ_BUF_SIZE; i++) {
    Serial.print(readBuf[i] / 204.8);
    char outChars[30];
    int j = 0;
    for (; j < readBuf[i] / 51.2; j++) {
      outChars[j] = '.';
    }
    outChars[j++] = '#';
    outChars[j++] = 0;
    Serial.println(outChars);
  }
  Serial.print("------- ");
  Serial.print(time);
  Serial.println(" ------------------------");
}
*/