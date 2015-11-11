/**
 * IrTest - модуль для проверки ИК пульта
 */

#include <IrTest.h>

//volatile int IrTest::count = -1;
//volatile long IrTest::times[IR_TEST_INTERRUPT_BUF_SIZE];
//volatile byte IrTest::values[IR_TEST_INTERRUPT_BUF_SIZE];

IrTest* IrTest::_testActiveObject=0;

IrTest::IrTest(int pin) {
  irPin = pin;
  pinMode(irPin, INPUT);
}

void IrTest::start() {
  attachInterrupt(
    digitalPinToInterrupt(irPin), IrTest::handle_interrupt, CHANGE
  );
  _testActiveObject = this;
}

void IrTest::stop() {
  detachInterrupt(digitalPinToInterrupt(irPin));
  _testActiveObject = 0;
}

void IrTest::handle_interrupt() {
  if (_testActiveObject) {
    _testActiveObject->interrupt();
  }
}

/**
 * Собираем камни.
 */
void IrTest::interrupt() {
  long time = micros() - startTime;
  startTime += time;
  if (time < IR_TEST_MINIMAL_TIME) {
    return;
  }
  byte value = digitalRead(irPin);
  if (value == HIGH && time > 3000 && time < 20000) {
    count = 0;
  }
  points[count].time = time;
  points[count].val = value;
  count++; 

  if (count >= IR_TEST_INTERRUPT_BUF_SIZE) {
    count = 0;
    //print();
  }
}


/**
 * Показываем содержимое.
 */
void IrTest::print() {
  Serial.println("; -----");
  for (int i = 0; i < IR_TEST_INTERRUPT_BUF_SIZE; i++) {
    Serial.print(i);
    Serial.print(": time = ");
    Serial.print(points[i].time, DEC);
    Serial.print("; value = ");
    Serial.print(points[i].val, DEC);
    Serial.println("; | ");
  }
  Serial.println("; -----");
}

/**
 * 
void IrTest::event() {
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

  char outChars[30];
  Serial.print("------- ");
  Serial.print(time);
  Serial.println(" ------------------------");
  for (int i = 0; i < IR_TEST_READ_BUF_SIZE; i++) {
    short val = readBuf[i];
    //Serial.println(readBuf[i] & 0xffff, BIN);    
    for (int k = 0; k < 16; k++) {
      Serial.print((val & 0x8000) ? '#' : '_');
      val = val << 1;
    }
    if (i % 8 == 0) {
      Serial.println(" |");
    }
  }
  Serial.print("------- ");
  Serial.print(time);
  Serial.println(" ------------------------");
}
 */
