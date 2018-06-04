#include <Wire.h>
#include "Arduino.h"
#include "I2C_EEPROM.h"

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
  beginTransmission(device, address);
  for (uint8_t c = 0; c < length; c++) {
    Wire.write(data[c]);
  }
  Wire.endTransmission();
  delay(10);
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

/**
 * Читаем один int (2 байта) из Serial
 */
int16_t I2C_EEPROM::serialReadInteger() {
  byte bt[2]; if (Serial.readBytes(bt, 2) == 2) {
    return makeWord(bt[0], bt[1]);
  }
  return -1;
}

/**
 * Читаем один блок данных для закачки в EEPROM из Serial
 * Блок состоит из:
 * 0 - int16_t = "EB" Признак, что это EEPROM Block
 * 5 - int8_t  - (size) Размер блока
 * 2 - int8_t  - (device) номер микосхемы (0x57 для CMOS)
 * 3 - int16_t - (address) блока в EEPROM памяти
 * 7 - int16_t - Контрольная сумма блока
 * 9 - byte[size] - сам блок
 */
SerialBlock * I2C_EEPROM::serialReadBlock() {
  while (Serial.available() < 6) delayMicroseconds(10);
  int8_t size = Serial.read();
  int8_t device = Serial.read();
  int16_t address = serialReadInteger();
  int16_t cs = serialReadInteger();
  if (size > 0 && size < 22) {
    byte * body = new byte[size];
    int len = Serial.readBytes(body, size);
    if (len == size) {
      SerialBlock * sb = new SerialBlock(size, device, address, cs, body);
      return sb;
    }
  }
  return NULL;
}

SerialBlock::SerialBlock(
    int8_t _size,
    int8_t _device,
    int16_t _address,
    int16_t _cs,
    byte * _body
) {
  size = _size;
  device = _device;
  address = _address;
  cs = _cs;
  body = _body;
}

SerialBlock::~SerialBlock() {
  if (NULL != body) delete[] body;
}
