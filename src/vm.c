#include "vm.h"
#include "common.h"
#include "debug.h"

VM gVM;

void initVM() {
}

void freeVM() {
}

static InterpretResult run() {
#define READ_BYTE() (*gVM.ip++)
#define READ_CONSTANT() (gVM.chunk->constants.values[READ_BYTE()])
    
    while (true) {
#ifdef DEBUG_TRACE_EXECUTION
        disassembleInstruction(gVM.chunk, (int)(gVM.ip - gVM.chunk->code));
#endif
        
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                printValue(constant);
                printf("\n");
            } break;
            case OP_RETURN: {
                return INTERPRET_OK;
            }
        }
    }
    
#undef READ_CONSTANT
#undef READ_BYTE
}

InterpretResult interpret(Chunk* chunk) {
    gVM.chunk = chunk;
    gVM.ip = gVM.chunk->code;
    return run();
}