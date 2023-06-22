#include "scanner.h"

typedef struct {
    const char* start;   // Beginning of current lexeme
    const char* current; // Current character being looked at
	int line;
} Scanner;

Scanner gScanner;
char errorBuffer[256];

void initScanner(const char* source) {
    gScanner.start = source;
    gScanner.current = source;
    gScanner.line = 1;
}

static bool isAlpha(int c) {
    return
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c == '_');
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAtEnd() {
    return *gScanner.current == '\0';
}

int advance() {
    gScanner.current++;
    return gScanner.current[-1];
}

static char peek() {
    return *gScanner.current;
}

static int peekNext() {
    if (isAtEnd()) {
        return '\0';
    }
    
    return gScanner.current[1];
}


// Conditional advance
bool match(char expected) {
    if (isAtEnd()) {
        return false;
    }
    
    if (*gScanner.current != expected) {
        return false;
    }
    
    gScanner.current++;
    return true;
}

Token makeToken(TokenType type) {
    Token token = {0};
    token.type = type;
    token.start = gScanner.start;
    token.length = (int)(gScanner.current - gScanner.start);
    token.line = gScanner.line;
    return token;
}

Token errorToken(const char* message) {
    Token token = {0};
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = gScanner.line;
    return token;
}

static void skipWhitespace() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t': {
                advance();
            } break;
            case '\n': {
                gScanner.line++;
                advance();
            } break;
            case '/': {
                if (peekNext() == '/') {
                    // Comment goes until the end of the line.
                    while (peek() != '\n' && !isAtEnd()) {
                        advance();
                    }
                } else {
                    return;
                }
            } break;
            default: {
                return;
            }
        }
    }
}

static TokenType checkKeyword(int start, int length,
                              const char* rest, TokenType type) {
    if (gScanner.current - gScanner.start == start + length &&
        memcmp(gScanner.start + start, rest, length) == 0) {
        return type;
    }
    
    return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
    switch (gScanner.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f': {
            if (gScanner.current - gScanner.start > 1) {
                switch (gScanner.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
        }
        break;
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't': {
            if (gScanner.current - gScanner.start > 1) {
                switch (gScanner.start[1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
        }
        break;
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }
    
    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while (isAlpha(peek()) || isDigit(peek())) {
        advance();
    }
    
    return makeToken(identifierType());
}

static Token number() {
    while (isDigit(peek())) {
        advance();
    }
    
    // Look for a fractional part
    if (peek() == '.' && isDigit(peekNext())) {
        // Consumer the "."
        advance();
        
        while(isDigit(peek())) {
            advance();
        }
    }
    
    return makeToken(TOKEN_NUMBER);
}

static Token string() {
    while(peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            gScanner.line++;
        }
        advance();
    }
    
    if (isAtEnd()) {
        return errorToken("Unterminated string");
    }
    
    // The closing quote.
    advance();
    return makeToken(TOKEN_STRING);
}

Token scanToken() {
    skipWhitespace();
    gScanner.start = gScanner.current;
    
    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }
    
    char c = advance();
    
    if (isAlpha(c)) {
        return identifier();
    }
    if (isDigit(c)) {
        return number();
    }
    
    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
    }
    
    sprintf(errorBuffer, "Unexpected character '%c'.", gScanner.current[-1]);
    return errorToken(errorBuffer);
}
