/**
 * проверка лазера
*/
#define pinL1 3
#define pinIR A0

void setup() {
  Serial.begin(9600);
  pinMode(pinL1, OUTPUT);
  pinMode(pinIR, INPUT);
}

void loop() {
  int L1 = digitalRead(pinIR);
  digitalWrite(pinL1, L1);
  Serial.println(L1);
  delay(100);
}

