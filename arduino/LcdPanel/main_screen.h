#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "lcd_screen.h"

/* ������ ����������� ��� �������� ������ */
enum LPShowModeType : uint8_t {
  BigTime = 0,
  DataTime = 1,
  TimeHum = 2,
  Humidity = 3,
  Battery = 4
};

class MainScreen : public LcdScreen {
public:
// --- ������ ---
  LPShowModeType showMode;
// --- ������ ---
  MainScreen();
  void showEveryTime();
  void showOnce();
  void edit(char key);
  void control(char key);
  void Time2Fields(Time tm);
  Time Fields2Time();
  void temperatureHumidity();
  void temperatureHumidity(DHT * dht, char n);
  void batteryCapasity();
};

#endif // MAINSCREEN_H
