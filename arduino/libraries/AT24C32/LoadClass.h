#ifndef LOAD_CLASS_H
#define LOAD_CLASS_H

#include <I2C_EEPROM.h>

class LoadClass {
private:
    /* ����� ��������� (0x57 ��� CMOS) */
    int8_t device;
    /* ����� ����� � EEPROM ������ */
    int16_t address;
public:
    LoadClass(int8_t device, int16_t address);
    ~LoadClass();
    char readChar();
    char readChar(int16_t address);
    uint8_t readByte();
    uint8_t readByte(int16_t address);
    int readInt();
    int readInt(int16_t address);
    long readLong();
    long readLong(int16_t address);
    float readFloat();
    float readFloat(int16_t address);
    char * readString();
    char * readString(int16_t address);
    void deleteString(char * str);
};

#endif // LOAD_CLASS_H
