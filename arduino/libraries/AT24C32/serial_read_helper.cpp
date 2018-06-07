#include <Arduino.h>
#include "I2C_EEPROM.h"


int8_t charToHex(char ch) {
  return ((ch < 'A') ? (ch - '0') : (ch - 'A' + 0xA)) & 0x0F;
}

char byteToHexChar(int8_t bt) {
  return ((bt < 0xA) ? (bt + '0') : (bt + 'A' - 0xA));
}

void SerialPrintHex(int8_t bt) {
  char out[3];
  out[0] = byteToHexChar((bt >> 4) & 0xf);
  out[1] = byteToHexChar(bt & 0xf);
  out[2] = 0;
  Serial.print(out);
}

/**
 * Читаем один int (2 байта) из Serial
 */
int16_t serialReadInteger() {
  byte bt[2]; if (Serial.readBytes(bt, 2) == 2) {
    return makeWord(bt[0], bt[1]);
  }
  return -1;
}

/**
 * Читаем один блок данных для закачки в EEPROM из Serial
 * Блок состоит из:
 * 0 - int16_t = "EB" Признак, что это EEPROM Block
 * 5 - int8_t  - (size) Размер блока
 * 2 - int8_t  - (device) номер микосхемы (0x57 для CMOS)
 * 3 - int16_t - (address) блока в EEPROM памяти
 * 7 - int16_t - Контрольная сумма блока
 * 9 - byte[size] - сам блок
 */
SerialBlock * serialReadBlock() {
  SerialBlock * sb = new SerialBlock();
  int len = Serial.readBytes((byte *) sb, 6);
  if (len == 6) {
    if (sb->size > 0 && sb->size < 22) {
      sb->body = new byte[sb->size];
      len = Serial.readBytes((char *) sb->body, sb->size);
      if (len == sb->size) {
        return sb;
      }
    }
  }
  delete sb;
  return NULL;
}

SerialBlock::~SerialBlock() {
#ifdef HAS_SERIAL
  Serial.print("delete ");
  Serial.println((int) this, HEX);
#endif
  if (NULL != body) {
    delete body;
    body = NULL;
  }
}
