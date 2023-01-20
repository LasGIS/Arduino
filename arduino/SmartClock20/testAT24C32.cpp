#include <Arduino.h>
#include <SerialBlock.h>
#include "I2CMemory.h"

#define DEVICE 0x57

void testDouble() {
    double dbl = 0.0;
    float flt = 0.01;
    Serial.println(sizeof(flt));
    Serial.println(sizeof(dbl));
//    I2CEEPROM.write_buffer(DEVICE, EEPROM_Double_ADDRESS, (uint8_t*) &dbl, (uint8_t) 8);
    I2CEEPROM.read_buffer(DEVICE, EEPROM_Double_ADDRESS, (uint8_t*) &dbl, (uint8_t) 4);
    Serial.print("Double = ");
    Serial.println(dbl, 8);
}

void testAT24C32() {
  Serial.println("testAT24C32.cpp");
  char c = I2CEEPROM.read(DEVICE, EEPROM_Char_ADDRESS);
  Serial.print("Char = ");
  Serial.println(c);

  byte b = I2CEEPROM.read(DEVICE, EEPROM_Byte_ADDRESS);
  Serial.print("Byte = ");
  Serial.println(b);

  int i = 0;
  I2CEEPROM.read_buffer(DEVICE, EEPROM_Int_ADDRESS, (uint8_t*) &i, (uint8_t) 2);
  Serial.print("Int = ");
  Serial.println(i);

  long l = 0;
  I2CEEPROM.read_buffer(DEVICE, EEPROM_Long_ADDRESS, (uint8_t*) &l, (uint8_t) 4);
  Serial.print("Long = ");
  Serial.println(i);

  float f = 0.01;
  I2CEEPROM.read_buffer(DEVICE, EEPROM_Float_ADDRESS, (uint8_t*) &f, (uint8_t) 4);
  Serial.print("Float = ");
  Serial.println(f);

  testDouble();
}
