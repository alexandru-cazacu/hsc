#include <stdio.h>
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

int main() {
    Token t;
    
    ASTNode* node;
    
    const char* src = "let a = 5; // This is a comment \n for() (); == 567!= >= <= +\"asd\";";
    g_Scanner.source.data = src;
    g_Scanner.source.length = strlen(src);
    //g_Scanner.src = "let a = 5\n";
    
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
    
    return 0;
}