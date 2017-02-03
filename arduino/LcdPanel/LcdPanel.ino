/**
 * real time часы + ик пульт + датчик влажности + жужалка
 */

#include "LcdPanel.h"
#include <IrControl.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include <DHT.h>
#include "alarm_clock.h"
#include "main_screen.h"
#include "set_screen.h"
#include "show_char_screen.h"
#include "pitches.h"

// указываем пин для ИК датчика 
IrControl control(2);
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3F, 16, 2);
// пин для жужалки
int buzzerPin = 8;

/** настраиваем real time clock. */
const int kCePin   = 3;  // Chip Enable
const int kIoPin   = 4;  // Input/Output
const int kSclkPin = 5;  // Serial Clock
DS1302 rtc(kCePin, kIoPin, kSclkPin);

// текущая команда
uint8_t currentCommand;
LPModeType mode = show;
int count = 0;

// время в миллисекундах
unsigned long milliSec;
char comBuffer[50];

/** настраиваем измеритель влажности. */
DHT dht1(7, DHT22);
DHT dht2(6, DHT22);

LcdScreen* screens[CURRENT_COMMAND_TYPE_MAX + 1] = {
  new MainScreen(),
  new SetScreen(),
  new ShowCharScreen(),
  new LcdScreen()
};

AlarmClock alarmClocks[4] = {
  AlarmClock(0),
  AlarmClock(1),
  AlarmClock(2),
  AlarmClock(3)
};

extern uint8_t buzzerfactor;

void SerialEEPROM() {
//  for (int i = 0; i < 255; i++) EEPROM.update(i, 0xfe);
  for (int i = 0; i < 64; i++) {
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

void setup() {
  Serial.begin(9600);
  SerialEEPROM();
  eepromSet();

  // initialize the lcd 
  lcd.init();
  loadCustomChars();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  screens[currentCommand]->showOnce();

  control.start();
  milliSec = millis();
  pinMode(buzzerPin, OUTPUT);
  dht1.begin();
  dht2.begin();
}

void eepromSet() {
  currentCommand = EEPROM.read(CUR_COMMAND_ADR);
  if (currentCommand > CURRENT_COMMAND_TYPE_MAX) {
    currentCommand = 0;
    EEPROM.update(CUR_COMMAND_ADR, currentCommand);
  }
  buzzerfactor = EEPROM.read(BUZZER_FACTOR_ADR);
  if (buzzerfactor > 8) {
    buzzerfactor = 1;
    EEPROM.update(BUZZER_FACTOR_ADR, buzzerfactor);
  }
}

/** проверить будильник  */
bool checkOnAlarm(Time time) {
  for (int i = 0; i < 4; i++) {
    if (alarmClocks[i].equal(time)) {
      return true;
    }
  }
  return false;
}

int music[] = {
  NOTE_F4, 1, NOTE_E4, 1, NOTE_D4, 1, NOTE_C4, 1, NOTE_G4, 3, NOTE_G4, 3,
  NOTE_F4, 1, NOTE_E4, 1, NOTE_D4, 1, NOTE_C4, 1, NOTE_G4, 3, NOTE_G4, 3,
  NOTE_F4, 1, NOTE_A4, 1, NOTE_A4, 1, NOTE_F4, 1,
  NOTE_E4, 1, NOTE_G4, 1, NOTE_G4, 1, NOTE_E4, 1,
  NOTE_D4, 1, NOTE_E4, 1, NOTE_F4, 1, NOTE_D4, 1, NOTE_C4, 3, NOTE_C4, 3,
  NOTE_F4, 1, NOTE_A4, 1, NOTE_A4, 1, NOTE_F4, 1,
  NOTE_E4, 1, NOTE_G4, 1, NOTE_G4, 1, NOTE_E4, 1,
  NOTE_D4, 1, NOTE_E4, 1, NOTE_F4, 1, NOTE_D4, 1, NOTE_C4, 3, NOTE_C4, 3
};

/**
 * @brief alarm
 */
void alarm() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("-!ALARM!-");
  uint8_t old_bf = buzzerfactor;
  buzzerfactor = 8;
  for (int i = 0; i < sizeof(music) / sizeof(int); i+=2) {
    buzzerOut(music[i], 300 * music[i + 1]);
    delay(100);
  }
  buzzerfactor = old_bf;
  //delay(2000);
  lcd.clear();
}

/** Общий цикл */
void loop() {
  LcdScreen * screen = screens[currentCommand];

  // показываем экран каждые 1/10 секунды.
  if (count % 10 == 1 && checkOnAlarm(rtc.time())) {
    alarm();
    screen->showOnce();
  }
  screen->showEveryTime();


  // если нажали кнопку
  if (control.hasCode()) {
    static bool isLcdBacklight = true;
    long code = control.getCode();
    IrControlKey* controlKey = control.toControlKey(code);
    char key = 0;
    if (controlKey != NULL) {
      key = controlKey->key;
      buzzerOut(controlKey->tone, 200);
    }
#ifdef HAS_SERIAL
    serIRkey(code, key);
#endif
    if (currentCommand == showIRkey) {
      lcdIRkey(code, key);
      SerialEEPROM();
    }

    // редактирование
    if (mode == edit) {
      screen->edit(key);
      return;
    }

    // управление экраном
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
    case 'm':
      screen = changeCurrentCommand(true);
      break;
    case 'b':
      screen = changeCurrentCommand(false);
      break;
    default:
      screen->control(key);
      if (mode == show) {
        screen->showOnce();
      }
      break;
    }
  }
  count++;
  if (count >= 1000) count = 0;
  delay(100);
}

LcdScreen * changeCurrentCommand(bool isIncrement) {
  lcd.clear();
  if (isIncrement) {
    currentCommand = currentCommand < CURRENT_COMMAND_TYPE_MAX ? currentCommand + 1 : 0;
  } else {
    currentCommand = currentCommand > 0 ? currentCommand - 1 : CURRENT_COMMAND_TYPE_MAX;
  }
  EEPROM.update(CUR_COMMAND_ADR, currentCommand);
  LcdScreen* screen = screens[currentCommand];
  screen->showOnce();
  return screen;
}

/** ввод значения извне.
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
} */

/**
 * Показываем полученное значение ИК пульта в Serial
 */
#ifdef HAS_SERIAL
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
#endif

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
