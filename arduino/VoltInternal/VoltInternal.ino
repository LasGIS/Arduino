static double rate = 3500. * 1.0847 / (500. * 1023.);

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
}

void loop() {
  int val = analogRead(A0);
  Serial.print("analogRead() = ");
  Serial.print(val);
  Serial.print("; V = ");
  Serial.println(val * rate);
  delay(1000);
}
