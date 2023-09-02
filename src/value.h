#pragma once

// For now we only support double precision floats.
typedef double Value;

// Used to store all the constant values that appear in a single Chunk.
typedef struct {
    size_t count;
    size_t capacity;
    Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void freeValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void printValue(Value value);
