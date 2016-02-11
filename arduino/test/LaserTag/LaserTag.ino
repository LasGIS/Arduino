/**
 * проверка лазера
*/
#define pinL1 3
#define pinL2 5

void setup() {
  //Serial.begin(9600);
  //pinMode(pinL1, OUTPUT);
  //pinMode(pinL2, OUTPUT);
}

void loop() {
  static int i = 0;
  int l1 = (sin((i * PI) / 18) + 1) * 127;
  int l2 = (cos((i * PI) / 18) + 1) * 127;
  analogWrite(pinL1, l1);
  analogWrite(pinL2, l2);
  delay(100);
/*
  digitalWrite(pinL1, HIGH);
  delay(100);
  digitalWrite(pinL1, LOW);
  delay(200);
  digitalWrite(pinL2, HIGH);
  delay(100);
  digitalWrite(pinL2, LOW);
  delay(600);
*/
  i++;
}

