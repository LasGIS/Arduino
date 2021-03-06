TEMPLATE = app

QT += qml quick
CONFIG += c++11

#// THE_ANSWER 42
DEFINES += ARDUINO=10612
DEFINES += F_CPU 16000000L
DEFINES += ARDUINO_AVR_PRO
DEFINES += ARDUINO_ARCH_AVR
DEFINES += __AVR__
DEFINES += __AVR_ATmega328P__

SOURCES += \
    ../libraries/ADXL345/src/ADXL345.cpp \
    ../libraries/DS3231/DS3231.cpp \
    ../libraries/TFT_LG_ILI9225/src/TFT_LG_ILI9225.cpp \
    ../libraries/TFT_LG_ILI9225/src/RussFontANSI.c \
    ../libraries/TFT_LG_ILI9225/src/RussFontOEM.c \
    ../libraries/TFT_LG_ILI9225/src/DefaultFonts.c \
    ../libraries/TFT_LG_ILI9225/examples/Demo/Demo.ino \
    ../libraries/DHT-sensor-library-master/DHT.cpp \
    ../libraries/IrControl/IrControl.cpp \
    ../libraries/MsTimer2/MsTimer2.cpp \
    ../libraries/AT24C32/I2C_EEPROM.cpp \
    ../libraries/AT24C32/Examples/AT24C32_TEST/AT24C32_TEST.ino \
    ../libraries/AT24C32/serial_read_helper.cpp \
    SmartClock20.ino \
    Accel.cpp \
    RealTime.cpp \
    buzzer.cpp \
    note.cpp \
    screen_date_time.cpp \
    screen_timer.cpp \
    screen_tft.cpp \
    screen_dump.cpp

# Specifies include paths that are passed to the Windows Resource Compiler.
INCLUDEPATH += ../libraries/arduino-ds1302-master \
    ../libraries/ADXL345/src \
    ../libraries/DS3231 \
    ../libraries/TFT_LG_ILI9225/src \
    ../libraries/DHT-sensor-library-master \
    ../libraries/IrControl \
    ../libraries/MsTimer2 \
    ../libraries/AT24C32 \
    D:/local/Arduino/hardware/arduino/avr/cores/arduino \
    D:/local/Arduino/hardware/tools/avr/avr/include \
    D:/local/Arduino/hardware/arduino/avr/variants/standard \
    D:/local/Arduino/hardware/arduino/avr/libraries/EEPROM/src \
    D:/local/Arduino/hardware/arduino/avr/libraries/HID/src \
    D:/local/Arduino/hardware/arduino/avr/libraries/SoftwareSerial/src \
    D:/local/Arduino/hardware/arduino/avr/libraries/SPI/src \
    D:/local/Arduino/hardware/arduino/avr/libraries/Wire/src \
    D:/local/Arduino/hardware/arduino/avr/libraries/Wire/src/utility \
    D:/local/Arduino/libraries/SD/src \
    D:/local/Arduino/libraries/Servo/src

HEADERS += \
    ../libraries/DHT-sensor-library-master/DHT.h \
    ../libraries/IrControl/IrControl.h \
    ../libraries/MsTimer2/MsTimer2.h \
    ../libraries/ADXL345/src/ADXL345.h \
    ../libraries/DS3231/DS3231.h \
    ../libraries/TFT_LG_ILI9225/src/TFT_LG_ILI9225.h \
    ../libraries/AT24C32/I2C_EEPROM.h \
    SmartClock20.h \
    pitches.h \
    note.h \
    screen_tft.h \
    screen_date_time.h \
    screen_timer.h \
    screen_dump.h \
    I2CMemory.h

DISTFILES += \
    ../libraries/ADXL345/library.json \
    ../libraries/TFT_LG_ILI9225/library.json \
    ../libraries/ADXL345/lib/readme.txt \
    ../libraries/ADXL345/keywords.txt \
    ../libraries/ADXL345/library.properties \
    ../libraries/ADXL345/License.txt \
    ../libraries/ADXL345/platformio.ini \
    ../libraries/TFT_LG_ILI9225/keywords.txt \
    ../libraries/TFT_LG_ILI9225/library.properties \
    ../libraries/ADXL345/README.md \
    ../libraries/TFT_LG_ILI9225/README.md \
    ../libraries/DS3231/keywords.txt \
    ../libraries/DS3231/Readme.txt \
    ../libraries/AT24C32/keywords.txt \
    I2CMemory.data \
    I2CMemory.xml \
    I2CMemory.hex
