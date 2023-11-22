#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "cfg.h"

extern ASTNodes allNodes[maxCountOfNodesLists];
extern int filenumber;

void CFG_printToFileAndPNG(const char *dotFileName, const char *pngFileName) {
    char command[1024];
    sprintf(command, "dot -Tpng %s -o %s", dotFileName, pngFileName);
    int result = system(command);

    if (result != 0) {
        fprintf(stderr, "Error converting DOT to PNG.\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc > maxCountOfNodesLists) {
        printf("Max count of input files - 1024");
        return 1;
    }
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            FILE *input_file = fopen(argv[i], "r");
            if (input_file) {
                allNodes[filenumber] = createNodes();
                yyin = input_file;
                yyparse();
                fclose(input_file);
                filenumber++;
            } else {
                printf("Failed to open file: %s\n", argv[i]);
            }
        }

        ASTNodes allProcedures = findAllProcedures();
        CFG **cfgs = malloc(allProcedures.count * sizeof(CFG *));
        for (int i = 0; i < allProcedures.count; ++i) {
            preparedFunc func = prepareProcedure(allProcedures.nodes[i]);
            CFG *cfg = makeCFG(func, 0, i);
            cfgs[i] = cfg;
        }

        for (int i = 0; i < allProcedures.count; ++i) {
        char *dotFileName = malloc(strlen(cfgs[i]->procedureName) + 5);
        sprintf(dotFileName, "%s.dot", cfgs[i]->procedureName);
        FILE *dotFile = fopen(dotFileName, "w");

        if (dotFile == NULL) {
            fprintf(stderr, "Error opening DOT file for writing.\n");
            return 1;
        }

        fprintf(dotFile, "digraph G {\n");
        CFG_printInDOT(dotFile, cfgs[i], i, cfgs, allProcedures.count);
        fprintf(dotFile, "}\n");

        fclose(dotFile);

        char *pngFileName = malloc(strlen(cfgs[i]->procedureName) + 5);
        sprintf(pngFileName, "%s.png", cfgs[i]->procedureName);

        CFG_printToFileAndPNG(dotFileName, pngFileName);

        free(dotFileName);
        free(pngFileName);
    }

        freeNodes();

    } else {
        printf("Using: %s <input_file>\n", argv[0]);
        return 1;
    }
    return 0;
}
