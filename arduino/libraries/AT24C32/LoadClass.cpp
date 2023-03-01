#include <Arduino.h> // todo remove
#include <string.h>

#include <LoadClass.h>
#include <SerialBlock.h>

// #define HAS_SERIAL_DEBUG

int getDefinitionSize(CharDefinition cdef) {
  switch (cdef) {
  case charDef:
  case byteDef: return 1;
  case intDef: return 2;
  case longDef:
  case floatDef: return 4;
  case stringDef:
  case arrayDef:
  case objectDef: return -1;
  default: return 0;
  }
}

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
 * ������ ����
 * INT8   - 'b'
 * @brief LoadClass::readByte
 * @return
 */
uint8_t LoadClass::readByte(){
  uint8_t b = I2CEEPROM.read(device, address);
  address++;
  return b;
}

/**
 * ������ �����
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

/**
 * ������ ������� �����
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

/**
 * ������ ����� � ��������� ������
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

/**
 * ������ ������
 * STRING - 's'
 * @brief LoadClass::readString
 * @param isLazyDelete ���� true, �� ����������� ������� ��������
 *  (�.�. �� ���� ������� �������������)
 * @return
 */
char * LoadClass::readString(bool isLazyDelete){
  int len = readInt() - 2;
  char * str = new char[len + 1];
  if (isLazyDelete) addRef(str);
  I2CEEPROM.read_buffer(device, address, (uint8_t*) str, len);
  str[len] = 0;
  address += len;
  return str;
}

int16_t LoadClass::toNext(CharDefinition cdef, int inc) {
  int size = getDefinitionSize(cdef);
  if (size == -1) {
    for (int i = 0; i < inc; i++) {
      address += readInt() - 2;
    }
  } else {
    address += size * inc;
  }
  return address;
}

/**
 * �������� ������ ������� ��� ��������� ������ ��� ����
 * @brief getLength
 * @param charDef
 * @return
 */
int LoadClass::getRomLength(CharDefinition cdef) {
  int size = getDefinitionSize(cdef);
  return size == -1 ? 2 : size;
}

/**
 * ������ �������� �� EEPROM ������ �� ���� (charDef)
 * @brief LoadClass::readRom
 * @param obj
 * @param pos
 * @param charDef
 */
void LoadClass::readRom(uint8_t * obj, int &pos, CharDefinition cdef) {
  switch (cdef) {
  case charDef:
  case byteDef:
    obj[pos] = readByte();
    pos++;
    break;
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
 * �������� ������ ������� ��� ��������� ������ ��� ����
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
 * ��������� ���������� ����� �� ����� ��������
 * ������������� ������ ��� ��������
 * @brief LoadClass::toObjectItem
 * @param item ����� ��������
 */
int16_t LoadClass::toObjectItem(int item) {
  readInt(); // ���������� ������ ������ �����
  char * definition = newString();
  for (int i = 0; i + 1 < strlen(definition) && i < item; i++) {
    toNext(definition[i], 1);
  }
  delete definition;
  return address;
}

/**
 * ������ ������
 * OBJECT - 'o'
 * @brief LoadClass::readObject
 * @return
 */
void * LoadClass::readObject(int &length){
  readInt(); // ���������� ������ ������ �����
  char * definition = newString();
  int objectLength = getObjectLength(definition);
  uint8_t * obj = addRef(new uint8_t[objectLength]);
  length = 0;
  for (int i = 0; i < strlen(definition); i++) {
    readRom(obj, length, definition[i]);
  }
  delete definition;
  return obj;
}

/**
 * ��������� ���������� ����� �� ����� ��������
 * ������������� ������ ��� ��������
 * @brief LoadClass::toArrayItem
 * @param item ����� ��������
 */
int16_t LoadClass::toArrayItem(int item) {
  readInt(); // ���������� ������ ������ �����
  char charDef = readByte();
  int count = readInt() - 1;
  return toNext(charDef, item < count ? item : count);
}

/**
 * ������ ������
 * ARRAY - 'a'
 * @brief LoadClass::readArray
 * @param count ����� ������ �������
 * @return
 */
void * LoadClass::readArray(int & count) {
#ifdef HAS_SERIAL_DEBUG
  int len = readInt();
#else
  readInt(); // ���������� ������ ������ �����
#endif
  CharDefinition cdef = readByte();
  count = readInt();
  int size = getDefinitionSize(cdef);
#ifdef HAS_SERIAL_DEBUG
  Serial.print("readArray len = ");
  Serial.print(len);
  Serial.print("; charDefinition = ");
  Serial.print(cdef);
  Serial.print("; definitionSize = ");
  Serial.print(size);
  Serial.print("; address = ");
  Serial.println(address, HEX);
#endif
  if (size == -1) {
    /** ������� */
    void * arr = addRef(new uint8_t[count * 2]);
    int pos = 0;
    for (int i = 0; i < count; i++) {
      readRom(arr, pos, cdef);
    }
    return arr;
  } else if (size > 0) {
    /** ������� ���� */
    int arrayLength = count * size;
    void * arr = addRef(new uint8_t[arrayLength]);
    I2CEEPROM.read_buffer(device, address, (uint8_t*) arr, arrayLength);
    return arr;
  }
  return NULL;
}
