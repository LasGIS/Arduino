#ifndef LCDSCREEN_H
#define LCDSCREEN_H

enum LPModeType : uint8_t {
  show, edit
};

class LcdField {
public:
// --- данные ---
  uint8_t row;
  uint8_t col;
  uint8_t len;
  uint16_t minVal;
  uint16_t maxVal;
  uint16_t val;
  String (*getValue) (const uint16_t val);
// --- методы ---
  void setValue(int nPosit, char key);
  void showField(int nPosit);
};

class LcdScreen {
public:
// --- данные ---
  uint8_t maxFields;
  uint8_t nField = 0;
  int8_t nPosit = 0;
  LcdField * fields;
// --- методы ---
  LcdScreen();
  virtual void show();
  virtual LPModeType edit(char key);
};

#endif // LCDSCREEN_H
