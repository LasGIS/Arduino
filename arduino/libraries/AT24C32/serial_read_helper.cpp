#include <Arduino.h>
#include "I2C_EEPROM.h"


inline int8_t charToHex(char ch) {
  return ((ch < 'A') ? (ch - '0') : (ch - 'A' + 0xA)) & 0x0F;
}

inline char byteToHexChar(int8_t bt) {
  return ((bt < 0xA) ? (bt + '0') : (bt + 'A' - 0xA));
}

void SerialPrintHex(int8_t bt) {
  char out[3];
  out[0] = byteToHexChar((bt >> 4) & 0xf);
  out[1] = byteToHexChar(bt & 0xf);
  out[2] = 0;
  Serial.print(out);
}
/*
size_t SerialReadHexBytes(int8_t *buffer, size_t length) {
  char * charBuf = new char[length * 2 + 1];
  size_t len = Serial.readBytes(charBuf, length * 2);
  Serial.print("in:");
  charBuf[length * 2] = 0;
  Serial.println(charBuf);
  Serial.print("out:");
  if (len == length * 2) {
    for (size_t i = 0; i < length; i++) {
      int8_t bt = charToHex(charBuf[i * 2]) << 4 | charToHex(charBuf[i * 2 + 1]);
      SerialPrintHex(bt);
      buffer[i] = bt;
    }
    Serial.println();
  }
  delete charBuf;
  return len;
}
*/
