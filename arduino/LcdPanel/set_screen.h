#ifndef SETSCREEN_H
#define SETSCREEN_H

#include "lcd_screen.h"

class SetScreen
{
  int zummer;
public:
  SetScreen();
  void show();
  LPModeType edit(char key);
};

#endif // SETSCREEN_H
