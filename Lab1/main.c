#include "main.h"

extern ASTNode ** Nodes;
extern int NodesCount;

int main(int argc, char *argv[]) {
    Nodes = malloc(1024 * 8 * sizeof(ASTNode*));
    NodesCount = 0;
    if (argc > 1) {
        FILE *input_file = fopen(argv[1], "r");
        if (input_file) {
            yyin = input_file;
            yyparse();
            fclose(input_file);

            printASTInDOT("ast.dot", Nodes, NodesCount);
            translateDOTtoPNG("ast.dot", "graph.png");
            freeAST();

        } else {
            printf("Failed to open file: %s\n", argv[1]);
        }
    } else {
        printf("Using: %s <input_file>\n", argv[0]);
    }
    return 0;
}
