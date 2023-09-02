#pragma once

#include "common.h"
#include "value.h"

//
// Chunk - Sequences of bytecode.
//

typedef enum {   // bytes
    OP_CONSTANT, // [opcode][constant index]
    OP_ADD,      // [opcode]
    OP_SUBTRACT, // [opcode]
    OP_MULTIPLY, // [opcode]
    OP_DIVIDE,   // [opcode]
    OP_NEGATE,   // [opcode]
    OP_RETURN,   // [opcode]
} OpCode;

typedef struct {
    size_t count;
    size_t capacity;
    uint8_t* code;
    // Each OpCode or constant has a relative entry is the lines array used for
    // when an error occurs to retrieve line information.
    // TODO(alex): Huge waste of memory. Implement run length encoding.
    uint32_t* lines;
    ValueArray constants; // Constant values used by OP_CONSTANT.
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
// Writes and OpCode or constant to the given Chunk.
// byte - OpCode or constant to write.
// line - Source line the byte came from.
void writeChunk(Chunk* chunk, uint8_t byte, uint32_t line);
// Returns the index of where the constant was placed in the constants store of
// the Chunk.
size_t addConstant(Chunk* chunk, Value value);
