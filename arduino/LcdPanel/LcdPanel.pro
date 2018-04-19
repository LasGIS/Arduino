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
    ../libraries/arduino-ds1302-master/DS1302.cpp \
    ../libraries/DHT-sensor-library-master/DHT.cpp \
    ../libraries/IrControl/IrControl.cpp \
    ../libraries/LiquidCrystal_I2C/LiquidCrystal_I2C.cpp \
    ../libraries/MsTimer2/MsTimer2.cpp \
    ../libraries/ili9341-arduino-master/font.c \
    LcdPanel.ino \
    alarm_clock.cpp \
    buzzer.cpp \
    CustomChars.cpp \
    lcd_screen.cpp \
    main_screen.cpp \
    set_screen.cpp \
    show_char_screen.cpp \
    note.cpp

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

HEADERS += \
    ../libraries/arduino-ds1302-master/DS1302.h \
    ../libraries/DHT-sensor-library-master/DHT.h \
    ../libraries/IrControl/IrControl.h \
    ../libraries/LiquidCrystal_I2C/LiquidCrystal_I2C.h \
    ../libraries/MsTimer2/MsTimer2.h \
    LcdPanel.h \
    alarm_clock.h \
    lcd_screen.h \
    main_screen.h \
    pitches.h \
    set_screen.h \
    show_char_screen.h \
    note.h

DISTFILES += \
    LcdPanel.fzz
