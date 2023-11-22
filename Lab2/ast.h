
#ifndef LAB2_AST_H
#define LAB2_AST_H

#define funcSignature "funcSignature"
#define sourceItem "sourceItem"
#define maxCountOfNodesLists 1024

typedef struct ASTNode ASTNode;
typedef struct ASTNodes ASTNodes;

struct ASTNodes {
    ASTNode **nodes;
    int count;
};

struct ASTNode {
    char *type;
    ASTNode *left;
    ASTNode *right;
    char *value;
    int id;
};

ASTNode *createNode(char *type, ASTNode *left, ASTNode *right, char *value);
ASTNodes createNodes();
void freeNodes();
void printAST();


#endif //LAB2_AST_H
