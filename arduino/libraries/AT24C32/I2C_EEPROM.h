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
  /* ����� ��������� (0x57 ��� CMOS) */
  int8_t device;
  /* ����� ����� � EEPROM ������ */
  int16_t address;
  /* ������ ����� */
  int16_t size;
  /* ����������� ����� ����� */
  int16_t cs;
  /* ���� ����� */
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
/** �������� ����� 2 ������� ��� ����� � HEX  */
extern void SerialPrintHex(int8_t bt);
/** ������ ���� ���� �� Serial */
extern int8_t serialReadByte();
/** ������ ���� short (2 �����) �� Serial */
extern int16_t serialReadShort();
/** ������ ���� ���� ������ � ����� ��� � EEPROM */
extern void serialWriteBlock();
/** ������ ���� ���� ������ �� EEPROM � �������� ��� � Serial */
extern void serialReadBlock();

static I2C_EEPROM __attribute__ ((unused)) I2CEEPROM;

#endif // I2C_EEPROM

