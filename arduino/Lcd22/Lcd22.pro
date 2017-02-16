#-------------------------------------------------
#
# Project created by QtCreator 2017-02-09T13:21:05
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lcd22
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
  ../libraries/ili9341-arduino-master \
  ../libraries/IrControl \
  ../libraries/LiquidCrystal_I2C \
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
    ../libraries/LiquidCrystal_I2C/LiquidCrystal_I2C.cpp \
    ../libraries/MsTimer2/MsTimer2.cpp \
    ../libraries/ili9341-arduino-master/font.c \
    ../libraries/ili9341-arduino-master/font.c \
    ../libraries/ili9341-arduino-master/TFTv2.cpp \
    Lcd22.ino

HEADERS += \
    ../libraries/arduino-ds1302-master/DS1302.h \
    ../libraries/DHT-sensor-library-master/DHT.h \
    ../libraries/IrControl/IrControl.h \
    ../libraries/LiquidCrystal_I2C/LiquidCrystal_I2C.h \
    ../libraries/MsTimer2/MsTimer2.h \
    ../libraries/ili9341-arduino-master/TFTv2.h \
    Lcd22.h

#FORMS    += mainwindow.ui
DISTFILES += \
    ../libraries/ili9341-arduino-master/README.md \
    ../libraries/ili9341-arduino-master/keywords.txt
