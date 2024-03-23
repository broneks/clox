#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "chunk.h"
#include "value.h"

#define OBJ_TYPE(value)  (AS_OBJ(value)->type)

#define IS_FUNCTION(value) isObjectType(value, OBJ_FUNCTION)
#define IS_STRING(value) isObjectType(value, OBJ_STRING)

#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
  OBJ_FUNCTION,
  OBJ_STRING
} ObjType;

struct Obj {
  ObjType type;
  struct Obj* next; // linked list
};

typedef struct {
  Obj obj; // pseudo-inheritance / base class
  int arity;
  Chunk chunk;
  ObjString* name;
} ObjFunction;

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
  uint32_t hash;
};

ObjFunction* newFunction();
ObjString* takeString(char* chars, int length);
ObjString* copyString(const char*  chars, int length);
void printObject(Value value);

// must be separate because the body uses "value" twice. A macro is expanded by inserting the argument expression
// every place the parameter name appears in the body. We don't want to risk evaluating the expression multiple times.
static inline bool isObjectType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
