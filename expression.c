ASTNodeType arithOp(TokenType t) {
    switch(t) {
        case T_PLUS: return A_ADD;
        case T_MINUS: return A_SUBTRACT;
        case T_STAR: return A_MULTIPLY;
        case T_SLASH: return A_DIVIDE;
        default: {
            fprintf(stderr, "Unknown token in arithop() on line %d\n", g_Scanner.line);
            exit(1);
        }
    }
}
