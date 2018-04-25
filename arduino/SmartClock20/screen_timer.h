#ifndef SCREEN_TIMER_H
#define SCREEN_TIMER_H

#include "screen_tft.h"

class ScreenTimer : public ScreenTft {
public:
// --- методы ---
  ScreenTimer();
  virtual void showTime();
//  virtual void showEveryTime();
//  virtual void showOnce();
  virtual void edit(char key);
//  virtual void control(char key);
  void load();
  void save();
};

#endif // SCREEN_TIMER_H
