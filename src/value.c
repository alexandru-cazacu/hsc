#include "value.h"
#include "common.h"
#include "chunk.h"
#include "memory.h"

#include <assert.h>

void initValueArray(ValueArray* array) {
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void freeValueArray(ValueArray* array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

void writeValueArray(ValueArray* array, Value value) {
    // OP_CONSTANT is a byte, we can't have more than 256 constants in a chunk.
    assert(array->count < 256);

    if (array->capacity < array->count + 1) {
        size_t oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
    }
    
    array->values[array->count] = value;
    array->count++;
}

void printValue(Value value) {
    printf("%g", value);
}
