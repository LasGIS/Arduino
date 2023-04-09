#include "LoadClassCommon.h"

int getDefinitionSize(CharDefinition cdef) {
  switch (cdef) {
  case charDef:
  case byteDef: return 1;
  case referDef:
  case intDef: return 2;
  case longDef:
  case floatDef: return 4;
  case stringDef:
  case arrayDef:
  case objectDef: return -1;
  default: return 0;
  }
}

/*
LoadClassCommon::LoadClassCommon()
{

}
*/
