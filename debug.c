#include "debug.h"
#include "chunk.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);
    
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

uint32_t simpleInstruction(const char* name, uint32_t offset) {
    printf("%s\n", name);
    return offset + 1;
}

uint32_t constantInstruction(const char* name, Chunk* chunk, uint32_t offset) {
    uint8_t constantIdx = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constantIdx);
    printValue(chunk->constants.values[constantIdx]);
    printf("'\n");
    
    return offset + 2;
}

// Returns new offset
uint32_t disassembleInstruction(Chunk* chunk, uint32_t offset) {
    printf("%04d ", offset);
    
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }
    
    uint8_t instruction = chunk->code[offset];
    switch(instruction) {
        case OP_CONSTANT: {
            return constantInstruction("OP_CONSTANT", chunk, offset);
        }
        case OP_RETURN: {
            return simpleInstruction("OP_RETURN", offset);
        }
        default: {
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
        }
    }
}
