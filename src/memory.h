#pragma once

#include "common.h"
#include "object.h"

#define ALLOCATE(type, count) \
    (type*)reallocate(NULL, 0, sizeof(type) * (count))

#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

#define GROW_CAPACITY(capacity) \
    ((capacity < 8) ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate(pointer, \
    sizeof(type) * oldCount, \
    sizeof(type) * newCount)

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

// oldSize  | newSize   | operation
// ---------|-----------|----------
//
// 0        | non-zero  | alloc.
// non-zero | 0         | free.
// non-zero | < oldSize | shrink.
// non-zero | > oldSize | grow.
void* reallocate(void* pointer, size_t oldSize, size_t newSize);
void freeObjects();
