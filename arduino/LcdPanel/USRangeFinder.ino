/** получить дистанцию.
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

void showDistance() {
  if (count % 10 == 0) {
    float cm = getDistance();
    Serial.print(cm);
    Serial.println(" cm");
    lcd.setCursor(11, 0);
    lcd.print(cm);
    lcd.print(" cm ");
  }
}
*/
