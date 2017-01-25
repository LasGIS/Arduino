#ifndef LCDSCREEN_H
#define LCDSCREEN_H

enum LPModeType : uint8_t {
  show, edit
};

class LcdField {
public:
  uint8_t row;
  uint8_t col;
  uint8_t len;
  uint16_t maxVal;
  uint16_t val;
};

/*class LcdScreen {
public:
  LcdScreen();

};*/

#endif // LCDSCREEN_H
