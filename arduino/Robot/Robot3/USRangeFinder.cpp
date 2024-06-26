#include "Robot.h"

#define WAIT_TIME_DISTANCE 70

extern int echoPin;
extern int trigPin;

/** получить дистанцию в point`ax (условная единица 1[point]*58.0 = 1[см] .*/
int getDistance() {
  delay(WAIT_TIME_DISTANCE);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH);
}

float showDistance(int degre) {
  Serial.print(degre);
  Serial.print("C ");
  float cm = getDistance() / 58.;
  Serial.print(cm);
  Serial.println("cm");
  return cm;
}
