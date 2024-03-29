#ifndef LOAD_CLASS_REF_H
#define LOAD_CLASS_REF_H

#include <I2C_EEPROM.h>
#include <LoadClassCommon.h>

class LoadClassRef {
private:
  /* ����� ��������� (0x57 ��� CMOS) */
  int8_t device;
  /* ����� ����� � EEPROM ������ */
  AddressEeprom address;
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
  AddressEeprom toNext(CharDefinition cdef, int inc);

public:
  LoadClassRef(int8_t device, AddressEeprom address);
  ~LoadClassRef();
  inline void toAddress(AddressEeprom _address) { address = _address; }

  //=== �������� ������ ===
  uint8_t readByte();
  int readInt();
  long readLong();
  float readFloat();
  inline char * readString(){ return readString(true); }
  inline char * newString() { return readString(false); }

  AddressEeprom toObjectItem(uint16_t item);
  inline AddressEeprom toObjectItem(AddressEeprom _address, int item) { address = _address; return toObjectItem(item); }
  void * readObject(int & length);

  AddressEeprom toArrayItem(int item);
  inline AddressEeprom toArrayItem(AddressEeprom _address, int item) { address = _address; return toArrayItem(item); }
  void * readArray(int & count);
};

#endif // LOAD_CLASS_REF_H
