#ifndef MotorShieldDefine_h
#define MotorShieldDefine_h

// ��������� ������ ������ ���������� ����������
#define MSHLD_DEBUG_MODE

#define FORWARD true
#define BACKWARD false

// ��� DIR_SER   / DS    / 14 ����� 74HC595
#define MSHLD_DIR_SER_PIN   8

// ��� DIR_CLK   / SH_CP / 11 ����� 74HC595
#define MSHLD_DIR_CLK_PIN   4

// ��� DIR_LATCH / ST_CP / 12 ����� 74HC595
#define MSHLD_DIR_LATCH_PIN 12

// ��� DIR_EN    / OE    / 13 ����� 74HC595
#define MSHLD_DIR_EN_PIN    7

// ��� PWM0A --- ��������� M4 (PIN 4, PIN 5)
#define MSHLD_PWM0A_PIN     6
// ��� PWM0B --- ��������� M3 (PIN 1, PIN 2)
#define MSHLD_PWM0B_PIN     5

// ��� PWM2A --- ��������� M1 (PIN 4, PIN 5)
#define MSHLD_PWM2A_PIN     11
// ��� PWM2B --- ��������� M2 (PIN 1, PIN 2)
#define MSHLD_PWM2B_PIN     3

// ��� PWM1A --- ���������� � 2
#define MSHLD_PWM1A_PIN     9
// ��� PWM1B --- ���������� � 1
#define MSHLD_PWM1B_PIN     10

// ��� �������� ������ ������
#define MSHLD_LEFT_COUNT_PIN 3
// ��� �������� ������� ������
#define MSHLD_RIGHT_COUNT_PIN 2

// ����� �������
#define MSHLD_UP_M3A_MASK  B00000001
#define MSHLD_UP_M2A_MASK  B00000010
#define MSHLD_UP_M1A_MASK  B00000100
#define MSHLD_UP_M1B_MASK  B00001000
#define MSHLD_UP_M2B_MASK  B00010000
#define MSHLD_UP_M4A_MASK  B00100000
#define MSHLD_UP_M3B_MASK  B01000000
#define MSHLD_UP_M4B_MASK  B10000000

#define MSHLD_DOWN_M3A_MASK  B11111110
#define MSHLD_DOWN_M2A_MASK  B11111101
#define MSHLD_DOWN_M1A_MASK  B11111011
#define MSHLD_DOWN_M1B_MASK  B11110111
#define MSHLD_DOWN_M2B_MASK  B11101111
#define MSHLD_DOWN_M4A_MASK  B11011111
#define MSHLD_DOWN_M3B_MASK  B10111111
#define MSHLD_DOWN_M4B_MASK  B01111111

#define MSHLD_M1 0
#define MSHLD_M2 1
#define MSHLD_M3 2
#define MSHLD_M4 3

#define WAIT_BUSY_TIME 1
#define MSHLD_DEL_TIME 20

#endif