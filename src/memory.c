#include "chunk.h"
#include "common.h"
#include "compiler.h"
#include "memory.h"
#include "object.h"
#include "vm.h"

#define GC_HEAP_GROW_FACTOR 2

// oldSize  | newSize   | operation
// ---------|-----------|----------
//
// 0        | non-zero  | alloc.
// non-zero | 0         | free.
// non-zero | < oldSize | shrink.
// non-zero | > oldSize | grow.
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    gVM.bytesAllocated += newSize - oldSize;
    if (newSize > oldSize) {
#ifdef DEBUG_STRESS_GC
        collectGarbage();
#endif

        if (gVM.bytesAllocated > gVM.nextGC) {
            collectGarbage();
        }
    }

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

void markObject(Obj* object) {
    if (object == NULL) {
        return;
    }
    if (object->isMarked) {
        return;
    }

#ifdef DEBUG_LOG_GC
    printf("%p mark ", (void*)object);
    printValue(OBJ_VAL(object));
    printf("\n");
#endif

    object->isMarked = true;

    if (gVM.grayCapacity < gVM.grayCount + 1) {
        gVM.grayCapacity = GROW_CAPACITY(gVM.grayCapacity);
        gVM.grayStack = (Obj**)realloc(gVM.grayStack,
                                    sizeof(Obj*) * gVM.grayCapacity);
    }

    gVM.grayStack[gVM.grayCount++] = object;

    if (gVM.grayStack == NULL) {
        exit(1);
    }
}

void markValue(Value value) {
    if (IS_OBJ(value)) {
        markObject(AS_OBJ(value));
    }
}

static void markArray(ValueArray* array) {
    for (int i = 0; i < array->count; i++) {
        markValue(array->values[i]);
    }
}

static void blackenObject(Obj* object) {
#ifdef DEBUG_LOG_GC
    printf("%p blacken ", (void*)object);
    printValue(OBJ_VAL(object));
    printf("\n");
#endif

    switch (object->type) {
        case OBJ_CLASS: {
            ObjClass* klass = (ObjClass*)object;
            markObject((Obj*)klass->name);
        } break;
        case OBJ_CLOSURE: {
            ObjClosure* closure = (ObjClosure*)object;
            markObject((Obj*)closure->function);
            for (int i = 0; i < closure->upvalueCount; i++) {
                markObject((Obj*)closure->upvalues[i]);
            }
        } break;
        case OBJ_FUNCTION: {
            ObjFunction* function = (ObjFunction*)object;
            markObject((Obj*)function->name);
            markArray(&function->chunk.constants);
        } break;
        case OBJ_INSTANCE: {
            ObjInstance* instance = (ObjInstance*)object;
            markObject((Obj*)instance->klass);
            markTable(&instance->fields);
        } break;
        case OBJ_UPVALUE: {
            markValue(((ObjUpvalue*)object)->closed);
        } break;
        case OBJ_NATIVE:
        case OBJ_STRING:
        break;
    }
}

static void freeObject(Obj* object) {
#ifdef DEBUG_LOG_GC
    printf("%p free type %d\n", (void*)object, object->type);
#endif

    switch (object->type) {
        case OBJ_CLASS: {
            FREE(ObjClass, object);
        } break;
        case OBJ_CLOSURE: {
            ObjClosure* closure = (ObjClosure*)object;
            FREE_ARRAY(ObjUpvalue*,
                       closure->upvalues,
                       closure->upvalueCount);
            FREE(ObjClosure, object);
        } break;
        case OBJ_FUNCTION: {
            ObjFunction* function = (ObjFunction*)object;
            freeChunk(&function->chunk);
            FREE(ObjFunction, object);
        } break;
        case OBJ_INSTANCE: {
            ObjInstance* instance = (ObjInstance*)object;
            freeTable(&instance->fields);
            FREE(ObjInstance, object);
        } break;
        case OBJ_NATIVE: {
            FREE(ObjNative, object);
        } break;
        case OBJ_STRING: {
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(ObjString, object);
        } break;
        case OBJ_UPVALUE: {
            FREE(ObjUpvalue, object);
        } break;
    }
}

static void markRoots() {
    for (Value* slot = gVM.stack; slot < gVM.stackTop; slot++) {
        markValue(*slot);
    }

    for (int i = 0; i < gVM.frameCount; i++) {
        markObject((Obj*)gVM.frames[i].closure);
    }

    for (ObjUpvalue* upvalue = gVM.openUpvalues;
        upvalue != NULL;
        upvalue = upvalue->next) {
        markObject((Obj*)upvalue);
    }

    markTable(&gVM.globals);
    markCompilerRoots();
}

static void traceReferences() {
    while (gVM.grayCount > 0) {
        Obj* object = gVM.grayStack[--gVM.grayCount];
        blackenObject(object);
    }
}

static void sweep() {
    Obj* previous = NULL;
    Obj* object = gVM.objects;
    while (object != NULL) {
        if (object->isMarked) {
            object->isMarked = false; // Clear for next run.
            previous = object;
            object = object->next;
        } else {
            Obj* unreached = object;
            object = object->next;
            if (previous != NULL) {
                previous->next = object;
            } else {
                gVM.objects = object;
            }

            freeObject(unreached);
        }
    }
}

void collectGarbage() {
#ifdef DEBUG_LOG_GC
    printf("-- gc begin\n");
    size_t before = gVM.bytesAllocated;
#endif

    markRoots();
    traceReferences();
    tableRemoveWhite(&gVM.strings);
    sweep();

    gVM.nextGC = gVM.bytesAllocated * GC_HEAP_GROW_FACTOR;

#ifdef DEBUG_LOG_GC
    printf("-- gc end\n");
    printf("   collected %zu bytes (from %zu to %zu) next at %zu\n",
           before - gVM.bytesAllocated, before, gVM.bytesAllocated,
           gVM.nextGC);
#endif
}

void freeObjects() {
    Obj* object = gVM.objects;
    while (object != NULL) {
        Obj* next = object->next;
        freeObject(object);
        object = next;
    }

    free(gVM.grayStack);
}
