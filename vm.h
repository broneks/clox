#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "table.h"
#include "value.h"

#define STACK_MAX 259

typedef struct {
  Chunk* chunk;
  uint8_t* ip; // instruction pointer
  Value stack[STACK_MAX];
  Value* stackTop;
  Table globals; // global variables
  Table strings; // interned strings (deduped)
  Obj* objects;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();

#endif
