#ifndef SCREEN_TIMER_H
#define SCREEN_TIMER_H

#include "screen_tft.h"

class ScreenTimer: public ScreenTft {
public:
  int32_t startTime = 5; //900;
  int32_t time;
// --- методы ---
  ScreenTimer();
  virtual void changeOrientation();
  virtual void showTime(DateTime * dateTime);
  virtual void showOnce();
  virtual void edit(char key);
  void load();
  void save();
  void start();
};

#endif // SCREEN_TIMER_H
