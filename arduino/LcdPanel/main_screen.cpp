#include "LcdPanel.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include <DHT.h>
#include "main_screen.h"

#define RT_MAX_FIELDS 6

extern LPModeType mode;
// время в миллисекундах
extern unsigned long milliSec;
// отсчет в главном цикле
extern int count;
extern char comBuffer[50];
extern DS1302 rtc;
extern LiquidCrystal_I2C lcd;
extern DHT dht1;
extern DHT dht2;

const char* dayAsString(const uint16_t val) {
  Time::Day day = (Time::Day) val;
  switch (day) {
    case Time::kSunday: return "Sun";
    case Time::kMonday: return "Mon";
    case Time::kTuesday: return "Tue";
    case Time::kWednesday: return "Wed";
    case Time::kThursday: return "Thu";
    case Time::kFriday: return "Fri";
    case Time::kSaturday: return "Sat";
  }
  return "(und)";
}

MainScreen::MainScreen(): LcdScreen() {
  maxFields = RT_MAX_FIELDS;
  fields = new LcdField[maxFields + 1];
  fields[0] = {0, 0,  1, 1, 7, 1, dayAsString};   // день недели
  fields[1] = {0, 4,  4, 1, 9999, 2015, NULL};    // год
  fields[2] = {0, 9,  2, 1, 12, 1, NULL};         // месяц
  fields[3] = {0, 12, 2, 1, 31, 1, NULL};         // день месяца
  fields[4] = {1, 7,  2, 0, 23, 0, NULL};         // час
  fields[5] = {1, 10, 2, 0, 59, 0, NULL};         // минута
  fields[6] = {1, 13, 2, 0, 59, 0, NULL};         // секунда

  showMode = (LPShowModeType) EEPROM.read(SHOW_MODE_ADR);
  if (showMode > Battery) {
    showMode = BigTime;
    EEPROM.update(SHOW_MODE_ADR, showMode);
  }
}

void MainScreen::showEveryTime() {
#ifdef HAS_SERIAL
#ifdef HAS_DEBUG
  Serial.print("showMode = ");
  Serial.println(showMode);
#endif
#endif
  if (mode == show) {
    /* Показываем время */
    unsigned long msec = millis();
    if ((msec - milliSec) / 100 > 0) {
      milliSec = msec;
      showOnce();
    }
    if (count % 20 == 1) {
      if (showMode == TimeHum || showMode == Humidity) {
        temperatureHumidity();
      } else if (showMode == Battery) {
        batteryCapasity();
      }
    }
  }
}

/** временнАя часть. */
void printOnlyTime(uint8_t row, Time* t) {
  snprintf(comBuffer, sizeof(comBuffer), "Time = %02d:%02d:%02d", t->hr, t->min, t->sec);
  lcd.setCursor(0, row);
  lcd.print(comBuffer);
}

/**
 * выводим время и дату на LCD.
 */
void MainScreen::showOnce() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  const char* day = dayAsString(t.day);

  switch (showMode) {
  case BigTime:
    viewCustomDigit(0, t.hr / 10);
    viewCustomDigit(4, t.hr % 10);
    viewCustomDigit(9, t.min / 10);
    viewCustomDigit(13, t.min % 10);
    lcd.setCursor(7, 1);
    snprintf(comBuffer, sizeof(comBuffer), "%02d", t.sec);
    lcd.print(comBuffer);
    break;
  case DataTime:
    snprintf(comBuffer, sizeof(comBuffer), "%s %04d-%02d-%02d ", day, t.yr, t.mon, t.date);
    lcd.setCursor(0, 0);
    lcd.print(comBuffer);
    printOnlyTime(1, &t);
    break;
  case TimeHum:
    printOnlyTime(0, &t);
    break;
  default:
    break;
  }
}

void MainScreen::Time2Fields(Time tm) {
  fields[0].val = tm.day;
  fields[1].val = tm.yr;
  fields[2].val = tm.mon;
  fields[3].val = tm.date;

  fields[4].val = tm.hr;
  fields[5].val = tm.min;
  fields[6].val = tm.sec;
}

Time MainScreen::Fields2Time() {
  Time tm(2099, 1, 1, 0, 0, 0, Time::kSunday);
  tm.day = (Time::Day) fields[0].val;
  tm.yr = fields[1].val;
  tm.mon = fields[2].val;
  tm.date = fields[3].val;

  tm.hr = fields[4].val;
  tm.min = fields[5].val;
  tm.sec = fields[6].val;
  return tm;
}

/**
 * Редактирование времени
 */
void MainScreen::edit(char key) {
  switch(key) {
  case 1: // начальная
    Time2Fields(rtc.time());
    break;
  case 'p': // записываем и выходим
    // initialize real time clock.
    rtc.writeProtect(false);
    rtc.halt(false);
    rtc.time(Fields2Time());
  case 'b': // выходим без записи
    break;
  }
  LcdScreen::edit(key);
}

void MainScreen::control(char key) {
  switch (key) {
  case '+':
    lcd.clear();
    showMode = showMode > BigTime ? (LPShowModeType) (showMode - 1) : Battery;
    EEPROM.update(SHOW_MODE_ADR, showMode);
    count = 0;
    break;
  case '-':
    lcd.clear();
    showMode = showMode < Battery ? (LPShowModeType) (showMode + 1) : BigTime;
    EEPROM.update(SHOW_MODE_ADR, showMode);
    count = 0;
    break;
  case 'p':
    if (showMode == DataTime) {
      edit(1);
    }
    break;
  }
}

/**
 * Показываем температуру и влажность
 */
void MainScreen::temperatureHumidity() {
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
  default:
    break;
  }
}

/**
 * Показываем температуру и влажность
 */
void MainScreen::temperatureHumidity(DHT * dht, char n) {
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

/**
 * Показываем уровень заряда батареи
 */
void MainScreen::batteryCapasity() {
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
