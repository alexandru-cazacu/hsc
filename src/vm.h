#pragma once

#include "chunk.h"
#include "value.h"

// TODO(alex): Check that we don't stack overflow.
#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip; // Points to the NEXT instruction about to be executed.
    Value stack[STACK_MAX];
    // Points ONE element past the element containing the top value on the
    // stack. This way we can indicate that the stack is empty by pointing at
    // element zero in the array.
    // [a][_][_] // stackTop = 1
    //     ↑
    Value* stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

static void initVM();
static void freeVM();
static InterpretResult interpret(const char* source);
static void push(Value value);
static Value pop();
