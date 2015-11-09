/**
 * IrControl - модуль для подключения ИК пульта
 */

/*
#define READ_BUF_SIZE 400
short readBuf[READ_BUF_SIZE];
*/
const int irPin = 2;

#define IR_INTERRUPT_BUF_SIZE 66

#define IR_CONTROL_MINIMAL_TIME 200
#define IR_CONTROL_MAXIMAL_TIME 20000
#define IR_CONTROL_MINIMAL_COUNT 14
#define IR_CONTROL_MAXIMAL_COUNT 1300
#define IR_CONTROL_BOUND_COUNT 80

//volatile int IRTestCount = -1;
volatile long IRTestStartTime = 0;
//volatile long IRTestTimes[IR_INTERRUPT_BUF_SIZE];
//volatile byte IRTestValues[IR_INTERRUPT_BUF_SIZE];

void initIRTest() {
  pinMode(irPin, INPUT);
  startIRTest();
}
void startIRTest() {
  attachInterrupt(digitalPinToInterrupt(irPin), IRTestInterrupt, CHANGE);
}
void stopIRTest() {
  detachInterrupt(digitalPinToInterrupt(irPin));
}

void IRTestInterrupt() {
  long time = micros() - IRTestStartTime;
  IRTestStartTime += time;
  timePanel++;
  if (time < IR_CONTROL_MINIMAL_TIME || time > IR_CONTROL_MAXIMAL_TIME) {
    return;
  }
  byte value = digitalRead(irPin);
  if (value == HIGH && time > 3000 && time <= IR_CONTROL_MAXIMAL_TIME) {
    noInterrupts();
    int count = IR_CONTROL_MAXIMAL_COUNT;
    if (
      //(count = IRTestWhite(LOW)) < IR_CONTROL_MAXIMAL_COUNT &&
      (count = IRTestWhite(HIGH)) < IR_CONTROL_MAXIMAL_COUNT
    ) {
      IRTestDecode();
    }
    interrupts();
  }
}

boolean IRTestDecode() {
  long value = 0;
  for (int i = 0; i < 32; i++) {
    int count = IRTestWhite(LOW);
    if (count < IR_CONTROL_MINIMAL_COUNT || count >= IR_CONTROL_BOUND_COUNT) {
      /* IRTestCount = i;
      IRTestValue = count;
      IRTestHasValue = true; */
      return false;
    }
    count = IRTestWhite(HIGH);
    if (count < IR_CONTROL_MINIMAL_COUNT || count >= IR_CONTROL_MAXIMAL_COUNT) {
      /*IRTestCount = i + 1000;
      IRTestValue = count;
      IRTestHasValue = true;*/
      return false;
    }
    value <<= 1;
    value |= (count > IR_CONTROL_BOUND_COUNT) ? 1 : 0;
  }
  IRTestValue = value;
  IRTestHasValue = true;
  return true;
}

/** пропускаем пока и считаем время */
int IRTestWhite(byte val) {
  int count = 0;
  while (digitalRead(irPin) == val && count < IR_CONTROL_MAXIMAL_COUNT) {
    delayMicroseconds(10);
    count ++;
  }
  return count;
}

/*
void IRTestCheck() {
  Serial.print("----- timePanel = ");
  Serial.print(timePanel);
  Serial.println("; -----");
  for (int i = 0; i < IR_INTERRUPT_BUF_SIZE; i++) {
    Serial.print(i);
    Serial.print(": time = ");
    Serial.print(IRTestTimes[i], DEC);
    Serial.print("; value = ");
    Serial.print(IRTestValues[i], DEC);
    Serial.println("; | ");
  }
  Serial.print("----- timePanel = ");
  Serial.print(timePanel);
  Serial.println("; -----");
  timePanel=0;
}
void IRTestEvent() {
  long time = millis();
  for (int i = 0; i < READ_BUF_SIZE; i++) {
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
  for (int i = 0; i < READ_BUF_SIZE; i++) {
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
/*
void IRTestRead() {
  char outChars[30];
  long time = millis();
  for (int i = 0; i < READ_BUF_SIZE; i++) {
    readBuf[i] = digitalRead(irPin);
    delayMicroseconds(100);
  }
  time = millis() - time;

  Serial.print("------- ");
  Serial.print(time);
  Serial.println(" ------------------------");
  for (int i = 0; i < READ_BUF_SIZE; i++) {
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
