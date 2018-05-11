#include <Wire.h> //I2C library
#include "I2C_EEPROM.h"

void setup() {
  Wire.begin(); // initialise the connection
  Serial.begin(9600);
//  Serial.println("Write:");
//  for(uint16_t i = 0; i < 0x1000; i++) {
//    I2CEEPROM.write(0x57, i, 0xaa);
//  }
//  Serial.print('\n');
}

void loop() {
  Serial.println("Read:");
  for (uint16_t addr = 0 ; addr < 0x1000; addr += 16) {
    Serial.print(addr, HEX);
    Serial.print(": ");
    for (uint16_t i = 0 ; i < 16; i++) {
      uint8_t b = I2CEEPROM.read(0x57, addr + i); //access an address from the memory
      Serial.print(b, HEX);
      Serial.print(" ");
    }
    Serial.print(";\n");
  }
  delay(20000);
}
