#include "Print.h"
#include <Arduino.h>
#include <SerialBlock.h>
#include <LoadClass.h>
#include "AT24C32_TEST.h"
#include "TestMemory.h"

struct TestObject {
  char c;
  int8_t b;
  int16_t i;
  int32_t l;
  float f;
  char *s;
};

struct TestObjectArray {
  int8_t int8;
  int16_t int16;
  char *str;
  char chr;
};

/**
 * case '1':
 * @brief testAT24C32
 */
void testAT24C32() {
  LoadClass lc(DEVICE, AT24C_Char);
  Serial.print("=> testAT24C32:");
  char c = lc.readByte();
  Serial.print(" Char = ");
  Serial.print(c);

  byte b = lc.readByte();
  Serial.print(", Byte = ");
  Serial.print(b);

  int i = lc.readInt();
  Serial.print(", Int = ");
  Serial.print(i);

  long l = lc.readLong();
  Serial.print(", Long = ");
  Serial.print(l);

  float f = lc.readFloat();
  Serial.print(", Float = ");
  Serial.print(f, 8);

  char *s = lc.readString();
  Serial.print(", String = \"");
  Serial.print(s);
  Serial.println("\"");
}

/**
 * case '2':
 * @brief testString
 */
void testString() {
  LoadClass lc(DEVICE, AT24C_ArrayOfObject_0_Object_String);
  char *s = lc.readString();
  Serial.print("=> String(");
  Serial.print((int)s, HEX);
  Serial.print(") = \"");
  Serial.print(s);
  Serial.println("\"");
}

/**
 * case '3':
 * @brief testObject
 */
void testObject() {
  LoadClass lc(DEVICE, AT24C_Object);
  int len;
  TestObject *obj = (TestObject *)lc.readObject(len);
  Serial.print("=> Object(");
  Serial.print((int)&obj, HEX);
  Serial.print("-");
  Serial.print(len);
  Serial.print(") = ");
  SerialPrintHex((uint8_t *)obj, len);
  Serial.println();
  Serial.print("  CHAR = ");
  Serial.print(obj->c);
  Serial.print(", INT8 = ");
  Serial.print(obj->b);
  Serial.print(", INT16 = ");
  Serial.print(obj->i);
  Serial.print(", INT32 = ");
  Serial.print(obj->l);
  Serial.print(", FLOAT = ");
  Serial.print(obj->f, 6);
  Serial.print(", STRING(");
  Serial.print((int)obj->s, HEX);
  Serial.print(") = ");
  Serial.println(obj->s);
}

/**
 * case '31':
 * @brief testObjectToItem
 */
void testObjectToItem() {
  LoadClass lc(DEVICE, 0);
  Serial.print("=> testObjectToItem ");

  lc.toObjectItem(AT24C_Object, 3);
  Serial.print("INT32 = ");
  Serial.print(lc.readLong());

  lc.toObjectItem(AT24C_Object, 0);
  Serial.print(", CHAR = ");
  Serial.print(lc.readByte());

  lc.toObjectItem(AT24C_Object, 5);
  char *str = lc.readString();
  Serial.print(", STRING(");
  Serial.print((int)str, HEX);
  Serial.print(") = ");
  Serial.println(str);
}

/**
 * case '4':
 * @brief testArray
 */
void testArray() {
  LoadClass lc(DEVICE, AT24C_ArrayOfInt);
  int len;
  int *arr = (int *)lc.readArray(len);
  Serial.print("=> testArray(");
  Serial.print(len);
  Serial.println(") = ");
  Serial.print("  ");
  for (int i = 0; i < len; i++) {
    Serial.print(arr[i]);
    Serial.print(", ");
  }
  Serial.println();
}

/**
 * case '41':
 * @brief testArrayToItem
 */
void testArrayToItem() {
  LoadClass lc(DEVICE, 0);
  Serial.print("=> testArrayToItem ");
  Serial.print("[3] = ");
  lc.toArrayItem(AT24C_ArrayOfInt, 3);
  Serial.print(lc.readInt());
  Serial.print(", [0] = ");
  lc.toArrayItem(AT24C_ArrayOfInt, 0);
  Serial.print(lc.readInt());
  Serial.println();
}

/**
 * case '5':
 * @brief testArrayObject
 */
void testArrayObject() {
  LoadClass lc(DEVICE, AT24C_ArrayOfObject);
  int len;
  TestObjectArray **objs = (TestObjectArray **)lc.readArray(len);
  Serial.print("=> testArrayObject(");
  Serial.print(len);
  Serial.println(") = ");
  for (int i = 0; i < len; i++) {
    Serial.print(" Int8 = ");
    Serial.print(objs[i]->int8);
    Serial.print(", Int16 = ");
    Serial.print(objs[i]->int16);
    Serial.print(", String = \"");
    Serial.print(objs[i]->str);
    Serial.print("\", Char = ");
    Serial.print(objs[i]->chr);
    Serial.println(";");
  }
}

/**
 * case '51':
 * @brief testArrayObjectToItem
 */
void testArrayObjectToItem() {
  Serial.print("=> testArrayObjectToItem ");
  LoadClass lc(DEVICE, 0);
  lc.toArrayItem(AT24C_ArrayOfObject, 4);
  lc.toObjectItem(2);
  char *str = lc.readString();
  Serial.print(", STRING(");
  Serial.print((int)str, HEX);
  Serial.print(") = \"");
  Serial.print(str);

  Serial.print("\", CHAR(");
  Serial.print(lc.toArrayItem(AT24C_ArrayOfObject, 4), HEX);
  Serial.print(",");
  Serial.print(lc.toObjectItem(5), HEX);
  Serial.print(")=");
  char chr = lc.readByte();
  Serial.println(chr);
}

/**
 * case '6':
 * @brief testArrayObject
 */
void testJingleBells() {
  Serial.println("testJingleBells");
  LoadClass lc(DEVICE, AT24C_music_0_JingleBells_music);
  int len;
  uint8_t *objs = (uint8_t *)lc.readArray(len);
  Serial.print("=> testJingleBells(");
  Serial.print(len);
  Serial.print(") = ");
  SerialPrintHex(objs, len);
  Serial.println();
}

/**
 * case '61':
 * @brief testJingleBellsToItem
 */
void testJingleBellsToItem() {
  Serial.println("=> testJingleBellsToItem");
  LoadClass lc(DEVICE, 0);

  lc.toObjectItem(AT24C_music_0_JingleBells, 1);
  int len;
  uint8_t *objs = (uint8_t *)lc.readArray(len);
  Serial.print("  music(");
  Serial.print(len);
  Serial.print(") = ");
  SerialPrintHex(objs, len);
  Serial.println();

  lc.toObjectItem(AT24C_music_0_JingleBells, 0);
  char *s = lc.readString();
  Serial.print("  name(");
  Serial.print((int)s, HEX);
  Serial.print(") = \"");
  Serial.print(s);
  Serial.println("\"");
}

struct MusicObject {
  char *name;
  int16_t notesLenght;
  uint8_t *notes;
};


/**
 * case '7':
 * @brief testLoadMusics
 */
void testLoadMusics() {
  Serial.println("=> testLoadMusics");
  LoadClass lc(DEVICE, AT24C_music);
  int count;
  MusicObject **music = (MusicObject **)lc.readArray(count);
  for (int16_t i = 0; i < count; i++) {
    Serial.print("  name(");
    Serial.print((int)music[i]->name, HEX);
    Serial.print(") = \"");
    Serial.print(music[i]->name);
    Serial.println("\"");

    Serial.print("  music(");
    Serial.print(music[i]->notesLenght);
    Serial.print(") = ");
    SerialPrintHex(music[i]->notes, music[i]->notesLenght);
    Serial.println();
    Serial.print(" address = ");
    Serial.println(lc.address, HEX);
  }
}
