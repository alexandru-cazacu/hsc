#pragma once

//-----------------------------------------------------------------------------
// Scanner
//

typedef struct {
    int index;
    int putback;
    int line;
    const char* src;
} Scanner;

typedef enum {
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_INTLIT,
    
    T_EOF,
#if 0
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
    
    T_COUNT
#endif
} TokenType;

typedef struct {
    TokenType type;
    int begin;
    int end;
    int intValue; // ?
} Token;