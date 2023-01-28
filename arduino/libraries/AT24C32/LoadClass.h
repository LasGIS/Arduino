#ifndef LOAD_CLASS_H
#define LOAD_CLASS_H

#include <I2C_EEPROM.h>

class LoadClass {
private:
    /* номер микосхемы (0x57 для CMOS) */
    int8_t device;
    /* адрес блока в EEPROM памяти */
    int16_t address;
private:
    int getObjectLength(char * definition);
public:
    LoadClass(int8_t device, int16_t address);
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

    uint8_t * readObject(int & pos, char * &definition);
    uint8_t * readObject(int16_t address, int &pos, char * &definition);
    void deleteObject(char * definition, uint8_t * obj);
 };

#endif // LOAD_CLASS_H
