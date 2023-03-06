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
  uint16_t size;
  /* ����������� ����� ����� todo: remove */
  int16_t cs;
  /* ���� ����� */
  uint8_t * body;
};

/** �������� ����� 2 ������� ��� ����� � HEX  */
extern void SerialPrintHex(uint8_t bt);
/** �������� ����� ��� ������� � ������� ��� ����� � HEX  */
extern void SerialPrintHex(uint8_t * obj, int len);
/** ������ ���� ���� �� Serial */
extern uint8_t serialReadByte();
/** ������ ���� short (2 �����) �� Serial */
extern uint16_t serialReadShort();
/** ������ ���� ���� ������ � ����� ��� � EEPROM */
extern void serialWriteBlock();
/** ������ ���� ���� ������ �� EEPROM � �������� ��� � Serial */
extern void serialReadBlock();

#endif // SERIAL_BLOCK_H
