#include <Arduino.h> // todo remove
#include <string.h>

#include <LoadClassRef.h>
#include <SerialBlock.h>

// #define HAS_SERIAL_DEBUG

LoadClassRef::LoadClassRef(int8_t device, AddressEeprom address) {
  this->device = device;
  this->address = address;
  refs = (void**) calloc(10, sizeof(void*));
  refMaxLength = 10;
  refLength = 0;
}

LoadClassRef::~LoadClassRef() {
#ifdef HAS_SERIAL_DEBUG
  Serial.print("LoadClassRef::~LoadClassRef(");
#endif
  for (int i = 0; i < refLength; i++) {
    void * ref = refs[i];
#ifdef HAS_SERIAL_DEBUG
    Serial.print((int) ref, HEX);
    Serial.print(",");
#endif
    delete (uint8_t *) ref;
  }
#ifdef HAS_SERIAL_DEBUG
  Serial.print((int) refs, HEX);
  Serial.println(")");
#endif
  delete refs;
}

void * LoadClassRef::addRef(void * ref) {
  if (refLength + 1 >= refMaxLength) {
    refMaxLength += 10;
    refs = (void**) realloc(refs, refMaxLength * sizeof(void*));
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
 * Читаем байт
 * INT8   - 'b'
 * @brief LoadClassRef::readByte
 * @return
 */
uint8_t LoadClassRef::readByte(){
  uint8_t b = I2CEEPROM.read(device, address);
  address++;
  return b;
}

/**
 * Читаем целое
 * INT16  - 'i'
 * @brief LoadClassRef::readInt
 * @return
 */
int LoadClassRef::readInt(){
  int i;
  I2CEEPROM.read_buffer(device, address, (uint8_t*) &i, 2);
  address+=2;
  return i;
}

/**
 * Читаем длинное целое
 * INT32  - 'l'
 * @brief LoadClassRef::readLong
 * @return
 */
long LoadClassRef::readLong(){
  long l;
  I2CEEPROM.read_buffer(device, address, (uint8_t*) &l, 4);
  address+=4;
  return l;
}

/**
 * Читаем число с плавающей точкой
 * FLOAT  - 'f'
 * @brief LoadClassRef::readFloat
 * @return
 */
float LoadClassRef::readFloat(){
  float f;
  I2CEEPROM.read_buffer(device, address, (uint8_t*) &f, 4);
  address += 4;
  return f;
}

/**
 * Читаем строку
 * STRING - 's'
 * @brief LoadClassRef::readString
 * @param isLazyDelete если true, то применяется ленивое удаление
 *  (т.е. не надо удалять принудительно)
 * @return
 */
char * LoadClassRef::readString(bool isLazyDelete){
  int len = readInt();
  char * str = new char[len + 1];
  if (isLazyDelete) addRef(str);
  I2CEEPROM.read_buffer(device, address, (uint8_t*) str, len);
  str[len] = 0;
  address += len;
  return str;
}

AddressEeprom LoadClassRef::toNext(CharDefinition cdef, int inc) {
  int size = getDefinitionSize(cdef);
  if (size == -1) {
    for (int i = 0; i < inc; i++) {
      address += readInt();
    }
  } else {
    address += size * inc;
  }
  return address;
}

/**
 * Получаем длинну объекта для выделения памяти под него
 * @brief getLength
 * @param charDef
 * @return
 */
int LoadClassRef::getRomLength(CharDefinition cdef) {
  int size = getDefinitionSize(cdef);
  return size == -1 ? (cdef == arrayDef ? 4 : 2) : size;
}

/**
 * Читаем знвчение из EEPROM памяти по типу (charDef)
 * @brief LoadClassRef::readRom
 * @param obj
 * @param pos
 * @param charDef
 */
void LoadClassRef::readRom(uint8_t * obj, int &pos, CharDefinition cdef) {
  switch (cdef) {
  case charDef:
  case byteDef:
    obj[pos] = readByte();
    pos++;
    break;
  case referDef:
  case intDef: {
    int intVal = readInt();
    memcpy(obj + pos, (const void*) &intVal, 2);
    pos += 2;
  } break;
  case longDef: {
    long longVal = readLong();
    memcpy(obj + pos, (const void*) &longVal, 4);
    pos += 4;
  } break;
  case floatDef: {
    float floatVal = readFloat();
    memcpy(obj + pos, (const void*) &floatVal, 4);
    pos += 4;
  } break;
  case stringDef: {
    char * stringVal = readString();
    memcpy(obj + pos, (const void*) &stringVal, 2);
    pos += 2;
  } break;
  case arrayDef: {
    int count;
    void * arrayVal = readArray(count);
    memcpy(obj + pos, (const void*) &count, 2);
    pos += 2;
    memcpy(obj + pos, (const void*) &arrayVal, 2);
    pos += 2;
  } break;
  case objectDef: {
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
 * @brief LoadClassRef::getObjectLength
 * @param definition
 * @return
 */
int LoadClassRef::getObjectLength(char * definition){
#ifdef HAS_SERIAL_DEBUG
  Serial.print("getObjectLength definition(");
  Serial.print(strlen(definition));
  Serial.print(") = \"");
  Serial.print(definition);
  Serial.print("\"");
#endif
  int len = 0;
  for (uint16_t i = 0; i < strlen(definition); i++) {
    len += getRomLength((CharDefinition) definition[i]);
  }
#ifdef HAS_SERIAL_DEBUG
  Serial.print("; len = ");
  Serial.println(len);
#endif
  return len;
}

/**
 * Переводим внутренний адрес на адрес элемента
 * Действительно только для объектов
 * @brief LoadClassRef::toObjectItem
 * @param item номер элемента
 * @return адрес элемента в EEPROM
 */
AddressEeprom LoadClassRef::toObjectItem(uint16_t item) {
  readInt(); // пропускаем размер своего блока
  char * definition = newString();
  for (uint16_t i = 0; i + 1 < strlen(definition) && i < item; i++) {
    toNext((CharDefinition) definition[i], 1);
  }
  delete definition;
  return address;
}

/**
 * Читаем объект
 * OBJECT - 'o'
 * @brief LoadClassRef::readObject
 * @return новый объект
 */
void * LoadClassRef::readObject(int &length) {
  readInt(); // пропускаем размер своего блока
  char* definition = newString();
  int objectLength = getObjectLength(definition);
  uint8_t* obj = (uint8_t*) addRef(new uint8_t[objectLength]);
  length = 0;
  for (uint16_t i = 0; i < strlen(definition); i++) {
    readRom(obj, length, (CharDefinition) definition[i]);
  }
  delete definition;
  return obj;
}

/**
 * Переводим внутренний адрес на адрес элемента
 * Действительно только для массивов
 * @brief LoadClassRef::toArrayItem
 * @param item номер элемента
 */
AddressEeprom LoadClassRef::toArrayItem(int item) {
  readInt(); // пропускаем размер своего блока
  CharDefinition charDef = (CharDefinition) readByte();
  int count = readInt() - 1;
  return toNext(charDef, item < count ? item : count);
}

/**
 * Читаем массив
 * ARRAY - 'a'
 * @brief LoadClassRef::readArray
 * @param count число членов массива
 * @return
 */
void * LoadClassRef::readArray(int & count) {
#ifdef HAS_SERIAL_DEBUG
  int len = readInt(); // пропускаем размер своего блока
  Serial.print("readArray len = ");
  Serial.print(len);
#else
  readInt(); // пропускаем размер своего блока
#endif
  CharDefinition cdef = (CharDefinition) readByte();
  count = readInt();
  int size = getDefinitionSize(cdef);
#ifdef HAS_SERIAL_DEBUG
  Serial.print("; charDefinition = ");
  Serial.print((char) cdef);
  Serial.print("; definitionSize = ");
  Serial.print(size);
  Serial.print("; address = ");
  Serial.println(address, HEX);
#endif
  if (size == -1) {
    /** объекты */
    size = (cdef == arrayDef) ? 4 : 2;
#ifdef HAS_SERIAL_DEBUG
    Serial.print("new uint8_t[");
    Serial.print(count * size);
    Serial.print("]");
#endif
    void * arr = addRef(new uint8_t[count * size]);
    int pos = 0;
    for (int i = 0; i < count; i++) {
#ifdef HAS_SERIAL_DEBUG
      Serial.print(i);
      Serial.print(" |-> address = ");
      Serial.println(address, HEX);
#endif
      readRom((uint8_t*) arr, pos, cdef);
    }
    return arr;
  } else if (size > 0) {
    /** простые типы */
    int arrayLength = count * size;
    void * arr = addRef(new uint8_t[arrayLength]);
    I2CEEPROM.read_buffer(device, address, (uint8_t*) arr, arrayLength);
    address += arrayLength;
    return arr;
  }
  return NULL;
}
