#include "compiler.h"
#include "scanner.h"

#if 0
static void advance() {
    gParser.previous = gParser.current;
    
    while (true) {
        gParser.current = scanToken();
        if (gParser.current.type != TOKEN_ERROR) {
            break;
        }
        
        errorAtCurrent(gParser.current.start);
    }
}
#endif

typedef struct {
    Token left;
    Token right;
} Declaration;

typedef struct {
    int vSync;
    bool startMaximized;
} Config;

#define MAX_CONFIG 24
uint32_t config_count = 0;
Declaration configs[MAX_CONFIG];

static int charPos(char *s, int c) {
    char *p;
    
    p = strchr(s, c);
    return (p ? p - s : -1);
}

static int scanInt(const char* start, int length) {
    int k = 0;
    int res = 0;
    int index = 0;
    char c = start[index];
    
    while ((k = charPos("0123456789", c)) >= 0) {
        res = res * 10 + k;
        if (isDigit(start[index+1])) {
            c = start[++index];
        } else {
            break;
        }
    }
    
    return res;
}

bool compile(const char* source, Chunk* chunk) {
    initScanner(source);
#if 0
    advance();
    expression();
    consume(TOKEN_EOF, "Expected end of expression.");
    
    return false;
#endif
    
    int line = -1;
    while (true) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        Token left = token;
        if (token.type == TOKEN_IDENTIFIER) {
            token = scanToken();
            if (token.type == TOKEN_EQUAL) {
                token = scanToken();
                if (token.type == TOKEN_NUMBER ||
                    token.type == TOKEN_TRUE ||
                    token.type == TOKEN_FALSE ||
                    token.type == TOKEN_STRING) {
                    printf("Done\n");
                    Declaration decl = {0};
                    decl.left = left;
                    decl.right = token;
                    configs[config_count++] = decl;
                } else {
                    printf("Expected string, boolean or number, but found: '%.*s'\n", token.length, token.start);
                }
            } else {
                printf("Expected '=', but found: '%.*s'\n", token.length, token.start);
            }
        } else {
            printf("Expected a declaration, but found: '%.*s'\n", token.length, token.start);
            return false;
        }
    }
    
    Config config;
    
    for (int i = 0; i < config_count; i++) {
        Token* left = &configs[i].left;
        Token* right = &configs[i].right;
        
        if (strncmp(left->start, "startMaximized", left->length) == 0) {
            if (right->type == TOKEN_TRUE || right->type == TOKEN_FALSE) {
                config.startMaximized = right->type == TOKEN_TRUE;
            }
        }
        if (strncmp(left->start, "vSync", left->length) == 0) {
            if (right->type == TOKEN_NUMBER) {
                // TODO(alex): We parse numbers as floats but convert them to ints.
                config.vSync = scanInt(right->start, right->length);
            }
        }
    }
    
    return false;
}