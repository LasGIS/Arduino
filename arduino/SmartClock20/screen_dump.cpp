#include "SmartClock20.h"

ScreenDump::ScreenDump(): ScreenTft() {
  name = (char*) "Dump  ";
  maxFields = 1;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {2, 1, 2, 3, 0, 256, 0, NULL}; // device
  fields[1] = {2, 5, 2, 5, 0, 0x1fff, 0, NULL}; // address
}

void ScreenDump::showOnce() {
  ScreenTft::showOnce();
  draw();
}
/**
 * Рисуем страницу
 */
void ScreenDump::draw() {
  for (uint16_t j = 0; j < 16; j++) {
    drawHex(0, 3 + j, 1, address + j * 8, 4, COLOR_BLUE);
    for (uint16_t i = 0; i < 8; i++) {
      uint8_t b = I2CEEPROM.read(device, address + j * 8 + i);
      drawHex(5 + i * 3, 3 + j, 1, b, 2, COLOR_OLIVE);
    }
  }
}

/**
 * Редактирование
 */
void ScreenDump::edit(char key) {
  switch(key) {
  case 1: // начальная
    nField = 0;
    nPosit = 0;
    fields[0].val = device;
    fields[1].val = address;
    showAllFields();
    break;
  case 'M': // записываем
    device = fields[0].val;
    address = fields[1].val;
  case 'r': // без записи
//    draw();
    break;
  }
  Serial.print("Dump key = ");
  Serial.println(key,HEX);
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
  }
  draw();
}
