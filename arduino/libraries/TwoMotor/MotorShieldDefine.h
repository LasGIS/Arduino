#ifndef MotorShieldDefine_h
#define MotorShieldDefine_h

// установка режима вывода отладочной информации
#define MSHLD_DEBUG_MODE

#define FORWARD true
#define BACKWARD false

// Пин DIR_SER   / DS    / 14 входу 74HC595
#define MSHLD_DIR_SER_PIN   8

// Пин DIR_CLK   / SH_CP / 11 входу 74HC595
#define MSHLD_DIR_CLK_PIN   4

// Пин DIR_LATCH / ST_CP / 12 входу 74HC595
#define MSHLD_DIR_LATCH_PIN 12

// Пин DIR_EN    / OE    / 13 входу 74HC595
#define MSHLD_DIR_EN_PIN    7

// Пин PWM0A --- мощьность M4 (PIN 4, PIN 5)
#define MSHLD_PWM0A_PIN     6
// Пин PWM0B --- мощьность M3 (PIN 1, PIN 2)
#define MSHLD_PWM0B_PIN     5

// Пин PWM2A --- мощьность M1 (PIN 4, PIN 5)
#define MSHLD_PWM2A_PIN     11
// Пин PWM2B --- мощьность M2 (PIN 1, PIN 2)
#define MSHLD_PWM2B_PIN     3

// Пин PWM1A --- Сервомотор № 2
#define MSHLD_PWM1A_PIN     9
// Пин PWM1B --- Сервомотор № 1
#define MSHLD_PWM1B_PIN     10

// Пин счетчика левого мотора
#define MSHLD_LEFT_COUNT_PIN 3
// Пин счетчика правого мотора
#define MSHLD_RIGHT_COUNT_PIN 2

// маски моторов
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

#define MSHLD_DEL_TIME 1

#endif