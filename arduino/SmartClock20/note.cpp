#include "pitches.h"
#include "note.h"

const uint16_t TONES_8_OCTAVE[] = {
  NOTE_C8,
  NOTE_CS8,
  NOTE_D8,
  NOTE_DS8,
  NOTE_E8,
  NOTE_F8,
  NOTE_FS8,
  NOTE_G8,
  NOTE_GS8,
  NOTE_A8,
  NOTE_AS8,
  NOTE_B8,
  NOTE_P0
};


/**
 * D1 D# R R# M F F# S S# L L# C
 */
Note::Note(uint8_t _note, uint8_t _octave, uint8_t _duration) {
  note = _note;
  octave = _octave;
  duration = _duration;
}

uint16_t Note::tone() {
  if (note < Pause) {
    float factor = (octave < 8) ? (1.0 * (1 << 8 - octave)) :
      (octave > 8) ? (1.0 / (0x1 << octave - 8)) : 1.0;
    return TONES_8_OCTAVE[note] / factor;
  }
  return NOTE_P0;
}

uint32_t Note::delay() {
  return 2000 >> duration;
}
