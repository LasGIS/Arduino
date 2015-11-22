/**
 * 
 */
#include <IrControl.h>
#include <LiquidCrystal.h>
#include <DS1302.h>

// указываем пин для ИК датчика 
IrControl control(2);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 10, 6, 7, 8, 9);
// пин для жужалки
const int buzzerPin = A5;

// текущая команда
int curCommand = 0;
// время в миллисекундах
unsigned long milliSec;
int charsRow = 0;

const int kCePin   = 3;  // Chip Enable
const int kIoPin   = 4;  // Input/Output
const int kSclkPin = 5;  // Serial Clock

DS1302 rtc(kCePin, kIoPin, kSclkPin);

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Start");
  control.start();
  milliSec = millis();
  pinMode(buzzerPin, OUTPUT);
  
}

void loop() {
  switch (curCommand) {
    case 0:
      lcdShowTime();
      break;
    //case 1: {
    //    int resistorValue = analogRead(resistorPin);
    //    myservo.write(map(resistorValue, 0, 1023, 0, 180));
    //    panel.setValue(String((resistorValue * 180.0) / 1023, 2));
    //  }
    //  break;
    case 1:
      break;
    case 2:
      break;
    //case 4:
    //  temperatureHumidity(true);
    //  break;
    //case 5:
    //  temperatureHumidity(false);
    //  break;
    default:
      break;
  }

  if (control.hasCode()) {
    long code = control.getCode();
    IrControlKey* controlKey = control.toControlKey(code);
    char key = 0;
    if (controlKey != NULL) {
      key = controlKey->key;
      buzzerOut(controlKey->tone, 200);
    }

    serIRkey(code, key);
    if (curCommand == 2) {
      lcdIRkey(code, key);
    }

    switch (key) {
    case '>':
      lcd.scrollDisplayLeft();
      break;
    case '<':
      lcd.scrollDisplayRight();
      break;
    case '+':
      if (curCommand == 1) {
        charsRow++;
        lcdShowChars();
      }
      break;
    case '-':
      if (curCommand == 1) {
        charsRow--;
        lcdShowChars();
      }
      break;
    case 'm':
      beforeCommandSet();
      curCommand++;
      if (curCommand > 2) {
        curCommand = 0;
      }
      afterCommandSet();
      break;
    case 'b':
      beforeCommandSet();
      curCommand--;
      if (curCommand < 0) {
        curCommand = 2;
      }
      afterCommandSet();
      break;
    }
  }
  delay(100);
}

void beforeCommandSet() {
  lcd.clear();
}

void afterCommandSet() {
  if (curCommand == 1) {
//    charsRow = 0;
    lcdShowChars();
  } else if (curCommand == 2) {
    lcd.print("Enter IR key");
  }
}

void serialEvent() {
  lcd.clear();
  lcd.setCursor(0, 0);
  char buf[10];
  int j = 0;
  while (Serial.available() > 0) {
    int cnt = Serial.readBytes(buf, 10);
    for (int i = 0; i < cnt; i++, j++) {
      if (j == 15 || buf[i] == '\n') {
       lcd.setCursor(0, 1);
       j = 0;
      } else {
        lcd.write(buf[i]);
      }
    }
  }
  delay(12000);
}

/** Показываем время */
void lcdShowTime() {
  unsigned long msec = millis();
  if ((msec - milliSec) / 1000 > 0) {
    milliSec = msec;
    printTime();
  }
}

/** Показываем последовательно раскладку */
void lcdShowChars() {
  if (charsRow > 15) {
    charsRow = 0;
  }
  if (charsRow < 0) {
    charsRow = 15;
  }
  int row = charsRow;
  for (int i = 0; i < 2; i++, row++) {
    lcd.setCursor(0, i);
    if (row > 16) {
      row = 0;
    }
    for (int col = 0; col < 16; col++) {
      lcd.write(row * 16 + col);
    }
  }
}

void serIRkey(long code, char key) {
  Serial.print("IR key = ");
  if (key == 0) {
    Serial.print("NOTHIN");
  } else {
    Serial.print(key);
  }
  Serial.print("; code = 0x");
  Serial.println(code, HEX);
}

void lcdIRkey(long code, char key) {
  lcd.setCursor(0, 0);
  lcd.print("IR key = ");
  if (key == 0) {
    lcd.print("NOTHIN");
  } else {
    lcd.print(key);
    lcd.print("       ");
  }
  lcd.setCursor(0, 1);
  lcd.print("code = ");
  lcd.print(code, HEX);
  lcd.print("       ");
}

