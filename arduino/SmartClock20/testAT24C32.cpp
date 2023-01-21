#include <Arduino.h>
#include <LoadClass.h>
#include "I2CMemory.h"

#define DEVICE 0x57

void testString() {
    LoadClass lc = LoadClass(DEVICE, 0);
    char* s = lc.readString(EEPROM_ArrayOfObject_0_Object_String_ADDRESS);
    Serial.print("String = ");
    Serial.println(s);
    lc.deleteString(s);
}

void testAT24C32() {
  Serial.println("testAT24C32.cpp");
  LoadClass lc = LoadClass(DEVICE, EEPROM_Char_ADDRESS);
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
