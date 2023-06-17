#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h> // isdigit()

#include "scan.h"

typedef enum {
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_INTLIT,
} ASTNodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
    ASTNodeType op;
    ASTNode* left;
    ASTNode* right;
    int intValue;
};

static Token g_Token;

#include "scan.c"
#include "expression.c"
#include "tree.c"
#include "vm.c"
#include "memory.c"
#include "debug.c"

int main() {
    Token t;
    
    ASTNode* node;
    
    const char* src =
        "let a = 5;"
        " // This is a comment  \n"
        " for() (); == 567!= >= <= +\"asd\";";
    g_Scanner.source.data = src;
    g_Scanner.source.length = strlen(src);
    
    while(true) {
        int res = scanToken(&t);
        if (res == 1) {
            printToken(&t);
        } else if (res == -1) {
            continue;
        } else {
            break;
        }
    }
    
    if (g_Scanner.hasError) {
        return 65;
    }
    
    //printf("Token %d, value: %d\n", g_Token.type, g_Token.intValue);
    //scanToken(&g_Token);
    //node = binaryExpression();
    //printf("%d\n", interpretAST(node));
    
    size_t line = 42;
    Chunk chunk = {0};
    initChunk(&chunk);
    uint8_t constantIdx = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, line);
    writeChunk(&chunk, constantIdx, line);
    writeChunk(&chunk, OP_RETURN, line);
    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    
    return 0;
}