#include <LoadClass.h>

LoadClass::LoadClass(int8_t device, int16_t address) {
    this->device = device;
    this->address = address;
}

LoadClass::~LoadClass(){

}

/**
 * Читаем знак
 * CHAR   - 'c'
 * @brief LoadClass::readChar
 * @return
 */
char LoadClass::readChar(){
    char c = I2CEEPROM.read(device, address);
    address++;
    return c;
}
char LoadClass::readChar(int16_t _address){
    address = _address;
    return readChar();
}

/**
 * Читаем байт
 * INT8   - 'b'
 * @brief LoadClass::readByte
 * @return
 */
uint8_t LoadClass::readByte(){
    uint8_t b = I2CEEPROM.read(device, address);
    address++;
    return b;
}
uint8_t LoadClass::readByte(int16_t _address){
    address = _address;
    return readByte();
}

/**
 * Читаем целое
 * INT16  - 'i'
 * @brief LoadClass::readInt
 * @return
 */
int LoadClass::readInt(){
    int i;
    I2CEEPROM.read_buffer(device, address, (uint8_t*) &i, (uint8_t) 2);
    address+=2;
    return i;
}
int LoadClass::readInt(int16_t _address){
    address = _address;
    return readInt();
}

/**
 * Читаем длинное целое
 * INT32  - 'l'
 * @brief LoadClass::readLong
 * @return
 */
long LoadClass::readLong(){
    long l;
    I2CEEPROM.read_buffer(device, address, (uint8_t*) &l, (uint8_t) 4);
    address+=4;
    return l;
}
long LoadClass::readLong(int16_t _address){
    address = _address;
    return readLong();
}

/**
 * Читаем число с плавающей точкой
 * FLOAT  - 'f'
 * @brief LoadClass::readFloat
 * @return
 */
float LoadClass::readFloat(){
    float f;
    I2CEEPROM.read_buffer(device, address, (uint8_t*) &f, (uint8_t) 4);
    address += 4;
    return f;
}
float LoadClass::readFloat(int16_t _address){
    address = _address;
    return readFloat();
}

/**
 * Читаем строку
 * STRING - 's'
 * @brief LoadClass::readString
 * @return
 */
char * LoadClass::readString(){
    int len = readInt();
    char * str = new char[len];
    I2CEEPROM.read_buffer(device, address, (uint8_t*) str, (uint8_t) len - 2);
    str[len - 2] = 0;
    address += len;
    return str;
}
char * LoadClass::readString(int16_t _address){
    address = _address;
    return readString();
}
void LoadClass::deleteString(char * str){
    delete str;
}
