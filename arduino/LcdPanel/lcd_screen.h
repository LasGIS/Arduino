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
  char* name;
  uint8_t maxFields;
  uint8_t nField = 0;
  int8_t nPosit = 0;
  LcdField * fields;
// --- методы ---
  LcdScreen();
  /** показываем каждые 100 мс */
  virtual void showEveryTime();
  /** показываем один раз */
  virtual void showOnce();
  virtual void edit(char key);
  virtual void control(char key);
};

#endif // LCDSCREEN_H
