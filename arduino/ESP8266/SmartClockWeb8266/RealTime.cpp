#include "HardwareSerial.h"
#include "SmartClockWeb8266.h"

DS3231 Clock;

String bufTime("xx:xx:xx");
String bufDate("xx.xx.20xx");

const char* dayOfWeekName(const unsigned int dayOfWeek) {
  //static const char* weekNames[] = { "Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота" };
  static const char* weekNames[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

  return dayOfWeek < 7 ? weekNames[dayOfWeek] : "непонятно";
}

void saveRealTime(DateTime* dateTime) {
  Clock.setClockMode(false);  // 24 часа mode
  Clock.setSecond(dateTime->second());
  Clock.setMinute(dateTime->minute());
  Clock.setHour(dateTime->hour());
  Clock.setDate(dateTime->day());
  Clock.setMonth(dateTime->month());
  Clock.setYear(dateTime->year() - 2000);
  Clock.setDoW(dateTime->dayOfWeek());
}

void tftShowRealTime() {
  static long lastTime = 0L;
  long time = millis();
  if (lastTime != time / 1000) {
    DateTime dateTime = Clock.now();

    toTwoChar(dateTime.hour(), bufTime, 0);
    toTwoChar(dateTime.minute(), bufTime, 3);
    toTwoChar(dateTime.second(), bufTime, 6);

    toTwoChar(dateTime.day(), bufDate, 0);
    toTwoChar(dateTime.month(), bufDate, 3);
    toTwoChar(dateTime.year(), bufDate, 8);

    tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK, true);
    tft.setViewport(CLOCK_X, CLOCK_Y, CLOCK_W, CLOCK_H);
    tft.setTextDatum(TC_DATUM);
    tft.setTextSize(3);
    tft.drawString(bufTime, CLOCK_W / 2, 0, 4);
    tft.setTextSize(1);
    tft.drawString(bufDate, CLOCK_W * 1 / 4, 75, 4);
    tft.drawString(dayOfWeekName(dateTime.dayOfWeek()), CLOCK_W * 3 / 4, 75, 4);
    /*
    Serial.print(bufTime);
    Serial.print(" - ");
    Serial.print(bufDate);
    Serial.print(" - ");
    Serial.println(dayOfWeekName(dateTime.dayOfWeek()));
*/
    lastTime = time / 1000;
  }
}
