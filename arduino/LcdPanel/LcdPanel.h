#ifndef LCD_PANEL_H
#define LCD_PANEL_H

#include <Arduino.h>

/* ������ ��� �������� ������ � EEPROM */
#define CUR_COMMAND_ADR 0
#define SHOW_MODE_ADR 1
#define KEY_SOUND_VOLUME_ADR 2
#define MUSIC_SOUND_VOLUME_ADR 3
#define ALARM_CLOCK_ADR 16
#define ALARM_CLOCK_ADR_SIZE 4

/* ������������ ���� */
// ��� ��� �������
#define BUZZER_PIN 8

// ��������� ������ ������ ���������� ����������
//#define HAS_SERIAL
//#define HAS_DEBUG

#define CURRENT_COMMAND_TYPE_MAX 3
/* ������� �����. */
enum CurrentCommandType : uint8_t {
  mainCommand = 0,
  settingsScreen= 1,
  showLCDchars = 2,
  showIRkey = 3/*,
  showDistance = 4 */
};

void musicAlarm();
void buzzerOut(uint16_t hertz, uint32_t del, uint8_t soundVolume);
void loadCustomChars();
void viewCustomDigit(int offset, int digit);

#endif // LCD_PANEL_H
