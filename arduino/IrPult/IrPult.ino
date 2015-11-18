#include <IRremote.h>
#include <IRremoteInt.h>
/*
//Пин подключен к ST_CP входу 74HC595
int latchPin = 8;
//Пин подключен к SH_CP входу 74HC595
int clockPin = 12;
//Пин подключен к DS входу 74HC595
int dataPin = 11;

int digit[4] = {10,9,7,6};
#define DIGIT_ON  HIGH
#define DIGIT_OFF LOW
*/
IRrecv irrecv(2); // указываем вывод, к которому подключен приемник
decode_results results;

void setup() {
  Serial.begin(9600); // выставляем скорость COM порта
/*
  //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < 4; i++) { 
    pinMode(digit[i], OUTPUT);
  }
*/
  irrecv.enableIRIn(); // запускаем прием
}

void loop() {
/*  for (int place = 0; place < 4; place++) {
    resetAllDigit();
    digitalWrite(digit[place], DIGIT_ON);
    // отсчитываем от 0 до 255  и отображаем значение на светодиоде
    for (int numberToDisplay = 0; numberToDisplay < 25; numberToDisplay++) {
      // устанавливаем синхронизацию "защелки" на LOW
      digitalWrite(latchPin, LOW);
      // передаем последовательно на dataPin
      shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay); 
   
      //"защелкиваем" регистр, тем самым устанавливая значения на выходах
      digitalWrite(latchPin, HIGH);
*/
      if (irrecv.decode(&results)) { // если данные пришли
        Serial.println(results.value, HEX); // печатаем данные
        irrecv.resume(); // принимаем следующую команду
      }
      // пауза перед следующей итерацией
      delay(100);
/*      
    }
    
  } */
}

/** Сбрасываем все цифры. * /
void resetAllDigit() {
  for (int digInd = 0; digInd < 4; digInd++) {
    int dig = digit[digInd];
    digitalWrite(dig, DIGIT_OFF);
  }
}
*/
