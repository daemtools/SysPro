#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

ASTNode **Nodes;
int NodesCount;

ASTNode *createNode(char *type, ASTNode *left, ASTNode *right, char *value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->left = left;
    node->right = right;
    char *buf= malloc(1024* sizeof(char)) ;
    strcpy(buf, value);
    node->value = buf;
    Nodes[NodesCount] = node;
    NodesCount++;
    return node;
}
void printNodeId(ASTNode *node) {
    printf("\"Type: %s, Id: %d", node->type, node->id);
    if (strlen(node->value)>0){
        printf(", Value: %s", node->value);
    }
    printf("\"");
}

void printNodeInDOT(FILE *dotFile, ASTNode *node) {
    if (node == NULL) {
        return;
    }

    fprintf(dotFile, "node%d [label=\"%s", node->id, node->type);

    if (strlen(node->value) > 0) {
        fprintf(dotFile, "\\nValue: %s", node->value);
    }

    fprintf(dotFile, "\"];\n");

    if (node->left) {
        fprintf(dotFile, "node%d -> node%d [label=\"left\"];\n", node->id, node->left->id);
        printNodeInDOT(dotFile, node->left);
    }

    if (node->right) {
        fprintf(dotFile, "node%d -> node%d [label=\"right\"];\n", node->id, node->right->id);
        printNodeInDOT(dotFile, node->right);
    }
}

void printASTInDOT(const char *dotFileName, ASTNode **Nodes, int NodesCount) {
    FILE *dotFile = fopen(dotFileName, "w");

    if (dotFile == NULL) {
        fprintf(stderr, "Error opening DOT file for writing.\n");
        return;
    }

    fprintf(dotFile, "digraph AST {\n");

    for (int i = 0; i < NodesCount; i++) {
        Nodes[i]->id = i;
    }

    printNodeInDOT(dotFile, Nodes[NodesCount - 1]);

    fprintf(dotFile, "}\n");

    fclose(dotFile);
}

void translateDOTtoPNG(const char *dotFileName, const char *pngFileName) {
    // Translate DOT to PNG
    char command[256];
    snprintf(command, sizeof(command), "dot -Tpng %s -o %s", dotFileName, pngFileName);
    int result = system(command);

    if (result == 0) {
        // Translation succeeded, open the PNG file
        snprintf(command, sizeof(command), "open %s", pngFileName);
        system(command);
    } else {
        // Translation failed, handle the error
        printf("Failed to generate PNG file from DOT.\n");
    }
}

void freeASTNode(ASTNode *node) {
    if (node == NULL) {
        return;
    }

    freeASTNode(node->left);
    freeASTNode(node->right);

    free(node->value);
    free(node);
}

void freeAST() {
    if (Nodes != NULL) {
        for (int i = 0; Nodes[i] != NULL; i++) {
            freeASTNode(Nodes[i]);
        }
        free(Nodes);
    }
}
