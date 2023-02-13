#include <Arduino.h>
#include <Wire.h>
#include <LoadClass.h>
#include <SerialBlock.h>
#include "AT24C32_TEST.h"

extern void testAT24C32();
extern void testString();
extern void testObject();
extern void testArray();
extern void testArrayObject();
extern void testJingleBells();

void setup() {
  Wire.begin(); // initialise the connection
  Serial.begin(9600);
  // clearEEPROM();
}

void loop() {
}

/**
 * @brief serialEvent
 * Входные команды из компьютера:
 * <:BR><device><address><size> - чтение блока памяти и пересылка в компьютер через Serial.print();
 * <:BW><SerialBlock> - запись блока памяти, полученного из компьютера;
 */
void serialEvent() {
  if (Serial.available()) {
    int16_t key = serialReadShort();
    if (key == 0x423A) {
      int8_t bt = serialReadByte();
      if (bt == 'W') {
        serialWriteBlock();
      } else
        if (bt == 'R') {
          serialReadBlock();
        }
    } else {
      switch (key) {
      case '11':
        testAT24C32();
        break;
      case '22':
        testString();
        break;
      case '33':
        testObject();
        break;
      case '44':
        testArray();
        break;
      case '55':
        testArrayObject();
        break;
      case '06':
        testJingleBells();
        break;
      default:
        break;
      }
    }
  }
}

void clearEEPROM() {
  Serial.println("Clear EEPROM:");
  uint8_t buffer[0x100];
  for (int16_t i = 0; i < 0x100; i++) {
    buffer[i]=0;
  }
  for (int16_t a = 0x00; a < 0x100; a += 0x100) {
    I2CEEPROM.write_buffer(DEVICE, a, buffer, 0x100);
  }
  //  for(uint16_t i = 0; i < 0x100; i++) {
  //    I2CEEPROM.write(DEVICE, i, i & 0xff);
  //  }
  Serial.print('\n');
}
