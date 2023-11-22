#ifndef LAB2_CFG_H
#define LAB2_CFG_H

#include "ast.h"

typedef struct preparedFunc preparedFunc;

struct preparedFunc {
    char *identifier;
    ASTNode *body;
};

ASTNodes findAllProcedures();
preparedFunc prepareProcedure(ASTNode *procedure);

typedef struct CFG CFG;

void CFG_printInDOT(FILE *file, CFG *cfg, int index, CFG **allCFGs, int totalCFGs);
CFG *makeCFG(preparedFunc pf, int nextId, int num);

typedef struct Link Link;
typedef struct LinkList LinkList;
typedef struct Block Block;
typedef struct BlockList BlockList;
typedef struct CFGBuilder CFGBuilder;

struct Block {
    int id;
    char *call;
    LinkList *predecessors;
    LinkList *exits;
};

Block *NewBlock(int id, char *call);

struct BlockList {
    Block **blocks;
    int count;
};

BlockList *NewBlockList();

struct CFGBuilder {
    BlockList *after_loop_block_stack;
    BlockList *curr_loop_guard_stack;
    Block *current_block;
    BlockList *calls;
    int current_id;
    CFG *cfg;
};

void CFGBuilder_visit(CFGBuilder *cfgBuilder, ASTNode *node);

struct CFG {
    char *procedureName;
    Block *entryblock;
    BlockList *finalblocks;
    int nextId;
};

CFG *NewCFG(char *procedureName, Block *entryblock);

struct Link {
    Block *source;
    Block *target;
    char *comment;
};

Link *NewLink(Block *source, Block *target, char *string);

struct LinkList {
    Link **links;
    int count;
};

LinkList *NewLinkList();

#endif //LAB2_CFG_H
