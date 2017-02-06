#ifndef NOTE_H
#define NOTE_H

#include <stdint.h>

enum NoteType : uint8_t {
  Do = 0, DoDies = 1, Re = 2, ReDies =  3, Mi =  4, Fa = 5, FaDies = 6,
  Sl = 7, SlDies = 8, La = 9, LaDies = 10, Ci = 11,
  Pause = 12, Stop = 12
};

/**
 *
 */
class Note {
public:
  /* нота с диезами: 0-> до; 1-> до-диез ... 11-> си;
   * 12 -> пауза; 13 -> stop; 14-> ??; 15-> ?? */
  uint8_t note : 4;
  /* номер октавы 0 -> СКО(субконтроктава), 1 -> КО(контроктава),
   * 2 -> БО(большая октава), 3 -> МО(малая октава),
   * 4 -> 1(первая октава), 5 -> 2(вторая октава) ...   */
  uint8_t octave : 4;
  /* 0-> целая; 1-> половина; 2-> четвертая; 3-> восьмая;
   * 4-> 1/16; 5-> 1/32; 6-> ...; 7-> ...; */
  uint8_t duration : 3;
  Note(uint8_t _note, uint8_t _octave, uint8_t _duration);
  uint16_t tone();
  uint32_t delay();
};

#endif // NOTE_H
