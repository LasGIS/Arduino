#ifndef DATE_TIME_SCREEN_H
#define DATE_TIME_SCREEN_H

#include "screen_tft.h"

class ScreenDateTime : public ScreenTft {
public:
// --- методы ---
  ScreenDateTime();
  virtual void showTime(DateTime * dateTime);
  virtual void showEveryTime();
  virtual void showOnce();
  virtual void edit(char key);
//  virtual void control(char key);
  void load();
  void save();
};

#endif // DATE_TIME_SCREEN_H
