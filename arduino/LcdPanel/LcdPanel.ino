/**
 * real time часы + ик пульт + датчик влажности + жужалка
 */
#include <EEPROM.h>
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
byte curCommand;
LPMode mode = show;
LPShowModeType showMode;
int count = 0;
#define COMMAND_MAX 3
#define CUR_COMMAND_ADR 0
#define SHOW_MODE_ADR 1

// начальная страница для показа раскладки символов (0-15)
int charsRow = 0;
// время в миллисекундах
unsigned long milliSec;
static char comBuffer[50];

/** настраиваем измеритель влажности. */
DHT dht1(7, DHT22);
DHT dht2(6, DHT22);

/* пины Ультразвукового дальномера */
const int echoPin = A1; 
const int trigPin = A0; 

void setup() {
  Serial.begin(9600);
  eepromSet();
/*  
  //for (int i = 0; i < 255; i++) EEPROM.update(i, i);
  for (int i = 0; i < 4; i++) {
    byte b = EEPROM.read(i);
    Serial.print(b, HEX);
    if (i % 16 == 15) {
      Serial.println(";");
    } else {
      Serial.print(",");
    }
  }
*/
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
  dht1.begin();
  dht2.begin();
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
}

void eepromSet() {
  curCommand = EEPROM.read(CUR_COMMAND_ADR);
  if (curCommand < 0 || curCommand > COMMAND_MAX) {
    curCommand = 0;
    EEPROM.update(CUR_COMMAND_ADR, curCommand);
  }
  showMode = (LPShowModeType) EEPROM.read(SHOW_MODE_ADR);
  if (showMode < 0 || showMode > 3) {
    showMode = BigTime;
    EEPROM.update(SHOW_MODE_ADR, showMode);
  }
}

void loop() {
  switch (curCommand) {
    case 0: // показываем часы, температуру и влажность
      if (mode == show) {
        lcdShowTime();
        if (count % 20 == 1) {
          if (showMode == TimeHum || showMode == Humidity) {
            temperatureHumidity();
          } else if (showMode == Battery) {
            batteryCapasity();
          }
        }
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
        showMode = showMode > BigTime ? (LPShowModeType) (showMode - 1) : Battery;
        EEPROM.update(SHOW_MODE_ADR, showMode);
        count = 0;
      } else if (curCommand == 1) {
        charsRow++;
        lcdShowChars();
      }
      break;
    case '-':
      if (curCommand == 0) {
        lcd.clear();
        showMode = showMode < Battery ? (LPShowModeType) (showMode + 1) : BigTime;
        EEPROM.update(SHOW_MODE_ADR, showMode);
        count = 0;
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
      EEPROM.update(CUR_COMMAND_ADR, curCommand);
      afterCommandSet();
      break;
    case 'b':
      beforeCommandSet();
      curCommand--;
      if (curCommand < 0) {
        curCommand = COMMAND_MAX;
      }
      EEPROM.update(CUR_COMMAND_ADR, curCommand);
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
void lcdShowTime() {
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
void temperatureHumidity() {
  switch (showMode) {
    case TimeHum:
    lcd.setCursor(0, 1);
    temperatureHumidity(&dht1, '1');
    break;
    case Humidity:
    lcd.setCursor(0, 0);
    temperatureHumidity(&dht1, '1');
    lcd.setCursor(0, 1);
    temperatureHumidity(&dht2, '2');
    break;
  }
}

/**
 * Показываем температуру и влажность
 */
void temperatureHumidity(DHT * dht, char n) {
  double h = dht->readHumidity();
  double t = dht->readTemperature();
  double hic = dht->computeHeatIndex(t, h, false);
  lcd.print("T"); lcd.print(n); lcd.print("=");
  lcd.print(t, 1);
  lcd.print("C ");
  lcd.print("H"); lcd.print(n); lcd.print("=");
  lcd.print(h, 1);
  lcd.print("% ");
  lcd.print("I"); lcd.print(n); lcd.print("=");
  lcd.print(hic, 2);
  lcd.print("C ");
}

/*
 * Показываем уровень заряда батареи
 */
void batteryCapasity() {
  static unsigned long startTime = millis();
  static float oldBat = 0.0;
  static float oldCrg = 0.0;

  analogReference(INTERNAL);
  delay(100);
  float vBattery = analogRead(A7) * 0.00630;
  float vCharger = analogRead(A6) * 0.01175;
  analogReference(DEFAULT);
  if ((oldBat > vBattery + 0.05) && (oldCrg + 0.05 < vCharger)) {
    startTime = millis();
  }
  oldBat = vBattery;
  oldCrg = vCharger;

  unsigned long milTime = (millis() - startTime) / 1000;
  int sec = milTime % 60;
  int min = (milTime / 60) % 60;
  int hour = milTime / 3600;
  snprintf(comBuffer, sizeof(comBuffer), "%d:%02d:%02d", hour, min, sec);
    
  lcd.setCursor(0, 0);
  lcd.print("Bat ");
  lcd.print(vBattery, 2);
  lcd.print("V ");
  lcd.print(comBuffer);
  
  lcd.setCursor(0, 1);
  lcd.print("Crg ");
  lcd.print(vCharger, 2);
  lcd.print("V   ");
}

