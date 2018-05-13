#include "SmartClock20.h"

ScreenDump::ScreenDump(): ScreenTft() {
  name = (char*) "Dump  ";
  maxFields = 1;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {2, 1, 2, 3, 0, 255, 0, NULL}; // device
  fields[1] = {2, 5, 2, 4, 0, 0x1fff, 0, NULL}; // address
}

void ScreenDump::showOnce() {
  ScreenTft::showOnce();
  if (mode == ModeType::show) {
    draw();
  }
}
/**
 * Рисуем страницу
 */
void ScreenDump::draw() {
  char ch[2] = "|";
  bool isHor = isHorisontalOrientation();
  uint8_t deltaText = isHor ? 1  : 3,
          startText = isHor ? 28 : 7;

  for (uint16_t j = 0; j < (isHor ? 18 : 24); j++) {
    drawHex(0, 3 + j, 1, address + j * 8, 4, COLOR_BLUE);
    printText(4, 3 + j, 1, "|", COLOR_ORANGE);
    for (uint16_t i = 0; i < 8; i++) {
      uint8_t b = I2CEEPROM.read(device, address + j * 8 + i);
      drawHex(5 + i * 3, 3 + j, 1, b, 2, COLOR_OLIVE);
      ch[0] = b == 0 ? ' ' : b;
      printText(startText + i * deltaText, 3 + j, 1, ch, COLOR_RED);
    }
  }
}

/**
 * Редактирование
 */
void ScreenDump::edit(char key) {
  switch(key) {
  case 1: // начальная
    nField = 1;
    nPosit = 0;
    fields[0].val = device;
    fields[1].val = address;
    break;
  case 'M': // записываем
    device = fields[0].val;
    address = fields[1].val;
    break;
  }
#ifdef HAS_SERIAL
  Serial.print("Dump key = ");
  Serial.println(key,HEX);
#endif
  ScreenTft::edit(key);
}

/**
 * Перемещение по памяти
 */
void ScreenDump::control(char key) {
  switch(key) {
  case '<': // страница вверх
    if (address > 0x80) address -= 0x80; else address = 0;
    break;
  case '>': // страница вниз
    if (address < 0x1f00) address += 0x80; else address = 0x1f80;
    break;
  case '-': // строка вверх
    if (address > 0x8) address -= 0x8; else address = 0;
    break;
  case '+': // строка вниз
    if (address < 0x1f78) address += 0x8; else address = 0x1f80;
    break;
  default:
    return;
  }
  draw();
}
