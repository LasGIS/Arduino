TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

#// THE_ANSWER 42
DEFINES += ARDUINO=10612
DEFINES += F_CPU 16000000L
DEFINES += ARDUINO_AVR_PRO
DEFINES += ARDUINO_ARCH_AVR
DEFINES += __AVR__
DEFINES += __AVR_ATmega328P__

SOURCES += \
    C:/Arduino/arduino/libraries/ADXL345/src/ADXL345.cpp \
    C:/Arduino/arduino/libraries/AT24C32/LoadClass.cpp \
    C:/Arduino/arduino/libraries/AT24C32/LoadClassRef.cpp \
    C:/Arduino/arduino/libraries/AT24C32/LoadClassCommon.cpp \
    C:/Arduino/arduino/libraries/AT24C32/SerialBlock.cpp \
    C:/Arduino/arduino/libraries/DS3231/DS3231.cpp \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/src/TFT_LG_ILI9225.cpp \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/src/RussFontANSI.c \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/src/RussFontOEM.c \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/src/DefaultFonts.c \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/examples/Demo/Demo.ino \
    C:/Arduino/arduino/libraries/DHT-sensor-library-master/DHT.cpp \
    C:/Arduino/arduino/libraries/IrControl/IrControl.cpp \
    C:/Arduino/arduino/libraries/MsTimer2/MsTimer2.cpp \
    C:/Arduino/arduino/libraries/AT24C32/I2C_EEPROM.cpp \
    C:/Arduino/arduino/libraries/AT24C32/Examples/AT24C32_TEST/AT24C32_TEST.ino \
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
INCLUDEPATH += \
    C:/Arduino/arduino/libraries/ADXL345/src \
    C:/Arduino/arduino/libraries/DS3231 \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/src \
    C:/Arduino/arduino/libraries/DHT-sensor-library-master \
    C:/Arduino/arduino/libraries/IrControl \
    C:/Arduino/arduino/libraries/MsTimer2 \
    C:/Arduino/arduino/libraries/AT24C32 \
    C:/Arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino \
    C:/Arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/avr/include \
    C:/Arduino15/packages/arduino/hardware/avr/1.8.6/variants/standard \
    C:/Arduino15/packages/arduino/hardware/avr/1.8.6/libraries/EEPROM/src \
    C:/Arduino15/packages/arduino/hardware/avr/1.8.6/libraries/HID/src \
    C:/Arduino15/packages/arduino/hardware/avr/1.8.6/libraries/SoftwareSerial/src \
    C:/Arduino15/packages/arduino/hardware/avr/1.8.6/libraries/SPI/src \
    C:/Arduino15/packages/arduino/hardware/avr/1.8.6/libraries/Wire/src \
    C:/Arduino15/packages/arduino/hardware/avr/1.8.6/libraries/Wire/src/utility \
    D:/local/Arduino/libraries/SD/src \
    D:/local/Arduino/libraries/Servo/src

HEADERS += \
    C:/Arduino/arduino/libraries/AT24C32/LoadClass.h \
    C:/Arduino/arduino/libraries/AT24C32/LoadClassRef.h \
    C:/Arduino/arduino/libraries/AT24C32/LoadClassCommon.h \
    C:/Arduino/arduino/libraries/AT24C32/SerialBlock.h \
    C:/Arduino/arduino/libraries/DHT-sensor-library-master/DHT.h \
    C:/Arduino/arduino/libraries/IrControl/IrControl.h \
    C:/Arduino/arduino/libraries/MsTimer2/MsTimer2.h \
    C:/Arduino/arduino/libraries/ADXL345/src/ADXL345.h \
    C:/Arduino/arduino/libraries/DS3231/DS3231.h \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/src/TFT_LG_ILI9225.h \
    C:/Arduino/arduino/libraries/AT24C32/I2C_EEPROM.h \
    SmartClock20.h \
    pitches.h \
    note.h \
    screen_tft.h \
    screen_date_time.h \
    screen_timer.h \
    screen_dump.h \
    I2CMemory.h

DISTFILES += \
    C:/Arduino/arduino/libraries/ADXL345/library.json \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/library.json \
    C:/Arduino/arduino/libraries/ADXL345/lib/readme.txt \
    C:/Arduino/arduino/libraries/ADXL345/keywords.txt \
    C:/Arduino/arduino/libraries/ADXL345/library.properties \
    C:/Arduino/arduino/libraries/ADXL345/License.txt \
    C:/Arduino/arduino/libraries/ADXL345/platformio.ini \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/keywords.txt \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/library.properties \
    C:/Arduino/arduino/libraries/ADXL345/README.md \
    C:/Arduino/arduino/libraries/TFT_LG_ILI9225/README.md \
    C:/Arduino/arduino/libraries/DS3231/keywords.txt \
    C:/Arduino/arduino/libraries/DS3231/Readme.txt \
    C:/Arduino/arduino/libraries/AT24C32/keywords.txt \
    I2CMemory.data \
    I2CMemory.xml \
    I2CMemory.hex
