#ifndef REALTIMESCREEN_H
#define REALTIMESCREEN_H

#include "lcd_screen.h"

class RealTimeScreen : public LcdScreen
{
public:
// --- данные ---

// --- методы ---
  RealTimeScreen();
  void show();
  LPModeType edit(char key);
  void Time2Fields(Time tm);
  Time Fields2Time();
};

#endif // REALTIMESCREEN_H
