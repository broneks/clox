#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "chunk.h"
#include "value.h"

#define OBJ_TYPE(value)  (AS_OBJ(value)->type)

#define IS_CLASS(value) isObjectType(value, OBJ_CLASS)
#define IS_CLOSURE(value) isObjectType(value, OBJ_CLOSURE)
#define IS_FUNCTION(value) isObjectType(value, OBJ_FUNCTION)
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_STRING(value) isObjectType(value, OBJ_STRING)

#define AS_CLASS(value) ((ObjClass*)AS_OBJ(value))
#define AS_CLOSURE(value) ((ObjClosure*)AS_OBJ(value))
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))
#define AS_NATIVE(value) (((ObjNative*)AS_OBJ(value))->function)
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
  OBJ_CLASS,
  OBJ_CLOSURE,
  OBJ_FUNCTION,
  OBJ_NATIVE,
  OBJ_STRING,
  OBJ_UPVALUE
} ObjType;

struct Obj {
  ObjType type;
  bool isMarked;
  struct Obj* next; // linked list
};

typedef struct {
  Obj obj; // pseudo-inheritance / base class
  int arity;
  int upvalueCount;
  Chunk chunk;
  ObjString* name;
} ObjFunction;

typedef Value (*NativeFn)(int argCount, Value* args);

typedef struct {
  Obj obj;
  NativeFn function;
} ObjNative;

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

typedef struct ObjUpvalue {
  Obj obj;
  Value* location;
  Value closed;
  struct ObjUpvalue* next;
} ObjUpvalue;

typedef struct {
  Obj obj;
  ObjFunction* function;
  ObjUpvalue** upvalues;
  int upvalueCount;
} ObjClosure;

typedef struct {
  Obj obj;
  ObjString* name;
} ObjClass;

ObjClass* newClass(ObjString* name);
ObjClosure* newClosure(ObjFunction* function);
ObjFunction* newFunction();
ObjNative* newNative(NativeFn function);
ObjString* takeString(char* chars, int length);
ObjString* copyString(const char*  chars, int length);
ObjUpvalue* newUpvalue(Value* slot);
void printObject(Value value);

// must be separate because the body uses "value" twice. A macro is expanded by inserting the argument expression
// every place the parameter name appears in the body. We don't want to risk evaluating the expression multiple times.
static inline bool isObjectType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
