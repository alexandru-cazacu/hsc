ASTNode* makeASTNode(ASTNodeType op, ASTNode* left, ASTNode* right, int intValue) {
    ASTNode* node;
    
    node = malloc(sizeof(ASTNode));
    
    node->op = op;
    node->left = left;
    node->right = right;
    node->intValue = intValue;
    
    return node;
}

ASTNode* makeASTLeaf(ASTNodeType op, int intValue) {
    return makeASTNode(op, NULL, NULL, intValue);
}

ASTNode* makeASTUnary(ASTNodeType op, ASTNode* left, int intValue) {
    return makeASTNode(op, left, NULL, intValue);
}

ASTNode* primary(void) {
    ASTNode* node;
    
    switch(g_Token.type) {
        case T_INTLIT: {
            node = makeASTLeaf(A_INTLIT, g_Token.intValue);
            scanToken(&g_Token);
            return node;
        }
        default: {
            fprintf(stderr, "Syntax error on line %d\n", g_Scanner.line);
            exit(1);
        }
    }
}

ASTNode* binaryExpression(void) {
    ASTNode* node;
    ASTNode* left;
    ASTNode* right;
    ASTNodeType nodeType;
    
    left = primary();
    
    if (g_Token.type == T_EOF) {
        return left;
    }
    
    nodeType = arithOp(g_Token.type);
    
    scanToken(&g_Token);
    
    right = binaryExpression();
    
    node = makeASTNode(nodeType, left, right, 0);
    
    return node;
}

int interpretAST(ASTNode* node) {
    int leftVal;
    int rightVal;
    
    if (node->left) {
        leftVal = interpretAST(node->left);
    }
    if (node->right) {
        rightVal = interpretAST(node->right);
    }
    
    switch(node->op) {
        case A_ADD: return leftVal + rightVal;
        case A_SUBTRACT: return leftVal - rightVal;
        case A_MULTIPLY: return leftVal * rightVal;
        case A_DIVIDE: return leftVal / rightVal;
        case A_INTLIT: return node->intValue;
        default: {
            fprintf(stderr, "Unknown AST Operator %d\n", node->op);
            exit(1);
        }
    }
    
    return 0;
}