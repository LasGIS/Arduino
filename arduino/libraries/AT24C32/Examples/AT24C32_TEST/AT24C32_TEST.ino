#include <Arduino.h>
#include <Wire.h>
#include <LoadClass.h>
#include <SerialBlock.h>

extern void testAT24C32();
extern void testString();
extern void testObject();

void setup() {
  Wire.begin(); // initialise the connection
  Serial.begin(9600);
  //clearEEPROM();
}

void loop() {
}

/**
 * @brief serialEvent
 * ������� ������� �� ����������:
 * <:BR><device><address><size> - ������ ����� ������ � ��������� � ��������� ����� Serial.print();
 * <:BW><SerialBlock> - ������ ����� ������, ����������� �� ����������;
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
  for (int16_t a = 0x00; a < 0x1000; a += 0x100) {
    I2CEEPROM.write_buffer(0x57, a, buffer, 0x100);
  }
  //  for(uint16_t i = 0; i < 0x100; i++) {
  //    I2CEEPROM.write(0x57, i, i & 0xff);
  //  }
  Serial.print('\n');
}
