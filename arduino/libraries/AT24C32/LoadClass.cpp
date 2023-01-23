//#include <Arduino.h> // todo remove
#include <string.h>

#include <LoadClass.h>
#include <SerialBlock.h>

//#define HAS_SERIAL_DEBUG

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
  int len = readInt() - 2;
  char * str = new char[len + 1];
  I2CEEPROM.read_buffer(device, address, (uint8_t*) str, (uint8_t) len);
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
 * Читаем объект
 * OBJECT - '{s}'
 * @brief LoadClass::readString
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
    switch (definition[i]) {
    case '}': return len;
    case 'c': len++; break;
    case 'b': len++; break;
    case 'i': len += 2; break;
    case 'l': len += 4; break;
    case 'f': len += 4; break;
    case 's': len += 2; break;
    case '{':
    default:
      break;
    }
  }
  return len;
}
uint8_t * LoadClass::readObject(int & pos){
  int len = readInt();
  char * definition = readString();
  int objectLength = getObjectLength(definition);
#ifdef HAS_SERIAL_DEBUG
  Serial.print("readObject len = ");
  Serial.print(len);
  Serial.print("; definition = ");
  Serial.print(definition);
  Serial.print("; objectLength = ");
  Serial.print(objectLength);
  Serial.print("; address = ");
  Serial.print(address, HEX);
#endif
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
      SerialPrintHex((uint8_t *) &stringVal, 2);
      memcpy(obj + pos, (const void*) &stringVal, 2);
      pos += 2;
    } break;
    default:
      break;
    }
  }
  delete definition;
  return obj;
}
uint8_t * LoadClass::readObject(int16_t _address, int & pos){
  address = _address;
  return readObject(pos);
}
void LoadClass::deleteObject(char * definition, uint8_t * obj){
  delete obj;
}
