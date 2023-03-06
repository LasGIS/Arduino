#ifndef LOAD_CLASS_H
#define LOAD_CLASS_H

#include <I2C_EEPROM.h>

enum CharDefinition: uint8_t {
  charDef = 'c',
  byteDef = 'b',
  intDef = 'i',
  longDef = 'l',
  floatDef = 'f',
  stringDef = 's',
  arrayDef = 'a',
  objectDef = 'o'
};

class LoadClass {
private:
  /* ����� ��������� (0x57 ��� CMOS) */
  int8_t device;
  /* ����� ����� � EEPROM ������ */
  int16_t address;
  /* ������ ���������� ������ ������ ��� �������� */
  void **refs;
  int refMaxLength = 10;
  int refLength = 0;
private:
  int getRomLength(CharDefinition cdef);
  int getObjectLength(char * definition);
  void * addRef(void * ref);
  void readRom(uint8_t * obj, int &pos, CharDefinition cdef);
  char * readString(bool isNew);
  int16_t toNext(CharDefinition cdef, int inc);

public:
  LoadClass(int8_t device, int16_t address);
  ~LoadClass();
  inline  void toAddress(int16_t _address) { address = _address; }

  //=== �������� ������ ===
  uint8_t readByte();
  int readInt();
  long readLong();
  float readFloat();
  inline char * readString(){ return readString(true); }
  inline char * newString() { return readString(false); }

  int16_t toObjectItem(uint16_t item);
  inline int16_t toObjectItem(int16_t _address, int item) { address = _address; return toObjectItem(item); }
  void * readObject(int & length);

  int16_t toArrayItem(int item);
  inline int16_t toArrayItem(int16_t _address, int item) { address = _address; return toArrayItem(item); }
  void * readArray(int & count);
};

#endif // LOAD_CLASS_H
