#include <Arduino.h>
#include <SerialBlock.h>

#define READ_BLOCK_LENGTH 16

int8_t charToHex(char ch) {
  return ((ch < 'A') ? (ch - '0') : (ch - 'A' + 0xA)) & 0x0F;
}

char byteToHexChar(uint8_t bt) {
  return ((bt < 0xA) ? (bt + '0') : (bt + 'A' - 0xA));
}

void SerialPrintHex(uint8_t bt) {
  char out[3];
  out[0] = byteToHexChar((bt >> 4) & 0xf);
  out[1] = byteToHexChar(bt & 0xf);
  out[2] = 0;
  Serial.print(out);
}
void SerialPrintHex(uint8_t * obj, int len) {
  for (int i = 0; i < len; i++) {
    if (i != 0) {
      Serial.print(',');
    }
    SerialPrintHex(obj[i]);
  }
}

/** Читаем один байт из Serial */
uint8_t serialReadByte() {
  uint8_t bt;
  return (Serial.readBytes(&bt, 1) == 1) ? bt : -1;
}

/**
 * Читаем один short (2 байта) из Serial
 */
uint16_t serialReadShort() {
  uint8_t bt[2];
  if (Serial.readBytes(bt, 2) == 2) {
    return makeWord(bt[1], bt[0]);
  }
  return -1;
}

/**
 * Читаем один блок данных и закачиваем его в EEPROM
 * Блок состоит из:
 * 0 - int8_t  - (device) номер микросхемы (0x57 для CMOS)
 * 1 - int16_t - (address) блока в EEPROM памяти
 * 3 - int16_t - (size) Размер блока
 * 5 - int16_t - Контрольная сумма блока
 * 7 - byte[size] - сам блок
 */
void serialWriteBlock() {
  SerialBlock * sb = new SerialBlock();
  size_t len = Serial.readBytes((uint8_t *) sb, 7);
#ifdef HAS_SERIAL
  Serial.print("sizeof(SerialBlock) = ");
  Serial.print(sizeof(SerialBlock));
  Serial.print("; len = ");
  Serial.print(len);
  Serial.print("; device = ");
  Serial.print(sb->device, HEX);
  Serial.print("; address = ");
  Serial.print(sb->address, HEX);
  Serial.print("; block size = ");
  Serial.print(sb->size);
  Serial.print("; cs = ");
  Serial.println(sb->cs);
#endif
  if (len == 7) {
    if (sb->size > 0 && sb->size < 22) {
      sb->body = new uint8_t[sb->size];
      len = Serial.readBytes((uint8_t *) sb->body, sb->size);
      if (len == sb->size) {
        // обязательный ответ
        Serial.print(":adr=");
        Serial.print(sb->device);
        Serial.print(",");
        Serial.println(sb->address);
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
  SerialBlock * sb = new SerialBlock();
#ifdef HAS_SERIAL
  size_t size = Serial.readBytes((uint8_t *) sb, 7);
  Serial.print("device = ");
  Serial.print(sb->device, HEX);
  Serial.print("; address = ");
  Serial.print(sb->address, HEX);
  Serial.print("; size = ");
  Serial.println(sb->size);
  Serial.print("; cs = ");
  Serial.println(sb->cs);
#else
  Serial.readBytes((uint8_t *) sb, 7);
#endif
  uint8_t buf[32];
  uint16_t len;
  for (uint16_t l = 0; l < sb->size; l += READ_BLOCK_LENGTH) {
    if (sb->size - l > READ_BLOCK_LENGTH) {
      len = READ_BLOCK_LENGTH;
    } else {
      len = sb->size - l;
    }
    I2CEEPROM.read_buffer(sb->device, sb->address + l, buf, len);
    Serial.print(":");
    SerialPrintHex(buf, len);
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
