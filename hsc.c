#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

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
    char *tokstr[] = { "+", "-", "*", "/", "T_INTLIT" };
    Token t;
    
    ASTNode* node;
    
    g_Scanner.src = "3 * 1 + 2;\n";
    //g_Scanner.src = "let a = 5\n";
    
    while(scan(&t)) {
        printf("Token: %s", tokstr[t.type]);
        if (t.type == T_INTLIT) {
            printf(", value %d", t.intValue);
        }
        printf("\n");
    }
    
    printf("Token %d, value: %d\n", g_Token.type, g_Token.intValue);
    //scan(&g_Token);
    //node = binaryExpression();
    //printf("%d\n", interpretAST(node));
    
    return 0;
}