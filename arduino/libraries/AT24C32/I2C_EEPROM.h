#ifndef I2C_EEPROM_h
#define I2C_EEPROM_h

#include <inttypes.h>

class SerialBlock {
public:
  SerialBlock() {
    body = NULL;
  }
  ~SerialBlock();
  /* Размер блока */
  int8_t size;
  /* номер микосхемы (0x57 для CMOS) */
  int8_t device;
  /* адрес блока в EEPROM памяти */
  int16_t address;
  /* Контрольная сумма блока */
  int16_t cs;
  /* тело блока */
  byte * body;
};

class I2C_EEPROM {
public:
  uint8_t read(uint8_t device, uint16_t address);
  void write(uint8_t device, uint16_t address, uint8_t data);
  void write_buffer(uint8_t device, uint16_t address, uint8_t* data, uint8_t length);
  void read_buffer(uint8_t device, uint16_t address, uint8_t* data, uint8_t length);
  /** Читаем один int (2 байта) из Serial */
  int16_t serialReadInteger();
  /**
   * Читаем один блок данных для закачки в EEPROM из Serial
   */
  SerialBlock * serialReadBlock();
private:
  void beginTransmission(uint8_t device, uint16_t address);
};

static I2C_EEPROM I2CEEPROM;

#endif // I2C_EEPROM

