#ifndef LCDSCREEN_H
#define LCDSCREEN_H

class LcdField {
public:
  uint8_t row;
  uint8_t col;
  uint8_t len;
  uint16_t minVal;
  uint16_t maxVal;
  uint16_t val;
  String (*getValue) (const uint16_t val);
  void setValue(int nPosit, char key);
  void showField(int nPosit);
};

/*class LcdScreen {
public:
  LcdScreen();

};*/

#endif // LCDSCREEN_H
