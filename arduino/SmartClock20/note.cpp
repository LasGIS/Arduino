#include "pitches.h"
#include "note.h"

const uint16_t TONES_8_OCTAVE[] = {
  NOTE_C8,    // до
  NOTE_CS8,   // до dies
  NOTE_D8,    // ре
  NOTE_DS8,   // ре dies
  NOTE_E8,    // ми
  NOTE_F8,    // фа
  NOTE_FS8,   // фа dies
  NOTE_G8,    // соль
  NOTE_GS8,   // соль dies
  NOTE_A8,    // ля
  NOTE_AS8,   // ля dies
  NOTE_B8,    // си
  NOTE_P0     // пауза
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
    float factor = (octave < 8) ? (1.0 * (1 << (8 - octave))) :
      (octave > 8) ? (1.0 / (1 << (octave - 8))) : 1.0;
    return TONES_8_OCTAVE[note] / factor;
  }
  return NOTE_P0;
}

uint32_t Note::delay() {
  return 2000 >> duration;
}
