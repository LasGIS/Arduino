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

void testString() {
  LoadClass lc = LoadClass(DEVICE, 0);
  char* s = lc.readString(EEPROM_ArrayOfObject_0_Object_String);
  Serial.print("String = ");
  Serial.println(s);
  lc.deleteString(s);
}

void testObject() {
  LoadClass lc = LoadClass(DEVICE, 0);
  int len = 0;
  char * definition;
  TestObject * obj = (TestObject *) lc.readObject(EEPROM_Object, len, definition);
// *
  Serial.println(">");
  Serial.print(definition);
  Serial.print(" => ");
  Serial.print("Object(");
  Serial.print((int) &obj, HEX);
  Serial.print("-");
  Serial.print(len);
  Serial.print(") = ");
  SerialPrintHex((uint8_t *) obj, len);
  Serial.println();
  Serial.println(obj->c);
  Serial.println(obj->b);
  Serial.println(obj->i);
  Serial.println(obj->l);
  Serial.println(obj->f, 6);
  Serial.print((int) obj->s, HEX);
  Serial.print("-");
  Serial.println(obj->s);
//*/
  lc.deleteObject(definition, (uint8_t *) obj);
}

void testArray() {
  LoadClass lc = LoadClass(DEVICE, 0);
  int len = 0;
  int16_t * arr = lc.readArray(EEPROM_ArrayOfInt, len);
  Serial.print("testArray len = ");
  Serial.println(len);
  for (int i = 0; i < len; i++) {
    if (i>0) {
      Serial.print(", ");
    }
    Serial.print(arr[i]);

  }
  Serial.println();
  lc.deleteArray(arr);
}

void testAT24C32() {
  LoadClass lc = LoadClass(DEVICE, EEPROM_Char);
  char c = lc.readChar();
  Serial.print("Char = ");
  Serial.println(c);

  byte b = lc.readByte();
  Serial.print("Byte = ");
  Serial.println(b);

  int i = lc.readInt();
  Serial.print("Int = ");
  Serial.println(i);

  long l = lc.readLong();
  Serial.print("Long = ");
  Serial.println(l);

  float f = lc.readFloat();
  Serial.print("Float = ");
  Serial.println(f, 8);

  char* s = lc.readString();
  Serial.print("String = ");
  Serial.println(s);
  lc.deleteString(s);

  testString();
}
