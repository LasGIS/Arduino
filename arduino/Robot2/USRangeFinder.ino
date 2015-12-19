/** получить дистанцию.*/
float getDistance() { 
  int duration;
  float cm;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH); 
  cm = duration / 58.;
  return cm;
}

void showDistance(int degre) {
  Serial.print(degre);
  Serial.print("C ");
  lcd.setCursor(0, 1);
  lcd.print(degre);
  lcd.print("C ");
  float cm[5];
  for (int i = 0; i < 5; i++) {
    cm[i] = getDistance();
    delay(10);
    Serial.print(cm[i]);
    Serial.print("cm  ");
  }
  Serial.println();
  float sum = 0.0;
  for (int i = 0; i < 5; i++) {
    sum += cm[i];
  }
  lcd.print(sum / 5);
  lcd.print(" cm ");
}

