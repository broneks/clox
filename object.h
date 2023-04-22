#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value)  (AS_OBJ(value)->type)

#define IS_STRING(value) isObjectType(value, OBJ_STRING)

#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
  OBJ_STRING,
} ObjType;

struct Obj {
  ObjType type;
};

/*
 * Given an ObjString* you can safely cast it to Obj* and then access the type field from it.
 * Every ObjString is an Obj in the OOP sense of "is".
 *
 * NOTE: C spec: There may be unnamed padding within a structure object, but NOT at its beginning.
 **/
struct ObjString {
  Obj obj; // pseudo-inheritance / base class
  int length;
  char* chars;
};

ObjString* copyString(const char*  chars, int length);
void printObject(Value value);

// must be separate because the body uses "value" twice. A macro is expanded by inserting the argument expression
// every place the parameter name appears in the body. We don't want to risk evaluating the expression multiple times.
static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
