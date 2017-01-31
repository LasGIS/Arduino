#ifndef SETSCREEN_H
#define SETSCREEN_H

#include "lcd_screen.h"

class SetScreen : public LcdScreen
{
public:
  SetScreen();
  void showOnce();
  void edit(char key);
};

#endif // SETSCREEN_H
