#ifndef I2C_EEPROM_h
#define I2C_EEPROM_h

#include <inttypes.h>

class I2C_EEPROM {
  public:
    void init(uint8_t _deviceaddress);
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);
    void write_buffer(uint16_t address, uint8_t* data, uint8_t length);
    void read_buffer(uint16_t address, uint8_t* data, uint8_t length);
private:
    uint8_t deviceaddress;
};

static I2C_EEPROM I2CEEPROM;

#endif // I2C_EEPROM

