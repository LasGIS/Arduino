#include <Wire.h>
#include <EEPROM.h>
#include "Arduino.h"
#include "I2C_EEPROM.h"

#define MAX_BLOCK_LENGTH 30
#define BLOCK_BOUND_MASK 0XFFE0

void I2C_EEPROM::beginTransmission(uint8_t device, uint16_t address) {
  Wire.beginTransmission(device);
  Wire.write(highByte(address)); // MSB
  Wire.write(lowByte(address)); // LSB
}

uint8_t I2C_EEPROM::read(uint8_t device, uint16_t address) {
  if (device == EEPROM_DEVICE) {
    return EEPROM.read(address);
  }
  beginTransmission(device, address);
  Wire.endTransmission();
  Wire.requestFrom(device, (uint8_t) 1);
  return Wire.available() ? Wire.read() : 0xFF;
}

void I2C_EEPROM::write(uint8_t device, uint16_t address, uint8_t data) {
  if (device == EEPROM_DEVICE) {
    EEPROM.write(address, data);
    return;
  }
  beginTransmission(device, address);
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void I2C_EEPROM::write_buffer(uint8_t device, uint16_t address, uint8_t* data, uint16_t length) {
  if (device == EEPROM_DEVICE) {
    for(int i = 0; i < length; i++) {
      EEPROM.write(address + i, data[i]);
    }
    return;
  }
  for (uint16_t i = 0; i < length;) {
    uint16_t len, addrBound;
    if (length - i > MAX_BLOCK_LENGTH) {
      len = MAX_BLOCK_LENGTH;
    } else {
      len = length - i;
    }
    addrBound = (address + len) & BLOCK_BOUND_MASK;
    if (addrBound > address) {
      len = addrBound - address;
    }
    beginTransmission(device, address);
    Wire.write(data + i, len);
    Wire.endTransmission();
    delay(10);
    i += len;
    address += len;
  }
}

// maybe let's not read more than 30 or 32 bytes at a time!
void I2C_EEPROM::read_buffer(uint8_t device, uint16_t address, uint8_t* data, uint16_t length) {
  if (device == EEPROM_DEVICE) {
    for(int i = 0; i < length; i++) {
      data[i] = EEPROM.read(address + i);
    }
    return;
  }
  for (uint16_t offset = 0; offset < length; offset += BUFFER_LENGTH) {
    uint16_t len = length - offset > BUFFER_LENGTH ? BUFFER_LENGTH : length - offset;
    beginTransmission(device, address + offset);
    Wire.endTransmission();
    Wire.requestFrom(device, len);
    for (uint16_t j = 0; j < len; j++ ) {
      if (Wire.available()) data[offset + j] = Wire.read();
    }
  }
}
