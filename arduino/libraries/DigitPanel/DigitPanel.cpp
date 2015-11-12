/**
 * Подключаем и работаем с панелью на 4 светодиодных цифры.
 */
#include <DigitPanel.h>

#define DIGIT_ON  HIGH
#define DIGIT_OFF LOW

#define MINUS_MASK 8

const byte DP_SegmentMap[16] = {
//   -0-        -1-        -2-        -3-        -4-        -5-        -6-        -7-    
  B01110111, B01100000, B00111110, B01111100, B01101001, B01011101, B01011111, B01100100,
//   -8-        -9-        -A-        -B-        -C-        -D-        -E-        -F-
  B01111111, B01111101, B01101111, B01011011, B00010111, B01111010, B00011111, B00001111
};

DigitPanel* DigitPanel::_activeDigitPanelObject = 0;

/**
 * Конструктор с инициализатором
 */
DigitPanel::DigitPanel(int _latchPin, int _clockPin, int _dataPin, int* _digits, int _count) {

  latchPin = _latchPin;
  clockPin = _clockPin;
  dataPin = _dataPin;
  for (int i = 0; i < 4 && i < _count; i++) {
    digit[i] = _digits[i];
  }

  //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (int i = 0; i < 4; i++) { 
    pinMode(digit[i], OUTPUT);
  }
  
  DigitPanel::_activeDigitPanelObject = this;
  MsTimer2::set(3, DigitPanel::handle_interrupt);
  MsTimer2::start();
}

void DigitPanel::handle_interrupt() {
  if (_activeDigitPanelObject) {
	_activeDigitPanelObject->showSegment();
  }
}

void DigitPanel::setValue(String value) {
  panelValue = value;
}

String DigitPanel::getValue() {
  return panelValue;
}

/** показываем очередной сегмент. */
void DigitPanel::showSegment() {
  static int place = 0;
  static unsigned int chars = 0;
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
}

/** Сбрасываем все цифры. */
void DigitPanel::resetAllDigit() {
  for (int digInd = 0; digInd < 4; digInd++) {
    int dig = digit[digInd];
    digitalWrite(dig, DIGIT_OFF);
  }
}

/** Находим маску по номеру знака(цифры) и выводим маску в сегменты */ 
void DigitPanel::setChar(char chr, boolean isPnt) {
  int bits;
  if (chr == '-') {
    bits = MINUS_MASK;
  } else if (chr >= '0' && chr <= '9') {
    bits = DP_SegmentMap[chr - '0'];
  } else if (chr >= 'a' && chr <= 'f') {
    bits = DP_SegmentMap[chr - 'a' + 10];
  } else if (chr >= 'A' && chr <= 'F') {
    bits = DP_SegmentMap[chr - 'A' + 10];
  } else {
    bits = 127;
  }
  if (isPnt) {
    bits |= 128;
  }
  setNumBit(bits);
}

/** Выводит маску символа в сегменты */
void DigitPanel::setNumBit(uint8_t mask) {
  // устанавливаем синхронизацию "защелки" на LOW
  digitalWrite(latchPin, LOW);
  // передаем последовательно на dataPin
  shiftOut(dataPin, clockPin, MSBFIRST, mask ^ 0xFF);
  //"защелкиваем" регистр, тем самым устанавливая значения на выходах
  digitalWrite(latchPin, HIGH);
}

