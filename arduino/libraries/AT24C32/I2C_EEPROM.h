#ifndef I2C_EEPROM_h
#define I2C_EEPROM_h

#include <inttypes.h>
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

class I2C_EEPROM {
public:
  uint8_t read(uint8_t device, uint16_t address);
  void write(uint8_t device, uint16_t address, uint8_t data);
  void write_buffer(uint8_t device, uint16_t address, uint8_t* data, uint16_t length);
  void read_buffer(uint8_t device, uint16_t address, uint8_t* data, uint16_t length);
private:
  void beginTransmission(uint8_t device, uint16_t address);
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

static I2C_EEPROM __attribute__ ((unused)) I2CEEPROM;

#endif // I2C_EEPROM

