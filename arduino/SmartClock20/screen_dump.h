#ifndef SCREEN_DUMP_H
#define SCREEN_DUMP_H

#include "screen_tft.h"

class ScreenDump : public ScreenTft {
public:
  uint8_t device = 0x57;
  uint16_t address = 0;
  // --- методы ---
  ScreenDump();
//  virtual void changeOrientation();
//  virtual void showTime(DateTime * dateTime);
//  virtual void showEveryTime();
  virtual void showOnce();
  virtual void edit(char key);
  virtual void control(char key);
  void draw();
};

#endif // SCREEN_DUMP_H
