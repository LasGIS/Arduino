#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "lcd_screen.h"

class MainScreen : public LcdScreen
{
public:
  MainScreen();
  void show();
  LPModeType edit(char key);
};

#endif // MAINSCREEN_H
