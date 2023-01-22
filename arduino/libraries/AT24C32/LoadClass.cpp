#include <Arduino.h> // todo remove
#include <LoadClass.h>
#include <SerialBlock.h>

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

/**
 * Читаем объект
 * OBJECT - '{s}'
 * @brief LoadClass::readString
 * @return
 */
int LoadClass::getObjectLench(char * definition){
  Serial.print(definition);
  Serial.print(" - ");
  Serial.println(strlen(definition));
  int len = 0;
  for (int i = 0; i < strlen(definition); i++) {
    switch (definition[i]) {
    case '}':
      return len;
    case 'c':
      Serial.print('c');
      len++;
      break;
    case 'b':
      Serial.print('b');
      len++;
      break;
    case 'i':
      Serial.print('i');
      len += 2;
      break;
    case 'l':
      Serial.print('l');
      len += 4;
      break;
    case 'f':
      Serial.print('f');
      len += 4;
      break;
    case 's':
      Serial.print('s');
      len += 2;
      break;
    case '{':
    default:
      break;
    }
  }
  return len;
}
uint8_t * LoadClass::readObject(char * definition, int & pos){
  int len = readInt();
  int count = readByte();
  int objectLench = getObjectLench(definition);
#ifdef HAS_SERIAL_DEBUG
  Serial.print("readObject len = ");
  Serial.print(len);
  Serial.print("; count = ");
  Serial.print(count);
  Serial.print("; objectLench = ");
  Serial.print(objectLench);
#endif
  uint8_t * obj = new uint8_t[objectLench];
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
  return obj;
}
uint8_t * LoadClass::readObject(char * definition, int16_t _address, int & pos){
  address = _address;
  return readObject(definition, pos);
}
void LoadClass::deleteObject(char * definition, uint8_t * str){
  delete str;
}
