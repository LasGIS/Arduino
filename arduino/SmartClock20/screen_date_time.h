#ifndef DATE_TIME_SCREEN_H
#define DATE_TIME_SCREEN_H

#include "screen_tft.h"

class ScreenDateTime : public ScreenTft {
public:
// --- методы ---
  ScreenDateTime();
  void showTime(DateTime * dateTime);
  void showEveryTime();
//  void showOnce();
  void edit(char key);
  void control(char key);
};

#endif // DATE_TIME_SCREEN_H
