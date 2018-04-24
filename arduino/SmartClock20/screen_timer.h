#ifndef SCREEN_TIMER_H
#define SCREEN_TIMER_H

#include "screen_tft.h"

class ScreenTimer : public ScreenTft {
public:
// --- методы ---
  ScreenTimer();
  void showTime();
//  void showEveryTime();
//  void showOnce();
  void edit(char key);
//  void control(char key);
};

#endif // SCREEN_TIMER_H
