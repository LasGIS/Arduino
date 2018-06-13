#include <Wire.h>
#include "Arduino.h"
#include "I2C_EEPROM.h"

#define MAX_BLOCK_LENGTH 0x20
//#define BLOCK_BOUND_SIZE 0x20
#define BLOCK_BOUND_MASK 0XFFE0

void I2C_EEPROM::beginTransmission(uint8_t device, uint16_t address) {
  Wire.beginTransmission(device);
  Wire.write(highByte(address)); // MSB
  Wire.write(lowByte(address)); // LSB
}

void I2C_EEPROM::write(uint8_t device, uint16_t address, uint8_t data) {
  beginTransmission(device, address);
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}

uint8_t I2C_EEPROM::read(uint8_t device, uint16_t address) {
  beginTransmission(device, address);
  Wire.endTransmission();
  Wire.requestFrom(device, 1);
  return Wire.available() ? Wire.read() : 0xFF;;
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void I2C_EEPROM::write_buffer(
    uint8_t device, uint16_t address,
    uint8_t* data, uint8_t length
) {
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
/*
    Serial.print("["); Serial.print(address, HEX); Serial.print("]:");
    for (uint16_t j = 0; j < len; j++) {
      SerialPrintHex(data[i + j]);
    }
    Serial.println(";");
*/
    delay(10);
    i += len;
    address += len;
  }
}

// maybe let's not read more than 30 or 32 bytes at a time!
void I2C_EEPROM::read_buffer(
    uint8_t device, uint16_t address,
    uint8_t* data, uint8_t length
) {
  beginTransmission(device, address);
  Wire.endTransmission();
  Wire.requestFrom(device, length);
  for (uint8_t c = 0; c < length; c++ ) {
    if (Wire.available()) data[c] = Wire.read();
  }
}
