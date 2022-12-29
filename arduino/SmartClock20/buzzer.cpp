#include "SmartClock20.h"
#include "pitches.h"
#include "note.h"

uint8_t keySoundVolume = 1;
uint8_t musicSoundVolume = 8;

/**
 Jingle Bells (������ ����)
 https://musicnotes.info/jingle-bells-dzingl-bels-noty-dla-fortepiano
 */
Note music[] = {
  Note(Mi, 4, 2), Note(Mi, 4, 2), Note(Mi, 4, 1),
  Note(Mi, 4, 2), Note(Mi, 4, 2), Note(Mi, 4, 1),
  Note(Mi, 4, 2), Note(Sl, 4, 2), Note(Do, 4, 2), Note(Re, 4, 2),
  Note(Mi, 4, 0),
  Note(Fa, 4, 2), Note(Fa, 4, 2), Note(Fa, 4, 1),
  Note(Mi, 4, 2), Note(Mi, 4, 2), Note(Mi, 4, 1),
  Note(Mi, 4, 2), Note(Re, 4, 2), Note(Re, 4, 2), Note(Mi, 4, 2),
  Note(Re, 4, 1), Note(Sl, 4, 1),
  Note(Mi, 4, 2), Note(Mi, 4, 2), Note(Mi, 4, 1),
  Note(Mi, 4, 2), Note(Mi, 4, 2), Note(Mi, 4, 1),

  Note(Mi, 4, 2), Note(Sl, 4, 2), Note(Do, 4, 2), Note(Re, 4, 2),
  Note(Mi, 4, 0),
  Note(Fa, 4, 2), Note(Fa, 4, 2), Note(Fa, 4, 1),
  Note(Mi, 4, 2), Note(Mi, 4, 2), Note(Mi, 4, 1),
  Note(Sl, 4, 2), Note(Sl, 4, 2), Note(Fa, 4, 2), Note(Re, 4, 2),
  Note(Do, 4, 0),
  Note(Mi, 3, 2), Note(Mi, 4, 2), Note(Re, 4, 2), Note(Do, 4, 2),
  Note(Mi, 3, 0), Note(Mi, 3, 2), Note(Mi, 4, 2),
  Note(Re, 4, 2), Note(Do, 4, 2), Note(Fa, 3, 0),
  Note(Fa, 3, 2), Note(Fa, 4, 2), Note(Mi, 4, 2), Note(Re, 4, 2),

  Note(Sl, 3, 1), Note(Sl, 4, 2), Note(Sl, 4, 2),
  Note(Fa, 4, 2), Note(Re, 4, 2), Note(Mi, 4, 0),
  Note(Mi, 3, 2), Note(Mi, 4, 2), Note(Re, 4, 2), Note(Do, 4, 2),
  Note(Mi, 3, 0), Note(Mi, 3, 2), Note(Mi, 4, 2),
  Note(Re, 4, 2), Note(Do, 4, 2), Note(Fa, 3, 0),
  Note(Fa, 3, 2), Note(Fa, 4, 2), Note(Mi, 4, 2), Note(Re, 4, 2),
  Note(Sl, 4, 2), Note(Sl, 4, 2), Note(Sl, 4, 1),
  Note(La, 4, 2), Note(Sl, 4, 2), Note(Fa, 4, 2), Note(Re, 4, 2),
  Note(Do, 4, 0),
  Note(Mi, 4, 2), Note(Mi, 4, 2), Note(Mi, 4, 1),

  Note(Mi, 4, 2), Note(Mi, 4, 2), Note(Mi, 4, 1),
  Note(Mi, 4, 2), Note(Sl, 4, 2), Note(Do, 4, 2), Note(Re, 4, 2),
  Note(Mi, 4, 0),
  Note(Fa, 4, 2), Note(Fa, 4, 2), Note(Fa, 4, 1),
  Note(Mi, 4, 2), Note(Mi, 4, 2), Note(Mi, 4, 1),
  Note(Sl, 4, 2), Note(Sl, 4, 2), Note(Fa, 4, 2), Note(Re, 4, 2),
  Note(Do, 4, 0),
};

/* ���� � ������ 2 ���� */
/*
Note music[] = {
  Note(Fa, 4, 2), Note(Mi, 4, 2), Note(Re, 4, 2), Note(Do, 4, 2), Note(Sl, 4, 1),  Note(Sl, 4, 1),
  Note(Fa, 4, 2), Note(Mi, 4, 2), Note(Re, 4, 2), Note(Do, 4, 2), Note(Sl, 4, 1),  Note(Sl, 4, 1),
  Note(Fa, 4, 2), Note(La, 4, 2), Note(La, 4, 2), Note(Fa, 4, 2),
  Note(Mi, 4, 2), Note(Sl, 4, 2), Note(Sl, 4, 2), Note(Mi, 4, 2),
  Note(Re, 4, 2), Note(Mi, 4, 2), Note(Fa, 4, 2), Note(Re, 4, 2), Note(Do, 4, 1),  Note(Do, 4, 1),
  Note(Fa, 4, 2), Note(La, 4, 2), Note(La, 4, 2), Note(Fa, 4, 2),
  Note(Mi, 4, 2), Note(Sl, 4, 2), Note(Sl, 4, 2), Note(Mi, 4, 2),
  Note(Re, 4, 2), Note(Mi, 4, 2), Note(Fa, 4, 2), Note(Re, 4, 2), Note(Do, 4, 1),  Note(Do, 4, 1)
};
*/

/**
 *  musicAlarm
 */
void musicAlarm() {
#ifdef HAS_SERIAL
  Serial.print("musicSize = ");
  Serial.print(sizeof(music) / sizeof(Note));
#endif
  for (uint16_t i = 0; i < (sizeof(music) / sizeof(Note)); i++) {
    Note note = music[i];
#ifdef HAS_SERIAL
    Serial.print("note.tone() = ");
    Serial.print(note.tone());
    Serial.print("; note.delay() = ");
    Serial.println(note.delay());
#endif
    buzzerOut(note.tone(), note.delay(), musicSoundVolume);
    delay(30);
  }
}

/**
 * ����� <del> ���������� � �������� <hertz> ����
 */
void buzzerOut(uint16_t hertz, uint32_t del, uint8_t soundVolume) {
  if (soundVolume == 0) return;
  pinMode(BUZZER_PIN, OUTPUT);
  noInterrupts();
  uint32_t delayVal = (500000 / hertz) - 9;
  uint32_t delayHigh = 0x1 << (soundVolume - 1);
  uint32_t delayLow = (delayVal) - delayHigh;
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
