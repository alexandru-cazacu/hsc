#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h> // isdigit()

#include "scanner.h"

#include "vm.c"
#include "memory.c"
#include "debug.c"
#include "chunk.c"
#include "value.c"
#include "scanner.c"
#include "compiler.c"

int main() {
#if 0
    initVM();
    
    size_t line = 42;
    Chunk chunk = {0};
    initChunk(&chunk);
    uint8_t constantIdx = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, line);
    writeChunk(&chunk, constantIdx, line);
    writeChunk(&chunk, OP_RETURN, line);
    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);
    freeChunk(&chunk);
    freeVM();
#endif
    
    compile("1 + 2;\n"
            "// A comment\n"
            "fun test() {\n"
            "  print();"
            "}");
    
    return 0;
}
