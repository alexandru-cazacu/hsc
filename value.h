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
