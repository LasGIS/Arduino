#ifndef LOADCLASSCOMMON_H
#define LOADCLASSCOMMON_H

#include <inttypes.h>

enum CharDefinition: uint8_t {
  charDef   = 'c',   // 0x63
  byteDef   = 'b',   // 0x62
  intDef    = 'i',   // 0x69
  longDef   = 'l',   // 0x6C
  floatDef  = 'f',   // 0x66
  stringDef = 's',   // 0x73
  arrayDef  = 'a',   // 0x61
  objectDef = 'o',   // 0x6F
  referDef  = 'r'    // 0x72
};

extern int getDefinitionSize(CharDefinition cdef);

/*
class LoadClassCommon
{
public:
  LoadClassCommon();
};
*/

#endif // LOADCLASSCOMMON_H
