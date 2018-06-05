#ifndef I2C_EEPROM_h
#define I2C_EEPROM_h

#include <inttypes.h>

class SerialBlock {
public:
  SerialBlock() {
    body = NULL;
  }
  ~SerialBlock();
  /* ������ ����� */
  int8_t size;
  /* ����� ��������� (0x57 ��� CMOS) */
  int8_t device;
  /* ����� ����� � EEPROM ������ */
  int16_t address;
  /* ����������� ����� ����� */
  int16_t cs;
  /* ���� ����� */
  byte * body;
};

class I2C_EEPROM {
public:
  uint8_t read(uint8_t device, uint16_t address);
  void write(uint8_t device, uint16_t address, uint8_t data);
  void write_buffer(uint8_t device, uint16_t address, uint8_t* data, uint8_t length);
  void read_buffer(uint8_t device, uint16_t address, uint8_t* data, uint8_t length);
  /** ������ ���� int (2 �����) �� Serial */
  int16_t serialReadInteger();
  /**
   * ������ ���� ���� ������ ��� ������� � EEPROM �� Serial
   */
  SerialBlock * serialReadBlock();
private:
  void beginTransmission(uint8_t device, uint16_t address);
};

static I2C_EEPROM I2CEEPROM;

#endif // I2C_EEPROM

