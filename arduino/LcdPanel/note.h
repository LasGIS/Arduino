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
  /* ���� � �������: 0-> ��; 1-> ��-���� ... 11-> ��;
   * 12 -> �����; 13 -> stop; 14-> ??; 15-> ?? */
  uint8_t note : 4;
  /* ����� ������ 0 -> ���(��������������), 1 -> ��(�����������),
   * 2 -> ��(������� ������), 3 -> ��(����� ������),
   * 4 -> 1(������ ������), 5 -> 2(������ ������) ...   */
  uint8_t octave : 4;
  /* 0-> �����; 1-> ��������; 2-> ���������; 3-> �������;
   * 4-> 1/16; 5-> 1/32; 6-> ...; 7-> ...; */
  uint8_t duration : 3;
  Note(uint8_t _note, uint8_t _octave, uint8_t _duration);
  uint16_t tone();
  uint32_t delay();
};

#endif // NOTE_H
