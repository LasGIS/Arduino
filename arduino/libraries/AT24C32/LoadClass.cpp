#include <Arduino.h> // todo remove
#include <string.h>

#include <LoadClass.h>
#include <SerialBlock.h>

// #define HAS_SERIAL_DEBUG

LoadClass::LoadClass(int8_t device, AddressEeprom address) {
  this->device = device;
  this->address = address;
  refs = (void**) calloc(10, sizeof(void*));
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
    delete (uint8_t *) ref;
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
 * @brief LoadClass::readByte
 * @return
 */
uint8_t LoadClass::readByte(){
  uint8_t b = I2CEEPROM.read(device, address);
  address++;
  return b;
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

/**
 * Читаем строку
 * STRING - 's'
 * @brief LoadClass::readString
 * @param isLazyDelete если true, то применяется ленивое удаление
 *  (т.е. не надо удалять принудительно)
 * @return
 */
char * LoadClass::readString(bool isLazyDelete){
  int len = readInt();
  char * str = new char[len + 1];
  if (isLazyDelete) addRef(str);
  I2CEEPROM.read_buffer(device, address, (uint8_t*) str, len);
  str[len] = 0;
  address += len;
  return str;
}

AddressEeprom LoadClass::toNext(CharDefinition cdef, int inc) {
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
 * Переводим внутренний адрес на адрес элемента
 * Действительно только для объектов
 * @brief LoadClass::toObjectItem
 * @param item номер элемента
 * @return адрес элемента в EEPROM
 */
AddressEeprom LoadClass::toObjectItem(uint16_t item) {
  readInt(); // пропускаем размер своего блока
  CharDefinition * definition = (CharDefinition *) newString();
  for (uint16_t i = 0; i + 1 < strlen((char *) definition) && i < item; i++) {
    toNext(definition[i], 1);
  }
  delete definition;
  return address;
}

/**
 * Переводим внутренний адрес на адрес элемента
 * Действительно только для массивов
 * @brief LoadClass::toArrayItem
 * @param item номер элемента
 */
AddressEeprom LoadClass::toArrayItem(int item) {
  readInt(); // пропускаем размер своего блока
  CharDefinition charDef = (CharDefinition) readByte();
  int count = readInt() - 1;
  return toNext(charDef, item < count ? item : count);
}

/**
 * Получаем длинну объекта для выделения памяти под него
 * @brief LoadClass::getObjectLength
 * @return размер памияти для выделения
 */
int LoadClass::getObjectLength() {
  readInt(); // пропускаем размер своего блока
  CharDefinition * definition = (CharDefinition *) newString();
  int length = 0;
  for (uint16_t i = 0; i < strlen((char *) definition); i++) {
    switch (definition[i]) {
      case stringDef: {
        address += readInt();
        length += 2;
      } break;
      case arrayDef: {
        int count;
        length += getArrayLength(count);
      } break;
      case objectDef:
        length += getObjectLength();
        break;
      default: {
        int size = getDefinitionSize(definition[i]);
        address += size;
        length += size;
      } break;
    }
  }
  delete definition;
  return length;
}

/**
 * Получаем длинну массива для выделения памяти под него
 * @brief LoadClass::getArrayLength
 * @param count число элементов
 * @return размер памияти для выделения
 */
int LoadClass::getArrayLength(int &count) {
  readInt(); // пропускаем размер своего блока
  CharDefinition cdef = (CharDefinition) readByte();
  count = readInt();
  switch (cdef) {
    case stringDef:
      address += readInt();
      return 2;
    case arrayDef: {
      int countDef;
      return getArrayLength(countDef);
    }
    case objectDef:
      return getObjectLength();
    default: {
      int size = count * getDefinitionSize(cdef);
      address += size;
      return size;
    }
  }
}

/**
 * Читаем знвчение из EEPROM памяти по типу (charDef)
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
    int posCount = pos;
    pos += 2;
    int count;
    pos += readArray(obj + pos, count);
    memcpy(obj + posCount, (const void*) &count, 2);
  } break;
  case objectDef: {
    pos += readObject(obj + pos);
  } break;
  default: break;
  }
}

/**
 * Читаем объект
 * OBJECT - 'o'
 * @brief LoadClass::readObject
 * @return новый объект
 */
int LoadClass::readObject(void * obj) {
  readInt(); // пропускаем размер своего блока
  CharDefinition * definition = (CharDefinition *) newString();
  int pos = 0;
  for (uint16_t i = 0; i < strlen((char *) definition); i++) {
    readRom((uint8_t *) obj, pos, definition[i]);
  }
  delete definition;
  return pos;
}

/**
 * Читаем массив
 * ARRAY - 'a'
 * @brief LoadClass::readArray
 * @param count число членов массива
 * @return
 */
int LoadClass::readArray(void * arr, int &count) {
  readInt(); // пропускаем размер своего блока
  CharDefinition cdef = (CharDefinition) readByte();
  int pos = 0;
  count = readInt();
  switch (cdef) {
    case stringDef:
    case arrayDef:
    case objectDef: {
      for (int i = 0; i < count; i++) {
        readRom((uint8_t *) arr, pos, cdef);
      }
    } break;
    default: {
      int size = count * getDefinitionSize(cdef);
      I2CEEPROM.read_buffer(device, address, (uint8_t*) arr, size);
      address += size;
    } break;
  }
  return pos;
}
