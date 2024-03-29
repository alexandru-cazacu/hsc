#pragma once

#include "common.h"
#include "value.h"

//
// Chunk - Sequences of bytecode.
//

// TODO(alex): Add composite comparison operators for performance. Not these
// comparisons are implemented with two instructions.
// a!=b -> !(a==b) implemented with OP_EQUAL+OP_NOT
// a<=b -> !(a>b)  implemented with OP_GREATER+OP_NOT
// a>=b -> !(a<b)  implemented with OP_LESS+OP_NOT
typedef enum {        // bytes
    OP_CONSTANT,      // [opcode][constant index]
    OP_NIL,           // [opcode]
    OP_TRUE,          // [opcode]
    OP_FALSE,         // [opcode]
    OP_POP,           // [opcode]
    OP_GET_LOCAL,     // [opcode][local index]
    OP_SET_LOCAL,     // [opcode][local index]
    OP_GET_GLOBAL,    // [opcode]
    OP_DEFINE_GLOBAL, // [opcode][constant index]
    OP_SET_GLOBAL,    // [opcode]
    OP_GET_UPVALUE,   // [opcode]
    OP_SET_UPVALUE,   // [opcode]
    OP_GET_SUPER,     // [opcode]
    OP_EQUAL,         // [opcode]
    OP_GET_PROPERTY,  // [opcode]
    OP_SET_PROPERTY,  // [opcode]
    OP_GREATER,       // [opcode]
    OP_LESS,          // [opcode]
    OP_ADD,           // [opcode]
    OP_SUBTRACT,      // [opcode]
    OP_MULTIPLY,      // [opcode]
    OP_DIVIDE,        // [opcode]
    OP_NOT,           // [opcode]
    OP_NEGATE,        // [opcode]
    OP_PRINT,         // [opcode]
    OP_JUMP,          // [opcode][offset]+[offset]
    OP_JUMP_IF_FALSE, // [opcode][offset]+[offset]
    OP_LOOP,          // [opcode][offset]+[offset]
    OP_CALL,          // [opcode][arg count]
    OP_INVOKE,        // [opcode][constant index][arg count]
    OP_SUPER_INVOKE,  // [opcode][constant index][arg count]
    OP_CLOSURE,       // [opcode][isLocal][local slot / upvalue index]
    OP_CLOSE_UPVALUE, // [opcode]
    OP_RETURN,        // [opcode]
    OP_CLASS,         // [opcode]
    OP_INHERIT,       // [opcode]
    OP_METHOD,        // [opcode]
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
