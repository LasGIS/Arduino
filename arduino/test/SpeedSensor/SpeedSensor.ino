/*
 * счётчик скорости
 */

// пин для аналогового счётчика скорости
const int analogSpeedPin = A0;

// пин для цифрового счётчика скорости
const int speedCountPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(speedCountPin, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  int anal = analogRead(analogSpeedPin);
  int digit = digitalRead(speedCountPin);
  Serial.print("digit = ");
  Serial.print(digit, BIN);
  Serial.print("; anal = ");
  Serial.println(anal, DEC);
  delay(1000);
}

