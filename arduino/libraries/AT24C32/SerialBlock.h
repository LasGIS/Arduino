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

#endif // SERIAL_BLOCK_H
