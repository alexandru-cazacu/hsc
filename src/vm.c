#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "debug.h"
#include <stdio.h>

VM gVM;

static void resetStack() {
    gVM.stackTop = gVM.stack;
}

static void initVM() {
    resetStack();
}

static void freeVM() {
}

static InterpretResult run() {
#define READ_BYTE() (*gVM.ip++)
#define READ_CONSTANT() (gVM.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = pop(); \
        double a = pop(); \
        push(a op b); \
    } while(false)
    
    while (true) {
#ifdef DEBUG_TRACE_EXECUTION
        // Prints stack.
        printf("          ");
        for (Value* slot = gVM.stack; slot < gVM.stackTop; slot++) {
            printf("[");
            printValue(*slot);
            printf("]");
        }
        printf("\n");
        // Prints current instruction.
        disassembleInstruction(gVM.chunk, (int)(gVM.ip - gVM.chunk->code));
#endif
        
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
            } break;
            case OP_ADD:      {
                BINARY_OP(+);
            } break;
            case OP_SUBTRACT: {
                BINARY_OP(-);
            } break;
            case OP_MULTIPLY: {
                BINARY_OP(*);
            } break;
            case OP_DIVIDE:   {
                BINARY_OP(/);
            } break;
            case OP_NEGATE: {
                push(-pop());
            } break;
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef BINARY_OP
#undef READ_CONSTANT
#undef READ_BYTE
}

static InterpretResult interpret(Chunk* chunk) {
    gVM.chunk = chunk;
    gVM.ip = gVM.chunk->code;
    return run();
}

static void push(Value value) {
    *gVM.stackTop = value;
    gVM.stackTop++;
}

static Value pop() {
    gVM.stackTop--;
    return *gVM.stackTop;
}
