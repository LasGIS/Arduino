
#define DIGIT_ON  HIGH
#define DIGIT_OFF LOW

#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH
/*
#define DIGIT_ON  LOW
#define DIGIT_OFF HIGH

#define SEGMENT_ON  HIGH
#define SEGMENT_OFF LOW
*/
int digit[4] = {10,11,12,13};
int segment[8] = {2,3,4,5,6,7,8,9};
//                    -0-  -1-  -2-  -3-  -4-  -5-  -6-  -7-  -8-  -9-  -A-  -B-  -C-  -D-  -E-  -F-  
int segmentMap[16] = {119,  96,  62, 124, 105,  93,  95, 100, 127, 125, 111,  91,  23, 122,  31,  15};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) { 
    pinMode(digit[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++) { 
    pinMode(segment[i], OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
// viewAllSignParallel();
// viewAllHexable();
  viewAllDecable();
  //pin13();
}

void viewAllHexable() {
  for (int i = 0; i < 32/*767*/; i++) {
    Serial.println(i, HEX);
    for (int tim = 0; tim < 50; tim++) {
      viewHexable(i);
    }
    //delay(500);
  }
}

void viewAllDecable() {//Вывод всей панели
  int count = 1;
  for (int i = 0; i < 10000; i += count) {//кол-во секунд
   Serial.println(i, DEC);
    for (int tim = 0; tim < 50; tim++) { //1 цифра в 1 секунду
      viewDecimal(i);
    }
  }
}

void viewAllSignParallel() {
  for (int digInd = 0; digInd < 4; digInd++) {
    int dig = digit[digInd];
    resetAllDigit();
    digitalWrite(dig, DIGIT_ON);
    Serial.print("dig[");
    Serial.print(digInd);
    Serial.print("] = ");
    for (int i = 0; i < 10; i++) {
      setNumber(i);
      Serial.print(i, DEC) ;
      delay(500);
    }
    Serial.println(";");
  }
}

void viewHexable(int hex) {
  for (int i = 0; i < 4; i++) {
    resetAllDigit();
    int dig = digit[i];
    int tetrad = hex & 15;
    Serial.print(tetrad, HEX);
    setNumber(tetrad);
    hex = hex >> 4;
    digitalWrite(dig, DIGIT_ON);
    delay(5);
  }
}

/** Выводим на панель 4 цифры, которые показывают число dec  */ 
void viewDecimal(int dec) {
  int dinum;
  int i = 0;
  while ((dec > 0 && i < 4) || i == 0) { 
    resetAllDigit();
    dinum = dec % 10;
    setNumber(dinum);
    dec = dec / 10;
    digitalWrite(digit[i], DIGIT_ON);
    i++;
    delay(5);
  }
  delay(5 * (4 - i));
}

/** Сбрасываем все цифры. */
void resetAllDigit() {
  for (int digInd = 0; digInd < 4; digInd++) {
    int dig = digit[digInd];
    digitalWrite(dig, DIGIT_OFF);
  }
}

/** Находим маску по номеру знака(цифры) и выводим маску в сегменты */ 
void setNumber(int num) {
    setNumBit(segmentMap[num & 15]);
}

/** Выводит маску символа в сегменты */
void setNumBit(int mask) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(segment[i], (mask & 1) ? SEGMENT_ON : SEGMENT_OFF);
    mask = mask >> 1;    
  }  
}


void pin13() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(20);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
}

