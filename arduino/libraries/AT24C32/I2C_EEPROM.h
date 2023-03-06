#ifndef I2C_EEPROM_h
#define I2C_EEPROM_h

#include <inttypes.h>

#define EEPROM_DEVICE 0x00

//#define HAS_SERIAL
//#define HAS_SERIAL_DEBUG

class I2C_EEPROM {
public:
  uint8_t read(uint8_t device, uint16_t address);
  void write(uint8_t device, uint16_t address, uint8_t data);
  void write_buffer(uint8_t device, uint16_t address, uint8_t* data, uint16_t length);
  void read_buffer(uint8_t device, uint16_t address, uint8_t* data, uint16_t length);
private:
  void beginTransmission(uint8_t device, uint16_t address);
};

static I2C_EEPROM __attribute__ ((unused)) I2CEEPROM;

#endif // I2C_EEPROM

