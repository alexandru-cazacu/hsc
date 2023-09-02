#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h> // isdigit()

#include "scanner.h"
#include "chunk.h"
#include "vm.h"
#include "debug.h"

#include "vm.c"
#include "memory.c"
#include "debug.c"
#include "chunk.c"
#include "value.c"
#include "scanner.c"
#include "compiler.c"

int main() {
    initVM();
    
    size_t line = 42;
    Chunk chunk = {0};
    initChunk(&chunk);
    uint8_t constantIdx = addConstant(&chunk, (float)1.2);
    writeChunk(&chunk, OP_CONSTANT, line);
    writeChunk(&chunk, constantIdx, line);

  constantIdx = addConstant(&chunk, 3.4);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constantIdx, 123);

  writeChunk(&chunk, OP_ADD, 123);

  constantIdx = addConstant(&chunk, 5.6);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constantIdx, 123);

  writeChunk(&chunk, OP_DIVIDE, 123);

    writeChunk(&chunk, OP_NEGATE, line);
    writeChunk(&chunk, OP_RETURN, line);
    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);
    freeChunk(&chunk);
    freeVM();
    
    // compile("1 + 2;\n"
    //         "// A comment\n"
    //         "fun test() {\n"
    //         "  print();"
    //         "}");
    
    // return 0;
}
