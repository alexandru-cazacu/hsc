#include "common.h"
#include "memory.h"

// oldSize  | newSize   | operation
// ---------|-----------|----------
//
// 0        | non-zero  | alloc.
// non-zero | 0         | free.
// non-zero | < oldSize | shrink.
// non-zero | > oldSize | grow.
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }
    
    // TODO(alex): Maybe implement some kind of memory arena?
    void* result = realloc(pointer, newSize);
    if (result == NULL) {
        exit(1);
    }
    return result;
}
