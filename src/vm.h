#pragma once

#include "chunk.h"
#include "object.h"
#include "value.h"
#include "table.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

// Ongoing function call.
typedef struct {
    ObjClosure* closure;
    uint8_t* ip; // Points to the NEXT instruction about to be executed.
    Value* slots; // First usable position by this function in the stack.
} CallFrame;

typedef struct {
    CallFrame frames[FRAMES_MAX];
    int frameCount;
    Value stack[STACK_MAX];
    // Points ONE element past the element containing the top value on the
    // stack. This way we can indicate that the stack is empty by pointing at
    // element zero in the array.
    // [a][_][_] // stackTop = 1
    //     ↑
    Value* stackTop;
    Table globals;
    Table strings; // For strings interning.
    ObjUpvalue* openUpvalues;
    Obj* objects; // Linked list of allocated objects.
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM gVM;

static void initVM();
static void freeVM();
static InterpretResult interpret(const char* source);
static void push(Value value);
static Value pop();
