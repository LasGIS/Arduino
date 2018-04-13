#ifndef TFT_SCREEN_H
#define TFT_SCREEN_H

class TftField {
public:
// --- данные ---
  uint8_t row;
  uint8_t col;
  uint8_t len;
  uint16_t minVal;
  uint16_t maxVal;
  uint16_t val;
  const char* (*getValue) (const uint16_t val);
// --- методы ---
  void setValue(int nPosit, char key);
  void showField(int nPosit);
};

class TftScreen {
public:
// --- данные ---
  uint8_t maxFields;
  uint8_t nField;
  int8_t nPosit;
  TftField * fields;
// --- методы ---
  TftScreen();
  /** показываем каждые 100 мс */
  virtual void showEveryTime();
  /** показываем один раз */
  virtual void showOnce();
  virtual void edit(char key);
  virtual void control(char key) {}
  /** вышли за пределы дозволенной области редактирования */
  virtual void hasBeyond(char key) {}
};

#endif // TFT_SCREEN_H
