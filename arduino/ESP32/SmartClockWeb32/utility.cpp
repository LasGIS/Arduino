#include "SmartClockWeb32.h"

/**
 * переводим целое в строку (две цифры)
 * @param val целое значение
 * @param str строка
 * @param start начало вывода в строке
 */
void toTwoChar(int val, String& str, unsigned int start) {
  char fst = '0' + (val % 100) / 10;
  char lst = '0' + val % 10;
  str[start] = fst;
  str[start + 1] = lst;
}
