#include <Wire.h>
#include "Arduino.h"
#include "I2C_EEPROM.h"

void I2C_EEPROM::init(uint8_t _deviceaddress) {
  deviceaddress = _deviceaddress;
}

void I2C_EEPROM::write(uint16_t address, uint8_t data) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((uint8_t)(address >> 8)); // MSB
  Wire.write((uint8_t)(address & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(2);
}

uint8_t I2C_EEPROM::read(uint16_t address) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((uint8_t)(address >> 8)); // MSB
  Wire.write((uint8_t)(address & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  return Wire.available() ? Wire.read() : 0xFF;;
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void I2C_EEPROM::write_buffer(uint16_t address, uint8_t* data, uint8_t length) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(address >> 8)); // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  for (uint8_t c = 0; c < length; c++) {
    Wire.write(data[c]);
  }
  Wire.endTransmission();
}

// maybe let's not read more than 30 or 32 bytes at a time!
void I2C_EEPROM::read_buffer(uint16_t address, uint8_t* data, uint8_t length) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(address >> 8)); // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, length);
  for (uint8_t c = 0; c < length; c++ ) {
    if (Wire.available()) data[c] = Wire.read();
  }
}

//static I2C_EEPROM I2CEEPROM;
