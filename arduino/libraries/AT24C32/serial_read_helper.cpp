#include <Arduino.h>
#include "I2C_EEPROM.h"

#define READ_BLOCK_LENGTH 31

int8_t charToHex(char ch) {
  return ((ch < 'A') ? (ch - '0') : (ch - 'A' + 0xA)) & 0x0F;
}

char byteToHexChar(int8_t bt) {
  return ((bt < 0xA) ? (bt + '0') : (bt + 'A' - 0xA));
}

void SerialPrintHex(int8_t bt) {
  char out[3];
  out[0] = byteToHexChar((bt >> 4) & 0xf);
  out[1] = byteToHexChar(bt & 0xf);
  out[2] = 0;
  Serial.print(out);
}

/** Читаем один байт из Serial */
int8_t serialReadByte() {
  uint8_t bt;
  return (Serial.readBytes(&bt, 1) == 1) ? bt : -1;
}

/**
 * Читаем один short (2 байта) из Serial
 */
int16_t serialReadShort() {
  uint8_t bt[2]; if (Serial.readBytes(bt, 2) == 2) {
    return makeWord(bt[1], bt[0]);
  }
  return -1;
}

/**
 * Читаем один блок данных и закачиваем его в EEPROM
 * Блок состоит из:
 * 0 - int8_t  - (size) Размер блока
 * 1 - int8_t  - (device) номер микосхемы (0x57 для CMOS)
 * 2 - int16_t - (address) блока в EEPROM памяти
 * 5 - int16_t - Контрольная сумма блока
 * 7 - byte[size] - сам блок
 */
void serialWriteBlock() {
  SerialBlock * sb = new SerialBlock();
  int len = Serial.readBytes((byte *) sb, 6);
  if (len == 6) {
    if (sb->size > 0 && sb->size < 22) {
      sb->body = new byte[sb->size];
      len = Serial.readBytes((char *) sb->body, sb->size);
      if (len == sb->size) {
        // обязательный ответ
        Serial.print(":adr=");
        Serial.println(sb->address);
        /*? todo: надо проверять КС ?*/
        I2CEEPROM.write_buffer(sb->device, sb->address, sb->body, sb->size);
#ifdef HAS_SERIAL
        Serial.print("block size = ");
        Serial.print(sb->size);
        Serial.print("; device = ");
        Serial.print(sb->device, HEX);
        Serial.print("; address = ");
        Serial.print(sb->address, HEX);
        Serial.print("; cs = ");
        Serial.println(sb->cs);
        Serial.print("; body = ");
        Serial.print((int) sb->body, HEX);
        Serial.print("; \"");
        for (int i = 0; i < sb->size; i++) {
          SerialPrintHex(sb->body[i]);
        }
        Serial.println("\"");
#endif
      }
    }
  }
  delete sb;
}

/**
 * Читаем один блок данных из EEPROM и посылаем его в Serial
 * вход состоит из:
 * 1 - int8_t  - (device) номер микосхемы (0x57 для CMOS)
 * 2 - int16_t - (address) блока в EEPROM памяти
 * 0 - int16_t - (size) Размер блока
 */
void serialReadBlock() {
  uint8_t device = serialReadByte();
  uint16_t address = serialReadShort();
  uint16_t size = serialReadShort();
#ifdef HAS_SERIAL
  Serial.print("device = ");
  Serial.print(device, HEX);
  Serial.print("; address = ");
  Serial.print(address, HEX);
  Serial.print("; size = ");
  Serial.println(size);
#endif
  uint8_t buf[32];
  uint16_t len;
  for (uint16_t l = 0; l < size; l += READ_BLOCK_LENGTH) {
    if (size - l > READ_BLOCK_LENGTH) {
      len = READ_BLOCK_LENGTH;
    } else {
      len = size - l;
    }
    I2CEEPROM.read_buffer(device, address + l, buf, len);
//    for (uint16_t i = 0; i < len; i++) {
//      buf[i] = I2CEEPROM.read(device, address + l + i);
//    }
    Serial.print(":");
    for (uint16_t i = 0; i < READ_BLOCK_LENGTH && l + i < size; i++) {
      SerialPrintHex(buf[i]);
    }
    Serial.println();
  }
}

SerialBlock::~SerialBlock() {
#ifdef HAS_SERIAL
  Serial.print("delete ");
  Serial.println((int) this, HEX);
#endif
  if (NULL != body) {
    delete body;
    body = NULL;
  }
}
