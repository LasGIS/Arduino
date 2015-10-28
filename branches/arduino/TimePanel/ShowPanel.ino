#include <MsTimer2.h>

#define DIGIT_ON  HIGH
#define DIGIT_OFF LOW

#define MINUS_MASK 8

//Пин подключен к ST_CP входу 74HC595
const int latchPin = 8;
//Пин подключен к SH_CP входу 74HC595
const int clockPin = 12;
//Пин подключен к DS входу 74HC595
const int dataPin = 11;

const int digit[4] = {10,9,7,6};

const byte segmentMap[16] = {
//   -0-        -1-        -2-        -3-        -4-        -5-        -6-        -7-    
  B01110111, B01100000, B00111110, B01111100, B01101001, B01011101, B01011111, B01100100,
//   -8-        -9-        -A-        -B-        -C-        -D-        -E-        -F-
  B01111111, B01111101, B01101111, B01011011, B00010111, B01111010, B00011111, B00001111
};

void initPanel() {
  //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < 4; i++) { 
    pinMode(digit[i], OUTPUT);
  }
  MsTimer2::set(5, showPanel);
  MsTimer2::start();
}

/** показываем очередной сегмент. */
void showPanel() {
  noInterrupts();
  static int place = 0;
  static int chars = 0;
  static String panelValueLocal = panelValue;
  resetAllDigit();
  if (chars < panelValueLocal.length()) {
    char chr = panelValueLocal[chars];
    if (chars + 1 < panelValueLocal.length() && panelValueLocal[chars + 1] == '.') {
      setChar(chr, true);
      chars++;
    } else {
      setChar(chr, false);
    }
    digitalWrite(digit[place], DIGIT_ON);
  }
  place++;
  chars++;
  if (!(place < 4 && chars < panelValueLocal.length())) {
    place = 0;
    chars = 0;
    panelValueLocal = panelValue;
  }
  interrupts();
}

/** Сбрасываем все цифры. */
void resetAllDigit() {
  for (int digInd = 0; digInd < 4; digInd++) {
    int dig = digit[digInd];
    digitalWrite(dig, DIGIT_OFF);
  }
}

/** Находим маску по номеру знака(цифры) и выводим маску в сегменты */ 
void setChar(char chr, boolean isPnt) {
  int bits;
  if (chr == '-') {
    bits = MINUS_MASK;
  } else if (chr >= '0' && chr <= '9') {
    bits = segmentMap[chr - '0'];
  } else if (chr >= 'a' && chr <= 'f') {
    bits = segmentMap[chr - 'a' + 10];
  } else if (chr >= 'A' && chr <= 'F') {
    bits = segmentMap[chr - 'A' + 10];
  } else {
    bits = 127;
  }
  if (isPnt) {
    bits |= 128;
  }
  setNumBit(bits);
}

/** Выводит маску символа в сегменты */
void setNumBit(int mask) {
  // устанавливаем синхронизацию "защелки" на LOW
  digitalWrite(latchPin, LOW);
  // передаем последовательно на dataPin
  shiftOut(dataPin, clockPin, MSBFIRST, mask ^ 0xFF);
  //"защелкиваем" регистр, тем самым устанавливая значения на выходах
  digitalWrite(latchPin, HIGH);
}

