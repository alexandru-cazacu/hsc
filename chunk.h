#pragma once

#include "common.h"
#include "value.h"

typedef enum { // n. bytes
    OP_CONSTANT, // [opcode][constant index]
    OP_RETURN,   // [opcode]
} OpCode;

typedef struct {
    size_t count;
    size_t capacity;
    uint8_t* code;
    uint32_t* lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, uint32_t line);
size_t addConstant(Chunk* chunk, Value value);
