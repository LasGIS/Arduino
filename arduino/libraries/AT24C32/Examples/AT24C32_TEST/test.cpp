#include <Arduino.h>
#include <SerialBlock.h>
#include <LoadClass.h>
#include "AT24C32_TEST.h"
#include "AT24CTestMemory.h"

struct TestObject {
  char c;
  int8_t b;
  int16_t i;
  int32_t l;
  float f;
  char * s;
};

struct TestObjectArray {
  int8_t int8;
  int16_t int16;
  char * str;
  char chr;
};

/**
 * case '22':
 * @brief testString
 */
void testString() {
  LoadClass lc = LoadClass(DEVICE, 0);
  char* s = lc.readString(EEPROM_ArrayOfObject_0_Object_String);
  Serial.print("=> String(");
  Serial.print((int) s, HEX);
  Serial.print(") = \"");
  Serial.print(s);
  Serial.println("\"");
}

/**
 * case '33':
 * @brief testObject
 */
void testObject() {
  LoadClass lc = LoadClass(DEVICE, 0);
  int len = 0;
  TestObject * obj = (TestObject *) lc.readObject(EEPROM_Object, len);
  Serial.print("=> Object(");
  Serial.print((int) &obj, HEX);
  Serial.print("-");
  Serial.print(len);
  Serial.print(") = ");
  SerialPrintHex((uint8_t *) obj, len);
  Serial.println();
  Serial.print("  CHAR = ");
  Serial.print(obj->c);
  Serial.print(", INT8 = ");
  Serial.print(obj->b);
  Serial.print(", INT16 = ");
  Serial.print(obj->i);
  Serial.print(", INT32 = ");
  Serial.print(obj->l);
  Serial.print(", FLOAT = ");
  Serial.print(obj->f, 6);
  Serial.print(", STRING(");
  Serial.print((int) obj->s, HEX);
  Serial.print(") = ");
  Serial.println(obj->s);
}

/**
 * case '44':
 * @brief testArray
 */
void testArray() {
  LoadClass lc = LoadClass(DEVICE, 0);
  int len = 0;
  int *arr = lc.readArray(EEPROM_ArrayOfInt, len);
  Serial.print("=> testArray(");
  Serial.print(len);
  Serial.println(") = ");
  Serial.print("  ");
  for (int i = 0; i < len; i++) {
    Serial.print(arr[i]);
    Serial.print(", ");
  }
  Serial.println();
}

/**
 * case '55':
 * @brief testArrayObject
 */
void testArrayObject() {
  LoadClass lc = LoadClass(DEVICE, 0);
  int len = 0;
  TestObjectArray ** objs = lc.readArray(EEPROM_ArrayOfObject, len);
  Serial.print("=> testArrayObject(");
  Serial.print(len);
  Serial.println(") = ");
  for (int i = 0; i < len; i++) {
    Serial.print(" Int8 = ");
    Serial.print(objs[i]->int8);
    Serial.print(", Int16 = ");
    Serial.print(objs[i]->int16);
    Serial.print(", String = \"");
    Serial.print(objs[i]->str);
    Serial.print("\", Char = ");
    Serial.print(objs[i]->chr);
    Serial.println(";");
  }
}

/**
 * case '60':
 * @brief testArrayObject
 */
void testJingleBells() {
  Serial.println("testJingleBells");
  LoadClass lc = LoadClass(DEVICE, EEPROM_music_0_JingleBells_music);
  int len = 0;
  uint8_t * objs = lc.readArray(len);
  Serial.print("=> testJingleBells(");
  Serial.print(len);
  Serial.print(") = ");
  SerialPrintHex(objs, len);
  Serial.println();
}

/**
 * case '11':
 * @brief testAT24C32
 */
void testAT24C32() {
  LoadClass lc = LoadClass(DEVICE, EEPROM_Char);
  Serial.print("=> testAT24C32:");
  char c = lc.readChar();
  Serial.print(" Char = ");
  Serial.print(c);

  byte b = lc.readByte();
  Serial.print(", Byte = ");
  Serial.print(b);

  int i = lc.readInt();
  Serial.print(", Int = ");
  Serial.print(i);

  long l = lc.readLong();
  Serial.print(", Long = ");
  Serial.print(l);

  float f = lc.readFloat();
  Serial.print(", Float = ");
  Serial.print(f, 8);

  char* s = lc.readString();
  Serial.print(", String = \"");
  Serial.print(s);
  Serial.println("\"");
}
