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
  uint16_t size;
  /* Контрольная сумма блока todo: remove */
  int16_t cs;
  /* тело блока */
  uint8_t * body;
};

/** Печатаем ровно 2 символа для байта в HEX  */
extern void SerialPrintHex(uint8_t bt);
/** Печатаем ровно все символы в объекте как байты в HEX  */
extern void SerialPrintHex(uint8_t * obj, int len);
/** Читаем один байт из Serial */
extern uint8_t serialReadByte();
/** Читаем один short (2 байта) из Serial */
extern uint16_t serialReadShort();
/** Читаем один блок данных и пишем его в EEPROM */
extern void serialWriteBlock();
/** Читаем один блок данных из EEPROM и посылаем его в Serial */
extern void serialReadBlock();

#endif // SERIAL_BLOCK_H
