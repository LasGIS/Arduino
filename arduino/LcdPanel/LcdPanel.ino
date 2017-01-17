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
const int buzzerPin = 8;

/** настраиваем real time clock. */
const int kCePin   = 3;  // Chip Enable
const int kIoPin   = 4;  // Input/Output
const int kSclkPin = 5;  // Serial Clock
DS1302 rtc(kCePin, kIoPin, kSclkPin);

// текущая команда
CurrentCommandType currentCommand;
LPModeType mode = show;
LPShowModeType showMode;
int count = 0;
#define CUR_COMMAND_ADR 0
#define SHOW_MODE_ADR 1

// начальная страница для показа раскладки символов (0-15)
int charsRow = 0;
// время в миллисекундах
unsigned long milliSec;
char comBuffer[50];

/** настраиваем измеритель влажности. */
DHT dht1(7, DHT22);
DHT dht2(6, DHT22);

/*
void SerialEEPROM() {
  //for (int i = 0; i < 255; i++) EEPROM.update(i, i);
  for (int i = 0; i < 8; i++) {
    byte b = EEPROM.read(i);
    Serial.print(b, HEX);
    if (i % 16 == 15) {
      Serial.println(";");
    } else {
      Serial.print(",");
    }
  }
  Serial.println(";");
}
*/
void setup() {
  Serial.begin(9600);
//  SerialEEPROM();  
  eepromSet();
//  SerialEEPROM();  

  // initialize the lcd 
  lcd.init();
  loadCustomChars();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  afterCommandSet();

  control.start();
  milliSec = millis();
  pinMode(buzzerPin, OUTPUT);
  dht1.begin();
  dht2.begin();
}

void eepromSet() {
  currentCommand = (CurrentCommandType) EEPROM.read(CUR_COMMAND_ADR);
  if (currentCommand < mainCommand || currentCommand > showIRkey) {
    currentCommand = mainCommand;
    EEPROM.update(CUR_COMMAND_ADR, currentCommand);
  }
  showMode = (LPShowModeType) EEPROM.read(SHOW_MODE_ADR);
  if (showMode < BigTime || showMode > Battery) {
    showMode = BigTime;
    EEPROM.update(SHOW_MODE_ADR, showMode);
  }
}

void loop() {
  switch (currentCommand) {
    case mainCommand: // показываем часы, температуру и влажность
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
    case showLCDchars: // показываем раскладку LCD символов
      break;
    case showIRkey: // показываем ключ и код ИК пульта
      break;
/*    
    case showDistance: // дистанцию
      showDistance();
      break;
*/
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
    if (currentCommand == showIRkey) {
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
      if (currentCommand == mainCommand) {
        lcd.clear();
        showMode = showMode > BigTime ? (LPShowModeType) (showMode - 1) : Battery;
        EEPROM.update(SHOW_MODE_ADR, showMode);
        count = 0;
      } else if (currentCommand == showLCDchars) {
        charsRow++;
        lcdShowChars();
      }
      break;
    case '-':
      if (currentCommand == mainCommand) {
        lcd.clear();
        showMode = showMode < Battery ? (LPShowModeType) (showMode + 1) : BigTime;
        EEPROM.update(SHOW_MODE_ADR, showMode);
        count = 0;
      } else if (currentCommand == showLCDchars) {
        charsRow--;
        lcdShowChars();
      }
      break;
    case 'p':
      if (currentCommand == mainCommand && showMode == DataTime) {
        mode = editTime(1);
      }
      break;
    case 'm':
      beforeCommandSet();
      currentCommand = currentCommand < showIRkey ? (CurrentCommandType) (currentCommand + 1) : mainCommand;
      EEPROM.update(CUR_COMMAND_ADR, currentCommand);
      afterCommandSet();
      break;
    case 'b':
      beforeCommandSet();
      currentCommand = currentCommand > mainCommand ? (CurrentCommandType) (currentCommand - 1) : showIRkey;
      EEPROM.update(CUR_COMMAND_ADR, currentCommand);
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
  switch (currentCommand) {
    case showLCDchars:
//      charsRow = 0;
      lcdShowChars();
      break;
    case showIRkey:
      lcd.print("Enter IR key");
      break;
/*
    case showDistance:
      lcd.print("Distance = ");
      break;
*/
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
  float vScheme = analogRead(A3) * 0.01175;
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
  lcd.print("V ");
  lcd.print(vScheme, 2);
  lcd.print("V ");
}

