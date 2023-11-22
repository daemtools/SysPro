#ifndef LAB1_AST_H
#define LAB1_AST_H

typedef struct ASTNode ASTNode;

struct ASTNode {
    char *type;
    ASTNode *left;
    ASTNode *right;
    char *value;
    int id;
};

void printASTInDOT();
void translateDOTtoPNG();
void freeASTNode();
void freeAST();
void printAST();

ASTNode *createNode(char *type, ASTNode *left, ASTNode *right, char *value);

#endif //LAB1_AST_H
