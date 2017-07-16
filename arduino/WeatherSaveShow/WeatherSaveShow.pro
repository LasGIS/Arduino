#-------------------------------------------------
#
# Project created by QtCreator 2017-02-09T13:21:05
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeatherSaveShow
TEMPLATE = app

#// THE_ANSWER 42
DEFINES += ARDUINO=10612
DEFINES += F_CPU 16000000L
DEFINES += ARDUINO_AVR_PRO
DEFINES += ARDUINO_ARCH_AVR
DEFINES += __AVR__
DEFINES += __AVR_ATmega328P__

# Specifies include paths that are passed to the Windows Resource Compiler.
INCLUDEPATH += ../libraries/arduino-ds1302-master \
  ../libraries/DHT-sensor-library-master \
  ../libraries/TFT_lg/src \
  ../libraries/TFT_lg/src/utility \
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

SOURCES += \
    ../libraries/arduino-ds1302-master/DS1302.cpp \
    ../libraries/DHT-sensor-library-master/DHT.cpp \
    ../libraries/IrControl/IrControl.cpp \
    ../libraries/MsTimer2/MsTimer2.cpp \
    ../libraries/TFT_lg/src/TFT_lg.cpp \
    ../libraries/TFT_lg/src/utility/Adafruit_GFX_lg.cpp \
    ../libraries/TFT_lg/src/utility/Adafruit_ST7735_lg.cpp \
    ../libraries/TFT_lg/src/utility/glcdfont.c \
    ../libraries/TFT_lg/src/utility/russfont1.c \
    ../libraries/TFT_lg/src/utility/RussFontANSI.c \
    ../libraries/TFT_lg/src/utility/RussFontOEM.c \
    WeatherSaveShow.ino \
    RealTime.cpp

HEADERS += \
    ../libraries/arduino-ds1302-master/DS1302.h \
    ../libraries/DHT-sensor-library-master/DHT.h \
    ../libraries/IrControl/IrControl.h \
    ../libraries/MsTimer2/MsTimer2.h \
    ../libraries/TFT_lg/src/TFT_lg.h \
    ../libraries/TFT_lg/src/utility/Adafruit_GFX_lg.h \
    ../libraries/TFT_lg/src/utility/Adafruit_ST7735_lg.h \
    ../libraries/TFT_lg/src/utility/PImage_lg.h \
    WeatherSaveShow.h

#FORMS    += mainwindow.ui
DISTFILES += \
    ../libraries/TFT_lg/README.md \
    ../libraries/TFT_lg/keywords.txt \
    ../libraries/TFT_lg/src/utility/keywords.txt
