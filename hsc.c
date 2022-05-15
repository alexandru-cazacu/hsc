#include <stdio.h>
#include <string.h>

typedef enum {
    TOKEN_TYPE_OPEN_BRACE,
    TOKEN_TYPE_CLOSE_BRACE,
    TOKEN_TYPE_OPEN_PAREN,
    TOKEN_TYPE_CLOSE_PAREN,
    TOKEN_TYPE_SEMICOLON,
    TOKEN_TYPE_COLON,
    TOKEN_TYPE_INT,
    TOKEN_TYPE_RETURN,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_EOF,
    
    TOKEN_TYPE_COUNT
} TokenType;

const char src[] = "main(): number { return 0; }";

typedef struct {
    TokenType type;
    int begin;
    int end;
} Token;

#define MAX_TOKENS 256
int tokensIndex = 0;
static Token tokens[MAX_TOKENS];

void printToken(int index) {
    switch(tokens[index].type) {
        case TOKEN_TYPE_OPEN_PAREN: {
            printf("TOKEN_TYPE_OPEN_PAREN\n");
            return;
        }
        case TOKEN_TYPE_CLOSE_PAREN: {
            printf("TOKEN_TYPE_CLOSE_PAREN\n");
            return;
        }
        case TOKEN_TYPE_OPEN_BRACE: {
            printf("TOKEN_TYPE_OPEN_BRACE\n");
            return;
        }
        case TOKEN_TYPE_CLOSE_BRACE: {
            printf("TOKEN_TYPE_CLOSE_BRACE\n");
            return;
        }
        case TOKEN_TYPE_SEMICOLON: {
            printf("TOKEN_TYPE_SEMICOLON\n");
            return;
        }
        case TOKEN_TYPE_COLON: {
            printf("TOKEN_TYPE_COLON\n");
            return;
        }
        case TOKEN_TYPE_IDENTIFIER: {
            printf("TOKEN_TYPE_IDENTIFIER ");
            for (int i = tokens[index].begin; i <= tokens[index].end; ++i) {
                printf("%c", src[i]);
            }
            printf("\n");
            return;
        }
        case TOKEN_TYPE_NUMBER: {
            printf("TOKEN_TYPE_NUMBER\n");
            return;
        }
        case TOKEN_TYPE_EOF: {
            printf("TOKEN_TYPE_EOF\n");
            return;
        }
    }
}

int main() {
    int index = 0;
    char c = src[index];
    while (c != '\0') {
        if (c == ' ') {
            c = src[++index];
            continue;
        }
        
        if (c >= '0' && c <= '9') {
            int begin = index;
            while (c >= '0' && c <= '0') {
                printf("%c", c);
                c = src[++index];
            }
            
            int end = index - 1;
            printf(" %d %d\n", begin, end);
            
            Token token = {0};
            token.type = TOKEN_TYPE_NUMBER;
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
            token.type = TOKEN_TYPE_IDENTIFIER;
            token.begin = begin;
            token.end = end;
            tokens[tokensIndex++] = token;
        } else {
            if (c == '(') {
                Token token = {0};
                token.type = TOKEN_TYPE_OPEN_PAREN;
                tokens[tokensIndex++] = token;
            }
            
            if (c == ')') {
                Token token = {0};
                token.type = TOKEN_TYPE_CLOSE_PAREN;
                tokens[tokensIndex++] = token;
            }
            
            if (c == '{') {
                Token token = {0};
                token.type = TOKEN_TYPE_OPEN_BRACE;
                tokens[tokensIndex++] = token;
            }
            
            if (c == '}') {
                Token token = {0};
                token.type = TOKEN_TYPE_CLOSE_BRACE;
                tokens[tokensIndex++] = token;
            }
            
            if (c == ';') {
                Token token = {0};
                token.type = TOKEN_TYPE_SEMICOLON;
                tokens[tokensIndex++] = token;
            }
            if (c == ':') {
                Token token = {0};
                token.type = TOKEN_TYPE_COLON;
                tokens[tokensIndex++] = token;
            }
            
            c = src[++index];
        }
    }
    
    Token token = {0};
    token.type = TOKEN_TYPE_EOF;
    tokens[tokensIndex++] = token;
    
    printf("\n");
    for (int i = 0; i < tokensIndex; i++) {
        printToken(i);
    }
    
    return 0;
}