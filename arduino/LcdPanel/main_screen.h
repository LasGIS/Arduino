#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "lcd_screen.h"

/* модель отображения для главного экрана */
enum LPShowModeType : uint8_t {
  BigTime = 0,
  DataTime = 1,
  TimeHum = 2,
  Humidity = 3,
  Battery = 4
};

class MainScreen : public LcdScreen {
public:
// --- данные ---
  LPShowModeType showMode;
// --- методы ---
  MainScreen();
  void showEveryTime(int count);
  void showOnce();
  void edit(char key);
  void Time2Fields(Time tm);
  Time Fields2Time();
};

#endif // MAINSCREEN_H
