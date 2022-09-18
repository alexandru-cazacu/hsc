#include "scan.h"

static Scanner g_Scanner;

static int next() {
    int c;
    
    if (g_Scanner.putback) {
        c = g_Scanner.putback;
        g_Scanner.putback = 0;
        return c;
    }
    
    c = g_Scanner.src[g_Scanner.index++];
    if ('\n' == c) {
        g_Scanner.line++;
    }
    
    return c;
}

static void putback(int c) {
    g_Scanner.putback = c;
}

static int skip() {
    int c;
    
    c = next();
    while(' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
        c = next();
    }
    
    return c;
}

static int chrpos(char *s, int c) {
    char *p;
    
    p = strchr(s, c);
    return (p ? p - s : -1);
}

static int scanInt(int c) {
    int k = 0;
    int val = 0;
    
    while ((k = chrpos("0123456789", c)) >= 0) {
        val = val * 10 + k;
        c = next();
    }
    
    putback(c);
    
    return val;
}

static bool scan(Token* t) {
    int c;
    
    c = skip();
    
    switch(c) { 
        case '\0': 
        case EOF: {
            t->type = T_EOF;
            return 0;
        }
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
            t->type = T_SLASH;
        } break;
        default: {
            if (isdigit(c)) {
                t->intValue = scanInt(c);
                t->type = T_INTLIT;
                break;
            }
            
            printf("Unrecognised character %c on line: %d\n", c, g_Scanner.line);
            exit(1);
        }
    }
    
    return true;
}