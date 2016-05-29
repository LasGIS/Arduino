/**
 * Проверяем:
 * real time часы + ик пульт + датчик влажности + жужалка
 */
#include "LcdPanel.h"
#include <IrControl.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include <DHT.h>

// указываем пин для ИК датчика 
IrControl control(2);
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3F, 16, 2);
// пин для жужалки
const int buzzerPin = A3;

/** настраиваем real time clock. */
const int kCePin   = 3;  // Chip Enable
const int kIoPin   = 4;  // Input/Output
const int kSclkPin = 5;  // Serial Clock
DS1302 rtc(kCePin, kIoPin, kSclkPin);

// текущая команда
int curCommand = 0;
int count = 0;
#define COMMAND_MAX 3

// начальная страница для показа раскладки символов (0-15)
int charsRow = 0;
// 
LPMode mode = show;
// время в миллисекундах
unsigned long milliSec;

/** настраиваем измеритель влажности. */
DHT dht(A2, DHT11);

/* пины Ультразвукового дальномера */
int echoPin = A1; 
int trigPin = A0; 

void setup() {
  Serial.begin(9600);
  // initialize the lcd 
  lcd.init();
  loadCustomChars();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  //lcd.print("Start");
  //viewCustomDigit(0, 2); 
  //viewCustomDigit(4, 3); 
  //viewCustomDigit(9, 5);
  //viewCustomDigit(13, 9);

  control.start();
  milliSec = millis();
  pinMode(buzzerPin, OUTPUT);
  dht.begin();
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
}

void loop() {
  static LPShowModeType showMode = BigTime;
  switch (curCommand) {
    case 0: // показываем часы, температуру и влажность
      if (mode == show) {
        lcdShowTime(showMode);
        temperatureHumidity(showMode);
      }
      break;
    case 1: // показываем раскладку LCD символов
      break;
    case 2: // показываем ключ и код ИК пульта
      break;
    case 3: // дистанцию
      showDistance();
      break;
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
    if (mode == edit) {
      mode = editTime(key);
      return;
    }
    
    serIRkey(code, key);
    if (curCommand == 2) {
      lcdIRkey(code, key);
    }
    static bool isLcdBacklight = true;
    switch (key) {
    case 'q':
      if (isLcdBacklight) {
        lcd.noBacklight();
        isLcdBacklight = false;
      } else {
        lcd.backlight();
        isLcdBacklight = true;
      }
      break;
    case '>':
      lcd.scrollDisplayLeft();
      break;
    case '<':
      lcd.scrollDisplayRight();
      break;
    case '+':
      if (curCommand == 0) {
        lcd.clear();
        showMode = showMode > BigTime ? (LPShowModeType) (showMode - 1) : BigTime;
      } else if (curCommand == 1) {
        charsRow++;
        lcdShowChars();
      }
      break;
    case '-':
      if (curCommand == 0) {
       lcd.clear();
       showMode = showMode < Humidity ? (LPShowModeType) (showMode + 1) : Humidity;
      } else if (curCommand == 1) {
        charsRow--;
        lcdShowChars();
      }
      break;
    case 'p':
      if (curCommand == 0) {
        mode = editTime(1);
      }
      break;
    case 'm':
      beforeCommandSet();
      curCommand++;
      if (curCommand > COMMAND_MAX) {
        curCommand = 0;
      }
      afterCommandSet();
      break;
    case 'b':
      beforeCommandSet();
      curCommand--;
      if (curCommand < 0) {
        curCommand = COMMAND_MAX;
      }
      afterCommandSet();
      break;
    }
  }
  count++;
  if (count > 1000) count = 0;
  delay(100);
}

void beforeCommandSet() {
  lcd.clear();
}

void afterCommandSet() {
  switch (curCommand) {
    case 1:
//      charsRow = 0;
      lcdShowChars();
      break;
    case 2:
      lcd.print("Enter IR key");
      break;
    case 3:
      lcd.print("Distance = ");
      break;
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
  delay(2000);
}

/** Показываем время */
void lcdShowTime(LPShowModeType showMode) {
  unsigned long msec = millis();
  if ((msec - milliSec) / 100 > 0) {
    milliSec = msec;
    printTime(showMode);
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

/**
 * Показываем полученное значение ИК пульта в Serial
 */
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

/**
 * Показываем полученное значение ИК пульта на дисплее
 */
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

/**
 * Показываем температуру и влажность
 */
void temperatureHumidity(LPShowModeType showMode) {
  if (count % 20 == 0) {
    delay(100);
    double h = dht.readHumidity();
    double t = dht.readTemperature();
    double hic = dht.computeHeatIndex(t, h, false);

    if (showMode == TimeHum) {
      lcd.setCursor(0, 1);
      lcd.print("T=");
      lcd.print(t, 1);
      lcd.print("C ");
      lcd.print("H=");
      lcd.print(h, 1);
      lcd.print("% ");
      lcd.print("I=");
      lcd.print(hic, 2);
      lcd.print("C ");
    } else if (showMode == Humidity) {
      lcd.setCursor(0, 0);
      lcd.print("Tem ");
      lcd.print(t, 1);
      lcd.print("C/");
      lcd.print(hic, 2);
      lcd.print("C ");
      lcd.setCursor(0, 1);
      lcd.print("Humidity ");
      lcd.print(h, 1);
      lcd.print("%    ");
    }
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.println(" *C ");
  }
}

