#include "scan.h"

static Scanner g_Scanner;

// ----------------------------------------------------------------------------
static bool isAtEnd() {
    return g_Scanner.current >= g_Scanner.source.length;
}

// ----------------------------------------------------------------------------
static int advance() {
    return g_Scanner.source.data[g_Scanner.current++];
}

// ----------------------------------------------------------------------------
static int peek() {
    if (isAtEnd()) {
        return '\0';
    }
    
    return g_Scanner.source.data[g_Scanner.current];
}

// ----------------------------------------------------------------------------
static int peekNext() {
    if (g_Scanner.current + 1 >= g_Scanner.source.length) {
        return '\0';
    }
    
    return g_Scanner.source.data[g_Scanner.current + 1];
}

// ----------------------------------------------------------------------------
static void scanString(Token* t) {
    // NOTE(alex): The -1 is used in scanIdentifier because when calling
    // scanString we are on " and when calling scanIdentifier we have
    // already advanced by one.
    // let; "asd"
    // 0123456789
    //  ^   ^
    //  1   2
    // 1) when calling scanIdentifier (current index too far ahead by one)
    // 2) when calling scanString (current index correct thanks to ")
    int begin = g_Scanner.current;
    
    while(peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            g_Scanner.line++;
        }
        advance();
    }
    
    if (isAtEnd()) {
        fprintf(stderr, "Unterminated string");
        exit(1);
    }
    
    int end = g_Scanner.current - 1;
    
    // Closing "
    advance();
    
    
    t->type = T_STRING;
    t->begin = begin;
    t->end = end;
}

// ----------------------------------------------------------------------------
// Conditional advance
static bool match(int expected) {
    if (isAtEnd()) {
        return false;
    }
    
    if (g_Scanner.source.data[g_Scanner.current] != expected) {
        return false;
    }
    
    g_Scanner.current++;
    return true;
}

// ----------------------------------------------------------------------------
static int charPos(char *s, int c) {
    char *p;
    
    p = strchr(s, c);
    return (p ? p - s : -1);
}

// ----------------------------------------------------------------------------
static bool isDigit(int c) {
    return c >= '0' && c <= '9';
}

// ----------------------------------------------------------------------------
static bool isAlpha(int c) {
    return
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c == '_');
}

// ----------------------------------------------------------------------------
static bool isAlphaNumeric(int c) {
    return isAlpha(c) || isDigit(c);
}

typedef struct {
    char* name;
    TokenType type;
} Keyword;

Keyword g_ReservedKeywords[] = {
    { "class", T_CLASS },
    { "const", T_CONST },
    { "else", T_ELSE },
    { "false", T_FALSE },
    { "for", T_FOR },
    { "if", T_IF },
    { "let", T_LET },
    { "null", T_NULL },
    { "undefined", T_UNDEFINED },
    { "return", T_RETURN },
    { "number", T_NUMBER },
    { "this", T_THIS },
    { "true", T_TRUE },
    { "var", T_VAR },
    { "while", T_WHILE },
};

// ----------------------------------------------------------------------------
static void scanInt(int c, Token* t) {
    int k = 0;
    int res = 0;
    
    while ((k = charPos("0123456789", c)) >= 0) {
        res = res * 10 + k;
        if (isDigit(peek())) {
            c = advance();
        } else {
            break;
        }
    }
    
    t->type = T_INTLIT;
    t->intValue = res;
}

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

// ----------------------------------------------------------------------------
static void scanIdentifier(int c, Token* t) {
    int begin = g_Scanner.current - 1;
    
    while (isAlphaNumeric(peek())) {
        advance();
    }
    
    int end = g_Scanner.current - 1;
    int length = end - begin + 1;
    
    // For now set as Identifier. Late check if it's a reserved keyword.
    t->begin = begin;
    t->end = end;
    t->type = T_IDENTIFIER;
    
    int reservedCount = COUNT_OF(g_ReservedKeywords);
    
    for (int i = 0; i < reservedCount; ++i) {
        int aLen = strlen(g_ReservedKeywords[i].name);
        int bLen = length;
        if (aLen != bLen) {
            continue;
        }
        bool found = true;
        for (int j = 0; j < aLen; ++j) {
            if (g_ReservedKeywords[i].name[j] != g_Scanner.source.data[j + begin]) {
                found = false;
                break;
            }
        }
        if (found) {
            t->type = g_ReservedKeywords[i].type;
        }
    }
}

// ----------------------------------------------------------------------------
// 1 valid token
// 0 eof
// -1 skip
static int scanToken(Token* t) {
    int c = advance();
    
    switch(c) { 
        case '\0': 
        case EOF: {
            t->type = T_EOF;
            return 0;
        }
        case '{': {
            t->type = T_OPEN_BRACE;
        } break;
        case '}': {
            t->type = T_CLOSE_BRACE;
        } break;
        case '(': {
            t->type = T_OPEN_PAREN;
        } break;
        case ')': {
            t->type = T_CLOSE_PAREN;
        } break;
        case '[': {
            t->type = T_OPEN_SQUARE_BRACE;
        } break;
        case ']': {
            t->type = T_CLOSE_SQUARE_BRACE;
        } break;
        case '+': {
            t->type = T_PLUS;
        } break;
        case '-': {
            t->type = T_MINUS;
        } break;
        case '*': {
            t->type = T_STAR;
        } break;
        case '/': {
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) {
                    advance();
                }
                return -1;
            } else {
                t->type = T_SLASH;
            }
        } break;
        case ',': {
            t->type = T_COMMA;
        } break;
        case '.': {
            t->type = T_DOT;
        } break;
        case ';': {
            t->type = T_SEMICOLON;
        } break;
        case ':': {
            t->type = T_COLON;
        } break;
        case '!': {
            t->type = match('=') ? T_BANG_EQUAL : T_BANG;
        } break;
        case '=': {
            t->type = match('=') ? T_EQUAL_EQUAL : T_EQUAL;
        } break;
        case '<': {
            t->type = match('=') ? T_LESS_EQUAL : T_LESS;
        } break;
        case '>': {
            t->type = match('=') ? T_GREATER_EQUAL : T_GREATER;
        } break;
        case ' ':
        case '\r':
        case '\t': {
            return -1;
        };
        case '\n': {
            g_Scanner.line++;
            return -1;
        } break;
        case '"': {
            scanString(t);
        } break;
        default: {
            if (isDigit(c)) {
                scanInt(c, t);
                break;
            } else if (isAlpha(c)) {
                scanIdentifier(c, t);
                break;
            }
            
            fprintf(stderr, "Unrecognised character '%c' on line: %d\n", c, g_Scanner.line);
            exit(1);
        }
    }
    
    return 1;
}

// ----------------------------------------------------------------------------
static printToken(Token* token) {
    char *tokstr[] = {
        "T_OPEN_BRACE",
        "T_CLOSE_BRACE",
        "T_OPEN_PAREN",
        "T_CLOSE_PAREN",
        "T_OPEN_SQUARE_BRACE",
        "T_CLOSE_SQUARE_BRACE",
        "T_PLUS",
        "T_MINUS",
        "T_STAR",
        "T_SLASH",
        "T_COMMA",
        "T_DOT",
        "T_SEMICOLON",
        "T_COLON",
        "T_BANG",
        "T_BANG_EQUAL",
        "T_EQUAL",
        "T_EQUAL_EQUAL",
        "T_GREATER",
        "T_GREATER_EQUAL",
        "T_LESS",
        "T_LESS_EQUAL",
        "T_IDENTIFIER",
        "T_STRING",
        "T_INTLIT",
        "T_CLASS",
        "T_CONST",
        "T_ELSE",
        "T_FALSE",
        "T_FOR",
        "T_IF",
        "T_LET",
        "T_NULL",
        "T_UNDEFINED",
        "T_RETURN",
        "T_NUMBER",
        "T_THIS",
        "T_TRUE",
        "T_VAR",
        "T_WHILE",
        "T_EOF",
        "T_COUNT"
    };
    
    printf("Token: %s", tokstr[token->type]);
    if (token->type == T_INTLIT) {
        printf(", value %d", token->intValue);
    }
    if (token->type == T_STRING) {
        printf(", value %.*s", token->end - token->begin + 1, &g_Scanner.source.data[token->begin]);
    }
    if (token->type == T_IDENTIFIER) {
        printf(", value %.*s", token->end - token->begin + 1, &g_Scanner.source.data[token->begin]);
    }
    printf("\n");
}