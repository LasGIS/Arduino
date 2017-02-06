#include "LcdPanel.h"
#include "pitches.h"

uint8_t keySoundVolume = 1;
uint8_t musicSoundVolume = 1;

int music[] = {
  NOTE_F4, 1, NOTE_E4, 1, NOTE_D4, 1, NOTE_C4, 1, NOTE_G4, 3, NOTE_G4, 3,
  NOTE_F4, 1, NOTE_E4, 1, NOTE_D4, 1, NOTE_C4, 1, NOTE_G4, 3, NOTE_G4, 3,
  NOTE_F4, 1, NOTE_A4, 1, NOTE_A4, 1, NOTE_F4, 1,
  NOTE_E4, 1, NOTE_G4, 1, NOTE_G4, 1, NOTE_E4, 1,
  NOTE_D4, 1, NOTE_E4, 1, NOTE_F4, 1, NOTE_D4, 1, NOTE_C4, 3, NOTE_C4, 3,
  NOTE_F4, 1, NOTE_A4, 1, NOTE_A4, 1, NOTE_F4, 1,
  NOTE_E4, 1, NOTE_G4, 1, NOTE_G4, 1, NOTE_E4, 1,
  NOTE_D4, 1, NOTE_E4, 1, NOTE_F4, 1, NOTE_D4, 1, NOTE_C4, 3, NOTE_C4, 3
};

/**
 *  musicAlarm
 */
void musicAlarm() {
  for (int i = 0; i < sizeof(music) / sizeof(int); i+=2) {
    buzzerOut(music[i], 300 * music[i + 1], musicSoundVolume);
    delay(100);
  }
}

/**
 * пищит <del> милисекунд с частотой <hertz> герц
 */
void buzzerOut(unsigned int hertz, unsigned long del, uint8_t soundVolume) {
  if (soundVolume == 0) return;
  pinMode(BUZZER_PIN, OUTPUT);
  noInterrupts();
  long delayVal = (500000 / hertz) - 9;
  long delayHigh = 0x1 << (soundVolume - 1);
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
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(delayHigh);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(delayLow);
  }
  interrupts();
  pinMode(BUZZER_PIN, INPUT);
}
