#-------------------------------------------------
#
# Project created by QtCreator 2017-02-09T13:21:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lcd22
TEMPLATE = app

# Specifies include paths that are passed to the Windows Resource Compiler.
INCLUDEPATH += ../libraries/arduino-ds1302-master \
  ../libraries/DHT-sensor-library-master \
  ../libraries/ili9341-arduino-master \
  ../libraries/IrControl \
  ../libraries/LiquidCrystal_I2C \
  ../libraries/MsTimer2 \
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
