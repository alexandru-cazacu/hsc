static int scannerIndex = 0;
int g_Putback = 0;
int line = 0;
const char src[] = "3 * 1 + 2\n";

local int next() {
    int c;
    
    if (g_Putback) {
        c = g_Putback;
        g_Putback = 0;
        return c;
    }
    
    c = src[scannerIndex++];
    if ('\n' == c) {
        line++;
    }
    
    return c;
}

local void putback(int c) {
    g_Putback = c;
}

local int skip() {
    int c;
    
    c = next();
    while(' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
        c = next();
    }
    
    return c;
}

local int chrpos(char *s, int c) {
    char *p;
    
    p = strchr(s, c);
    return (p ? p - s : -1);
}

local int scanInt(int c) {
    int k = 0;
    int val = 0;
    
    while ((k = chrpos("0123456789", c)) >= 0) {
        val = val * 10 + k;
        c = next();
    }
    
    putback(c);
    
    return val;
}

local bool scan(Token* t) {
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
            
            printf("Unrecognised character %c on line: %d\n", c, line);
            exit(1);
        }
    }
    
    return true;
}