#include <Arduino.h>
#include <Wire.h>
#include <LoadClassRef.h>
#include <SerialBlock.h>
#include "AT24C32_TEST.h"

extern void testAT24C32();
extern void testString();
extern void testObject();
extern void testObjectToItem();
extern void testArray();
extern void testArrayToItem();
extern void testArrayObject();
extern void testArrayObjectToItem();
extern void testArrayObjectAreal();
extern void testJingleBells();
extern void testJingleBellsToItem();
extern void testLoadMusics();
extern void testLoadMusicsRef();

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
    uint8_t key;
    if ((key = serialReadByte()) == 0x3A && serialReadByte() == 0x42) {
      int8_t bt = serialReadByte();
      if (bt == 'W') {
        serialWriteBlock();
      } else if (bt == 'R') {
        serialReadBlock();
      }
    } else {
      switch (key) {
        case '1':
          testAT24C32();
          break;
        case '2':
          testString();
          break;
        case '3':
          {
            if (serialReadByte() == 0x31) {
              testObjectToItem();
            } else {
              testObject();
            }
          }
          break;
        case '4':
          {
            if (serialReadByte() == 0x31) {
              testArrayToItem();
            } else {
              testArray();
            }
          }
          break;
        case '5':
          {
            key = serialReadByte();
            switch (key) {
              case '1':
                testArrayObjectToItem();
                break;
              case '2':
                testArrayObjectAreal();
                break;
              default:
                testArrayObject();
            }
          }
          break;
        case '6':
          {
            if (serialReadByte() == 0x31) {
              testJingleBellsToItem();
            } else {
              testJingleBells();
            }
          }
          break;
        case '7':
          {
            if (serialReadByte() == 0x31) {
              testLoadMusicsRef();
            } else {
              testLoadMusics();
            }
          }
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
    buffer[i] = 0;
  }
  for (int16_t a = 0x00; a < 0x1000; a += 0x100) {
    I2CEEPROM.write_buffer(DEVICE, a, buffer, 0x100);
  }
  for (int16_t a = 0x00; a < 0x400; a += 0x100) {
    I2CEEPROM.write_buffer(EEPROM_DEVICE, a, buffer, 0x100);
  }
  Serial.print('\n');
}
