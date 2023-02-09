#include <Arduino.h> // todo remove
#include <string.h>

#include <LoadClass.h>
#include <SerialBlock.h>

#define HAS_SERIAL_DEBUG

LoadClass::LoadClass(int8_t device, int16_t address) {
  this->device = device;
  this->address = address;
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
  I2CEEPROM.read_buffer(device, address, (uint8_t*) &i, 2);
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
  I2CEEPROM.read_buffer(device, address, (uint8_t*) &l, 4);
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
  I2CEEPROM.read_buffer(device, address, (uint8_t*) &f, 4);
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
  int len = readInt() - 2;
  char * str = new char[len + 1];
  I2CEEPROM.read_buffer(device, address, (uint8_t*) str, len);
  str[len] = 0;
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

/**
 * Получаем длинну объекта для выделения памяти под него
 * @brief getLength
 * @param charDef
 * @return
 */
int LoadClass::getRomLength(char charDef) {
  switch (charDef) {
  case 'c': return 1;
  case 'b': return 1;
  case 'i': return 2;
  case 'l': return 4;
  case 'f': return 4;
  case 's': return 2;
  default: return 0;
  }
}

/**
 * Получаем длинну объекта для выделения памяти под него
 * @brief LoadClass::getObjectLength
 * @param definition
 * @return
 */
int LoadClass::getObjectLength(char * definition){
#ifdef HAS_SERIAL_DEBUG
  Serial.print(definition);
  Serial.print(" - ");
  Serial.println(strlen(definition));
#endif
  int len = 0;
  for (int i = 0; i < strlen(definition); i++) {
    len += getRomLength(definition[i]);
  }
  return len;
}
/**
 * Читаем объект
 * OBJECT - '{s}'
 * @brief LoadClass::readString
 * @return
 */
void * LoadClass::readObject(int &pos, char * &definition){
  int len = readInt();
  definition = readString();
  int objectLength = getObjectLength(definition);
  uint8_t * obj = new uint8_t[objectLength];
  pos = 0;
  for (int i = 0; i < strlen(definition); i++) {
    switch (definition[i]) {
    case 'c':
      obj[pos] = readChar();
      pos++;
      break;
    case 'b':
      obj[pos] = readByte();
      pos++;
      break;
    case 'i': {
      int intVal = readInt();
      memcpy(obj + pos, (const void*) &intVal, 2);
      pos += 2;
    } break;
    case 'l': {
      long longVal = readLong();
      memcpy(obj + pos, (const uint8_t *) &longVal, 4);
      pos += 4;
    } break;
    case 'f': {
      float floatVal = readFloat();
      memcpy(obj + pos, (const void*) &floatVal, 4);
      pos += 4;
    } break;
    case 's': {
      char * stringVal = readString();
      memcpy(obj + pos, (const void*) &stringVal, 2);
      pos += 2;
    } break;
    default: break;
    }
  }
  return obj;
}
void * LoadClass::readObject(int16_t _address, int & pos, char * &definition){
  address = _address;
  return readObject(pos, definition);
}
void LoadClass::deleteObject(char * definition, void * obj){
  int len = 0;
  for (int i = 0; i < strlen(definition); i++) {
    switch (definition[i]) {
    case 'c': len++; break;
    case 'b': len++; break;
    case 'i': len += 2; break;
    case 'l': len += 4; break;
    case 'f': len += 4; break;
    case 's': {
      delete (char *)(*((uint16_t *)(obj + len)));
      len += 2;
    } break;
    default: break;
    }
  }
  delete definition;
  delete obj;
}

void * LoadClass::readArray(int & count) {
  int len = readInt();
  char charDef = readChar();
  int romLength = getRomLength(charDef);
  count = readInt();
  int objectLength = romLength * count;
#ifdef HAS_SERIAL_DEBUG
  Serial.print("readArray len = ");
  Serial.print(len);
  Serial.print("; charDef = ");
  Serial.print(charDef);
  Serial.print("; objectLength = ");
  Serial.print(objectLength);
  Serial.print("; address = ");
  Serial.println(address, HEX);
#endif
  void * arr = new uint8_t[objectLength];
  I2CEEPROM.read_buffer(device, address, (uint8_t*) arr, objectLength);
  return arr;
}
void * LoadClass::readArray(int16_t _address, int & count) {
  address = _address;
  return readArray(count);
}
void LoadClass::deleteArray(void * arr) {
  delete arr;
}
