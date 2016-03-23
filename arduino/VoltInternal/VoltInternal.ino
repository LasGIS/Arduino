static double rate = 3500. * 1.0847 / (500. * 1023.);

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
}

void loop() {
/*
  int val = analogRead(A0);
  Serial.print("analogRead() = ");
  Serial.print(val);
  Serial.print("; V = ");
  Serial.println(val * rate);
  delay(1000);
*/
  delay(10);
}

void serialEvent() {
  char buf[10];
  while (Serial.available() > 0) {
    int cnt = Serial.readBytes(buf, 10);
    for (int i = 0; i < cnt; i++) {
      Serial.write(buf[i]);
    }
    //Serial.println(".");
  }
  delay(10);
}
