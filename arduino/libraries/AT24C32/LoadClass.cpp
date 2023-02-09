#include <Arduino.h> // todo remove
#include <string.h>

#include <LoadClass.h>
#include <SerialBlock.h>

// #define HAS_SERIAL_DEBUG

LoadClass::LoadClass(int8_t device, int16_t address) {
  this->device = device;
  this->address = address;
  refs = calloc(10, sizeof(void*));
  refMaxLength = 10;
  refLength = 0;
}
LoadClass::~LoadClass() {
#ifdef HAS_SERIAL_DEBUG
  Serial.print("LoadClass::~LoadClass(");
#endif
  for (int i = 0; i < refLength; i++) {
    void * ref = refs[i];
#ifdef HAS_SERIAL_DEBUG
    Serial.print((int) ref, HEX);
    Serial.print(",");
#endif
    delete ref;
  }
#ifdef HAS_SERIAL_DEBUG
  Serial.print((int) refs, HEX);
  Serial.println(")");
#endif
  delete refs;
}
void * LoadClass::addRef(void * ref) {
  if (refLength + 1 >= refMaxLength) {
    refMaxLength += 10;
    refs = realloc(refs, refMaxLength * sizeof(void*));
  }
  refs[refLength++] = ref;
#ifdef HAS_SERIAL_DEBUG
  Serial.print("addRef(");
  for (int i = 0; i < refLength; i++) {
    Serial.print((int) refs[i], HEX);
    Serial.print(",");
  }
  Serial.println(")");
#endif
  return ref;
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
  char * str = addRef(new char[len + 1]);
  I2CEEPROM.read_buffer(device, address, (uint8_t*) str, len);
  str[len] = 0;
  address += len;
  return str;
}
char * LoadClass::readString(int16_t _address){
  address = _address;
  return readString();
}

/**
 * Получаем длинну объекта для выделения памяти под него
 * @brief getLength
 * @param charDef
 * @return
 */
int LoadClass::getRomLength(char charDef) {
  switch (charDef) {
  case 'c':
  case 'b': return 1;
  case 'i': return 2;
  case 'l':
  case 'f': return 4;
  case 's':
  case 'a':
  case 'o': return 2;
  default: return 0;
  }
}

/**
 * Читаем знвчение из EEPROM памяти по типу (charDef)
 * @brief LoadClass::readRom
 * @param obj
 * @param pos
 * @param charDef
 */
void LoadClass::readRom(uint8_t * obj, int &pos, char charDef) {
  switch (charDef) {
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
    memcpy(obj + pos, (const void*) &longVal, 4);
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
  case 'a': {
    int count;
    void * arrayVal = readArray(count);
    memcpy(obj + pos, (const void*) &arrayVal, 2);
    pos += 2;
  } break;
  case 'o': {
    int count;
    void * objectVal = readObject(count);
    memcpy(obj + pos, (const void*) &objectVal, 2);
    pos += 2;
  } break;
  default: break;
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
void * LoadClass::readObject(int &pos){
  int len = readInt();
  char * definition = readString();
  int objectLength = getObjectLength(definition);
  uint8_t * obj = addRef(new uint8_t[objectLength]);
  pos = 0;
  for (int i = 0; i < strlen(definition); i++) {
    readRom(obj, pos, definition[i]);
  }
  return obj;
}
void * LoadClass::readObject(int16_t _address, int & pos){
  address = _address;
  return readObject(pos);
}

void * LoadClass::readArray(int & count) {
  int len = readInt();
  char charDef = readChar();
  int romLength = getRomLength(charDef);
  count = readInt();
  int arrayLength = romLength * count;
#ifdef HAS_SERIAL_DEBUG
  Serial.print("readArray len = ");
  Serial.print(len);
  Serial.print("; charDef = ");
  Serial.print(charDef);
  Serial.print("; arrayLength = ");
  Serial.print(arrayLength);
  Serial.print("; address = ");
  Serial.println(address, HEX);
#endif
  void * arr = addRef(new uint8_t[arrayLength]);
  switch (charDef) {
  case 'c':
  case 'b':
  case 'i':
  case 'l':
  case 'f': {
    I2CEEPROM.read_buffer(device, address, (uint8_t*) arr, arrayLength);
  } break;
  case 's':
  case 'a':
  case 'o': {
    int pos = 0;
    for (int i = 0; i < count; i++) {
      readRom(arr, pos, charDef);
    }
  } break;
  default: break;
  }
  return arr;
}
void * LoadClass::readArray(int16_t _address, int & count) {
  address = _address;
  return readArray(count);
}
