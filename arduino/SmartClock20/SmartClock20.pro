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
    ../libraries/TFT_22_ILI9225-master/src/TFT_22_ILI9225.cpp \
    ../libraries/TFT_22_ILI9225-master/src/DefaultFonts.c \
    ../libraries/DHT-sensor-library-master/DHT.cpp \
    ../libraries/IrControl/IrControl.cpp \
    ../libraries/MsTimer2/MsTimer2.cpp \
    ../libraries/DS3231/DS3231.cpp \
    SmartClock20.ino \
    Accel.cpp \
    RealTime.cpp

# Specifies include paths that are passed to the Windows Resource Compiler.
INCLUDEPATH += ../libraries/arduino-ds1302-master \
    ../libraries/ADXL345/src \
    ../libraries/DS3231 \
    ../libraries/TFT_22_ILI9225-master/src \
    ../libraries/DHT-sensor-library-master \
    ../libraries/IrControl \
    ../libraries/MsTimer2 \
    C:/local/Arduino/hardware/arduino/avr/cores/arduino \
    C:/local/Arduino/hardware/tools/avr/avr/include \
    C:/local/Arduino/hardware/arduino/avr/variants/standard \
    C:/local/Arduino/hardware/arduino/avr/libraries/EEPROM \
    C:/local/Arduino/hardware/arduino/avr/libraries/HID \
    C:/local/Arduino/hardware/arduino/avr/libraries/SoftwareSerial \
    C:/local/Arduino/hardware/arduino/avr/libraries/SPI \
    C:/local/Arduino/hardware/arduino/avr/libraries/Wire \
    C:/local/Arduino/hardware/arduino/avr/libraries/Wire/utility \
    C:/local/Arduino/libraries/SD/src \
    C:/local/Arduino/libraries/Servo/src

HEADERS += \
    ../libraries/DHT-sensor-library-master/DHT.h \
    ../libraries/IrControl/IrControl.h \
    ../libraries/MsTimer2/MsTimer2.h \
    ../libraries/ADXL345/src/ADXL345.h \
    ../libraries/DS3231/DS3231.h \
    ../libraries/TFT_22_ILI9225-master/src/TFT_22_ILI9225.h \
    ../libraries/DS3231/Examples/DS3231_TEST/DS3231_TEST.ino \
    ../libraries/DS3231/DS3231.h \
    SmartClock20.h

DISTFILES += \
    ../libraries/ADXL345/library.json \
    ../libraries/TFT_22_ILI9225-master/library.json \
    ../libraries/TFT_22_ILI9225-master/images/ILI9225_TFT_back.jpg \
    ../libraries/TFT_22_ILI9225-master/images/ILI9225_TFT_dimensions.jpg \
    ../libraries/TFT_22_ILI9225-master/images/ILI9225_TFT_front.jpg \
    ../libraries/TFT_22_ILI9225-master/images/color.png \
    ../libraries/TFT_22_ILI9225-master/images/tux.png \
    ../libraries/ADXL345/lib/readme.txt \
    ../libraries/ADXL345/keywords.txt \
    ../libraries/ADXL345/library.properties \
    ../libraries/ADXL345/License.txt \
    ../libraries/ADXL345/platformio.ini \
    ../libraries/TFT_22_ILI9225-master/keywords.txt \
    ../libraries/TFT_22_ILI9225-master/library.properties \
    ../libraries/ADXL345/README.md \
    ../libraries/TFT_22_ILI9225-master/README.md \
    ../libraries/DS3231/keywords.txt \
    ../libraries/DS3231/Readme.txt
