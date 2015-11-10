#ifndef IrControl_h
#define IrControl_h

#include <MsTimer2.h>
#include <Arduino.h>

#define IR_CONTROL_MINIMAL_TIME 200
#define IR_CONTROL_MAXIMAL_TIME 20000
#define IR_CONTROL_MINIMAL_COUNT 14
#define IR_CONTROL_MAXIMAL_COUNT 1300
#define IR_CONTROL_BOUND_COUNT 80

/*
#define READ_BUF_SIZE 400
short readBuf[READ_BUF_SIZE];
#define IR_INTERRUPT_BUF_SIZE 66
*/

class IrControl {
	
  private:
	int irPin;
	// int count = -1;
	long startTime;
	//long times[IR_INTERRUPT_BUF_SIZE];
	//byte values[IR_INTERRUPT_BUF_SIZE];
	bool _hasCode = false;
	unsigned long code = 0l;
	static IrControl* _activeObject;

  public:
	IrControl(int pin);
	bool hasCode();
	unsigned long getCode();
	void start();
	void stop();

  private:
	static inline void handle_interrupt();
	void interrupt();
	boolean decode();
	int wait(byte val);
};

#endif
