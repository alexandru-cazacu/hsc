#pragma once

//-----------------------------------------------------------------------------
// Scanner
//

typedef struct {
	int length;
	const char* data;
} String;

typedef struct {
	String source;
    bool hasError;
    
	int start; // start char of lexeme
	int current; // current char of lexeme
	int line;
} Scanner;

typedef enum {
    // Single char
    T_OPEN_BRACE,
    T_CLOSE_BRACE,
    T_OPEN_PAREN,
    T_CLOSE_PAREN,
    T_OPEN_SQUARE_BRACE,
    T_CLOSE_SQUARE_BRACE,
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_COMMA,
    T_DOT,
    T_SEMICOLON, // ;
    T_COLON,// :
    
    // One or two chars
    T_BANG, T_BANG_EQUAL,
    T_EQUAL, T_EQUAL_EQUAL,
    T_GREATER, T_GREATER_EQUAL,
    T_LESS, T_LESS_EQUAL,
    
    // Literals
    T_IDENTIFIER, T_STRING, T_INTLIT,
    
    // Keywords
    T_CLASS, T_CONST, T_ELSE, T_FALSE, T_FOR, T_IF, T_LET, T_NULL, T_UNDEFINED,
    T_RETURN, T_NUMBER, T_THIS, T_TRUE, T_VAR, T_WHILE,
    
    T_EOF,
    
    T_COUNT
} TokenType;

typedef struct {
    TokenType type;
    int begin;
    int end;
    int intValue;
    
    //char* lexeme;
    //int line;
} Token;

static printToken(Token* token);