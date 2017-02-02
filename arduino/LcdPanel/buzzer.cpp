#include "LcdPanel.h"
#include "pitches.h"

extern int buzzerPin;
uint8_t buzzerfactor = 1;

/**
 * пищит <del> секунд с частотой <hertz> герц  
 */
void buzzerOut(unsigned int hertz, unsigned long del) {
//  tone(buzzerPin, hertz, del);
  if (buzzerfactor == 0) return;
  noInterrupts();
  long delayVal = (500000 / hertz) - 9;
  long delayHigh = 0x1 << (buzzerfactor - 1);
  long delayLow = (delayVal) - delayHigh;
  int noteCount = (del * 500) / delayVal;
#ifdef HAS_SERIAL
  Serial.print(delayVal);
  Serial.print(", (");
  Serial.print(delayHigh);
  Serial.print(", ");
  Serial.print(delayLow);
  Serial.print("), ");
  Serial.print(noteCount);
  Serial.println(";");
#endif
  for (int i = 0; i < noteCount; i++) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(delayHigh);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(delayLow);
  }
  interrupts();
}
