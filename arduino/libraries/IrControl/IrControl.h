#ifndef IrControl_h
#define IrControl_h

#include <Arduino.h>

#define IR_CONTROL_MINIMAL_TIME 200
#define IR_CONTROL_MAXIMAL_TIME 20000
#define IR_CONTROL_MINIMAL_COUNT 14
#define IR_CONTROL_MAXIMAL_COUNT 1300
#define IR_CONTROL_BOUND_COUNT 80

//#define FIRST_KEYBOARD
//#define PLAY_KEYBOARD
#define RUBBER_KEYBOARD

class IrControlKey {
public:
  char key;
  long code;
  int tone;
};

class IrControl {
  
public:
  unsigned long code = 0l;

private:
  int irPin;
  long startTime;
  bool _hasCode = false;
  static IrControl* _activeIrControlObject;

public:
  IrControl(int pin);
  bool hasCode();
  unsigned long getCode();
  void start();
  void stop();
  char toKey(long code);
  IrControlKey* toControlKey(long code);

private:
  static inline void handle_interrupt();
  void interrupt();
  int wait(byte val);
  boolean decode();
  
};

#endif
