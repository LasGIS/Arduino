#define DIGIT_ON  HIGH
#define DIGIT_OFF LOW

#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH
#define MINUS_MASK 8

int digit[4] = {13,12,11,10};
int segment[8] = {2,3,4,5,6,7,8,9};
//                    -0-  -1-  -2-  -3-  -4-  -5-  -6-  -7-  -8-  -9-  -A-  -B-  -C-  -D-  -E-  -F-  
int segmentMap[16] = {119,  96,  62, 124, 105,  93,  95, 100, 127, 125, 111,  91,  23, 122,  31,  15};

void initPanel() {
    for (int i = 0; i < 4; i++) { 
    pinMode(digit[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++) { 
    pinMode(segment[i], OUTPUT);
  }
}

/** показываем очередной сегмент. */
void showPanel() {
  static int place = 0;
  static int chars = 0;
  resetAllDigit();
  if (chars < panelValue.length()) {
    char chr = panelValue[chars];
    if (chars + 1 < panelValue.length() && panelValue[chars + 1] == '.') {
      setChar(chr, true);
      chars++;
    } else {
      setChar(chr, false);
    }
    digitalWrite(digit[place], DIGIT_ON);
  }
  place++;
  chars++;
  if (!(place < 4 && chars < panelValue.length())) {
    place = 0;
    chars = 0;
  }
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
  for (int i = 0; i < 8; i++) {
    digitalWrite(segment[i], (mask & 1) ? SEGMENT_ON : SEGMENT_OFF);
    mask = mask >> 1;
  }
}

