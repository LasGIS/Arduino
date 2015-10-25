/*namespace IRTest {
  IRTest()
};*/
#define READ_BUF_SIZE 100
#define IR_INTERRUPT_BUF_SIZE 80

const int irPin = 2;
short readBuf[READ_BUF_SIZE];

int IRTestCount = 0;
long IRTestStartTime = 0;
long IRTestTimes[IR_INTERRUPT_BUF_SIZE];
byte IRTestValues[IR_INTERRUPT_BUF_SIZE];

void initIRTest() {
  pinMode(irPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(irPin), IRTestInterrupt, CHANGE);
}

void IRTestInterrupt() {
  long time = micros() - IRTestStartTime;
  byte value = digitalRead(irPin);
  IRTestStartTime += time;
  if (value == HIGH && time > 3000 && time < 20000) {
    IRTestCount = 0;
  }
  IRTestTimes[IRTestCount] = time;
  IRTestValues[IRTestCount] = value;
  IRTestCount++;
  if (IRTestCount >= IR_INTERRUPT_BUF_SIZE) {
    IRTestCount = 0;
    //IRTestCheck();
  }
}

void IRTestCheck() {
    Serial.print("----- IRTestCount = ");
    Serial.print(IRTestCount);
    Serial.println(" -----");
    for (int i = 0; i < IR_INTERRUPT_BUF_SIZE; i++) {
      Serial.print(i);
      Serial.print(": time = ");
      Serial.print(IRTestTimes[i], DEC);
      Serial.print("; value = ");
      Serial.print(IRTestValues[i], DEC);
      Serial.println("; | ");
    }
    Serial.println("-------");
}

void IRTestEvent() {
  long time = millis();
  for (int i = 0; i < READ_BUF_SIZE; i++) {
    short val = 0;
    for (int k = 0; k < 16; k++) {
      val = val << 1;
      val |= digitalRead(irPin);
      delayMicroseconds(100);
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

