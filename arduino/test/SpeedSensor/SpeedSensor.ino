/*
 * счётчик скорости
 */

// пин для аналогового счётчика скорости
const int analogSpeedPin = A0;

// пин для цифрового счётчика скорости
const int speedometrPin = 2;

static int count = 0;

void setup() {
  Serial.begin(9600);
  pinMode(speedometrPin, INPUT);
  //прерывание спидометра по фронту импульса
  attachInterrupt(
    digitalPinToInterrupt(speedometrPin),
    speedometr, CHANGE
  );
}

// the loop function runs over and over again forever
void loop() {
  Serial.print("count = ");
  Serial.println(count, DEC);
  delay(1000);
}

/** 
 * измеряем частоту на входе спидометра по прерыванию
 */
void speedometr() {
  static int lastVal = 0;
  int val = digitalRead(speedometrPin);
  if (val != lastVal) {
    count++;
    lastVal = val;
  }
/*  
  int anal = analogRead(analogSpeedPin);
  int digit = digitalRead(speedometrPin);
  Serial.print("digit = ");
  Serial.print(digit, BIN);
  Serial.print("; anal = ");
  Serial.println(anal, DEC);
*/  
  delay(10);
}


