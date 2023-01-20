#ifndef SERIAL_BLOCK_H
#define SERIAL_BLOCK_H

#include <I2C_EEPROM.h>
#include <stdio.h> // for size_t

class SerialBlock {
public:
  SerialBlock() {
    body = NULL;
  }
  ~SerialBlock();
  /* номер микосхемы (0x57 для CMOS) */
  int8_t device;
  /* адрес блока в EEPROM памяти */
  int16_t address;
  /* Размер блока */
  int16_t size;
  /* Контрольная сумма блока */
  int16_t cs;
  /* тело блока */
  int8_t * body;
};

/** Печатаем ровно 2 символа для байта в HEX  */
extern void SerialPrintHex(int8_t bt);
/** Читаем один байт из Serial */
extern int8_t serialReadByte();
/** Читаем один short (2 байта) из Serial */
extern int16_t serialReadShort();
/** Читаем один блок данных и пишем его в EEPROM */
extern void serialWriteBlock();
/** Читаем один блок данных из EEPROM и посылаем его в Serial */
extern void serialReadBlock();

#endif // SERIAL_BLOCK_H
