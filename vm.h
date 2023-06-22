#pragma once

#include "chunk.h"

typedef struct {
    Chunk* chunk;
    uint8_t* ip; // Points to the NEXT instruction about to be executed
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);