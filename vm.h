#pragma once

typedef double Value;

// Used to store constant values of Chunk.
typedef struct {
    size_t count;
    size_t capacity;
    Value* values;
    
} ValueArray;

void initValueArray(ValueArray* array);
void freeValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void printValue(Value value);

typedef enum {   // n. bytes
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
