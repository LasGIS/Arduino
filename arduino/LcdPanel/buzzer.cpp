#include <Arduino.h>
#include "pitches.h"

extern int buzzerPin;

/**
 * пищит <del> секунд с частотой <hertz> герц  
 */
void buzzerOut(unsigned int hertz, unsigned long del) {
//  tone(buzzerPin, hertz, del);
  noInterrupts();
  long delayVal = (500000 / hertz) - 9;
  int count = (del * hertz) / 1000;
  for (int i = 0; i < count; i++) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(delayVal);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(delayVal);
  }
  interrupts();
}

/**
 * для проверки и валидации параметров 
void buzzerOutTest(unsigned int hertz, unsigned long del) {
  long msecs = millis();
  //noInterrupts();
  tone(buzzerPin, hertz, del);
  delay(del + 100);
  buzzerOut(hertz, del);
  //interrupts();
  msecs = millis() - msecs;
  Serial.println(msecs, DEC);
  lcd.print(msecs);
  delay(1000);
}
*/

