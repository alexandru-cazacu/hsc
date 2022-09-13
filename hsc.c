#define local static

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//~
// Scanner
//

typedef enum {
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_INTLIT,
    
    T_OPEN_BRACE,
    T_CLOSE_BRACE,
    T_OPEN_PAREN,
    T_CLOSE_PAREN,
    T_SEMICOLON,
    T_COLON,
    T_INT,
    T_RETURN,
    T_IDENTIFIER,
    T_NUMBER,
    T_EOF,
    
    T_COUNT
} TokenType;

typedef struct {
    TokenType type;
    int begin;
    int end;
    int intValue; // ?
} Token;

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

#define MAX_TOKENS 256
int tokensIndex = 0;
static Token tokens[MAX_TOKENS];

void printToken(int index) {
    switch(tokens[index].type) {
        case T_OPEN_PAREN: {
            printf("T_OPEN_PAREN\n");
            return;
        }
        case T_CLOSE_PAREN: {
            printf("T_CLOSE_PAREN\n");
            return;
        }
        case T_OPEN_BRACE: {
            printf("T_OPEN_BRACE\n");
            return;
        }
        case T_CLOSE_BRACE: {
            printf("T_CLOSE_BRACE\n");
            return;
        }
        case T_SEMICOLON: {
            printf("T_SEMICOLON\n");
            return;
        }
        case T_COLON: {
            printf("T_COLON\n");
            return;
        }
        case T_IDENTIFIER: {
            printf("T_IDENTIFIER ");
            for (int i = tokens[index].begin; i <= tokens[index].end; ++i) {
                printf("%c", src[i]);
            }
            printf("\n");
            return;
        }
        case T_NUMBER: {
            printf("T_NUMBER\n");
            return;
        }
        case T_EOF: {
            printf("T_EOF\n");
            return;
        }
    }
}

int main() {
    char *tokstr[] = { "+", "-", "*", "/", "intlit" };
    Token t;
    
#if 0
    while(scan(&t)) {
        printf("Token: %s", tokstr[t.type]);
        if (t.type == T_INTLIT) {
            printf(", value %d", t.intValue);
        }
        printf("\n");
    }
#endif
    
    ASTNode* node;
    
    scan(&g_Token);                 // Get the first token from the input
    node = binaryExpression();                // Parse the expression in the file
    printf("%d\n", interpretAST(node));      // Calculate the final result
    exit(0);
    
#if 0
    int index = 0;
    char c = src[index];
    while (c != '\0') {
        if (c == ' ') {
            c = src[++index];
            continue;
        }
        
        if (c >= '0' && c <= '9') {
            int begin = index;``
                while (c >= '0' && c <= '0') {
                printf("%c", c);
                c = src[++index];
            }
            
            int end = index - 1;
            printf(" %d %d\n", begin, end);
            
            Token token = {0};
            token.type = T_NUMBER;
            tokens[tokensIndex++] = token;
        } else if (c >= 'a' && c <= 'z') {
            int begin = index;
            while (c >= 'a' && c <= 'z') {
                printf("%c", c);
                c = src[++index];
            }
            
            int end = index - 1;
            printf(" %d %d\n", begin, end);
            
            Token token = {0};
            token.type = T_IDENTIFIER;
            token.begin = begin;
            token.end = end;
            tokens[tokensIndex++] = token;
        } else {
            if (c == '(') {
                Token token = {0};
                token.type = T_OPEN_PAREN;
                tokens[tokensIndex++] = token;
            }
            
            if (c == ')') {
                Token token = {0};
                token.type = T_CLOSE_PAREN;
                tokens[tokensIndex++] = token;
            }
            
            if (c == '{') {
                Token token = {0};
                token.type = T_OPEN_BRACE;
                tokens[tokensIndex++] = token;
            }
            
            if (c == '}') {
                Token token = {0};
                token.type = T_CLOSE_BRACE;
                tokens[tokensIndex++] = token;
            }
            
            if (c == ';') {
                Token token = {0};
                token.type = T_SEMICOLON;
                tokens[tokensIndex++] = token;
            }
            if (c == ':') {
                Token token = {0};
                token.type = T_COLON;
                tokens[tokensIndex++] = token;
            }
            
            c = src[++index];
        }
    }
    
    Token token = {0};
    token.type = T_EOF;
    tokens[tokensIndex++] = token;
    
    printf("\n");
    for (int i = 0; i < tokensIndex; i++) {
        printToken(i);
    }
#endif
    
    return 0;
}