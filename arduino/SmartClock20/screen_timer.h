#ifndef SCREEN_TIMER_H
#define SCREEN_TIMER_H

#include "screen_tft.h"

class ScreenTimer : public ScreenTft {
public:
  uint32_t startTime = 900;
  uint32_t time;
// --- методы ---
  ScreenTimer();
  virtual void changeOrientation(OrientationType orientation);
  virtual void showTime(DateTime * dateTime);
  virtual void showEveryTime();
  virtual void showOnce();
  virtual void edit(char key);
//  virtual void control(char key);
  void load();
  void save();
  void start();
};

#endif // SCREEN_TIMER_H
