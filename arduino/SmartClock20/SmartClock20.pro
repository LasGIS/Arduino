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
    ../libraries/DHT-sensor-library-master/DHT.cpp \
    ../libraries/IrControl/IrControl.cpp \
    ../libraries/MsTimer2/MsTimer2.cpp \
    ../libraries/DS3231/DS3231.cpp \
    SmartClock20.ino \
    Accel.cpp \
    RealTime.cpp \
    buzzer.cpp \
    note.cpp

# Specifies include paths that are passed to the Windows Resource Compiler.
INCLUDEPATH += ../libraries/arduino-ds1302-master \
    ../libraries/ADXL345/src \
    ../libraries/DS3231 \
    ../libraries/TFT_LG_ILI9225/src \
    ../libraries/DHT-sensor-library-master \
    ../libraries/IrControl \
    ../libraries/MsTimer2 \
    C:/local/Arduino/hardware/arduino/avr/cores/arduino \
    C:/local/Arduino/hardware/tools/avr/avr/include \
    C:/local/Arduino/hardware/arduino/avr/variants/standard \
    C:/local/Arduino/hardware/arduino/avr/libraries/EEPROM/src \
    C:/local/Arduino/hardware/arduino/avr/libraries/HID/src \
    C:/local/Arduino/hardware/arduino/avr/libraries/SoftwareSerial/src \
    C:/local/Arduino/hardware/arduino/avr/libraries/SPI/src \
    C:/local/Arduino/hardware/arduino/avr/libraries/Wire/src \
    C:/local/Arduino/hardware/arduino/avr/libraries/Wire/src/utility \
    C:/local/Arduino/libraries/SD/src \
    C:/local/Arduino/libraries/Servo/src

HEADERS += \
    ../libraries/DHT-sensor-library-master/DHT.h \
    ../libraries/IrControl/IrControl.h \
    ../libraries/MsTimer2/MsTimer2.h \
    ../libraries/ADXL345/src/ADXL345.h \
    ../libraries/DS3231/DS3231.h \
    ../libraries/TFT_LG_ILI9225/src/TFT_LG_ILI9225.h \
    ../libraries/DS3231/Examples/DS3231_TEST/DS3231_TEST.ino \
    ../libraries/DS3231/DS3231.h \
    SmartClock20.h \
    pitches.h \
    note.h

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
    ../libraries/DS3231/Readme.txt
