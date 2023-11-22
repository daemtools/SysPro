%{
#include "grammar.tab.h"
#include "error.h"

#define YYDEBUG 1
%}
%error-verbose

%union {
    ASTNode *node;
}

%token <node> PLUS MINUS STAR SLASH PERCENT EQUAL NOTEQUAL
%token <node> LESSTHAN GREATERTHAN LESSTHANEQ GREATERTHANEQ
%token <node> AND OR NOT
%token <node> IDENTIFIER
%token <node> STR
%token <node> COMMA
%token <node> CHAR
%token <node> BIN HEX DEC
%token <node> TRUE FALSE
%token <node> IF ELSE WHILE DO BREAK
%token <node> SEMICOLON
%token <node> LPAREN RPAREN LBRACE RBRACE
%token <node> TYPEDEF
%token <node> DIM LOOP THEN UNTIL WEND
%token <node> ARRAY_COMMAS

%left OR
%left AND
%left NOT
%left PLUS MINUS
%left STAR SLASH PERCENT
%nonassoc EQUAL NOTEQUAL LESSTHAN GREATERTHAN LESSTHANEQ GREATERTHANEQ

%type <node> expr
%type <node> typeRef
%type <node> funcSignature
%type <node> argDef
%type <node> sourceItem
%type <node> statement
%type <node> var
%type <node> if
%type <node> while
%type <node> do
%type <node> break
%type <node> expression
%type <node> builtin
%type <node> custom
%type <node> array
%type <node> source
%type <node> listArgDef
%type <node> optionalTypeRef
%type <node> literal
%type <node> place
%type <node> listExpr
%type <node> optionalListExpr
%type <node> call
%type <node> braces
%type <node> unary
%type <node> binary
%type <node> assignment
%type <node> listVarDeclared
%type <node> optionalStatements
%type <node> listStatement
%type <node> listVarDeclaredItem


%%
source: {{$$ = NULL;}}
    | source sourceItem {{$$ = createNode("source", $1, $2, "");}};

sourceItem: funcSignature listStatement {{$$ = createNode("sourceItem", $1, $2, "");}};

/* FUNCSIGNATURE */

funcSignature: argDef LPAREN listArgDef RPAREN {{$$ = createNode("funcSignature", $1, $3, "");}};

listArgDef:  {{$$ = NULL;}}
    | argDef {{$$ = $1;}}
    | argDef COMMA listArgDef {{$$ = createNode("listArgDef", $1, $3, "");}};

argDef: optionalTypeRef IDENTIFIER {{$$ = createNode("argDef", $1, $2, "");}};

optionalTypeRef: typeRef {{$$ = $1;}};

/* TypeRef */
typeRef: builtin {{$$ = $1;}}
    | custom {{$$ = $1;}}
    | array {{$$ = $1;}};

builtin: TYPEDEF {{$$ = $1;}};

/* Statement */
statement: var {{$$ =  $1;}}
    | if {{$$ =  $1;}}
    | while {{$$ =  $1;}}
    | do {{$$ =  $1;}}
    | break {{$$ =  $1;}}
    | expression {{$$ =  $1;}};

custom: IDENTIFIER {{$$ = $1;}};

array: typeRef ARRAY_COMMAS {{$$ = createNode("array", $1, NULL, $2->value);}};

listVarDeclared: listVarDeclaredItem COMMA listVarDeclared {{$$ = createNode("listVarDeclared", $1, $3, "");}}
    | listVarDeclaredItem {{$$ = createNode("listVarDeclared", $1, NULL, "");}};

listVarDeclaredItem: IDENTIFIER {{$$ = $1;}}
    | assignment {{$$ = $1;}};

var: DIM listVarDeclared typeRef {{$$ = createNode("var", $2, $3, "");}};

if: IF expr statement optionalStatements {{$$ = createNode("if", $2, createNode("ifStatements", $3, $4, ""), "");}};

optionalStatements: THEN statement optionalStatements {{$$ = createNode("then", $2, $3, "");}}
    | THEN statement {{$$ = createNode("then", $2, NULL, "");}}
    | ELSE statement optionalStatements {{$$ = createNode("else", $2, $3, "");}}
    | ELSE statement {{$$ = createNode("else", $2, NULL, "");}}
    | {{$$ = NULL;}};

listStatement: statement listStatement {{$$ = createNode("listStatement", $1, $2, "");}}
    | {{$$ = NULL;}};

while: WHILE expr listStatement WEND {{$$ = createNode("while", $2, $3, "");}};

do: DO listStatement LOOP WHILE expr {{$$ = createNode("dowhile", $2, $5, "");}}
  | DO listStatement LOOP UNTIL expr {{$$ = createNode("dowhile", $2, $5, "");}};

break: BREAK {{$$ = createNode("break", NULL, NULL, "");}};

expression: expr SEMICOLON {{$$ = $1;}};

assignment: expr EQUAL expr {{$$ = createNode("assigment", $1, $3, "");}};

expr: binary    {{$$ = $1;}}
    | unary   {{$$ = $1;}}
    | braces   {{$$ = $1;}}
    | call     {{$$ = $1;}}
    | place    {{$$ = $1;}}
    | literal  {{$$ = $1;}};

binary: assignment
    | expr PLUS expr {{$$ = createNode("PLUS", $1, $3, "");}}
    | expr MINUS expr {{$$ = createNode("MINUS", $1, $3, "");}}
    | expr STAR expr {{$$ = createNode("STAR", $1, $3, "");}}
    | expr SLASH expr {{$$ = createNode("SLASH", $1, $3, "");}}
    | expr PERCENT expr {{$$ = createNode("PERCENT", $1, $3, "");}}
    | expr EQUAL EQUAL expr {{$$ = createNode("EQUALITY", $1, $4, "");}}
    | expr NOTEQUAL expr {{$$ = createNode("NOTEQUAL", $1, $3, "");}}
    | expr LESSTHAN expr {{$$ = createNode("LESSTHAN", $1, $3, "");}}
    | expr GREATERTHAN expr {{$$ = createNode("GREATERTHAN", $1, $3, "");}}
    | expr LESSTHANEQ expr {{$$ = createNode("LESSTHANEQ", $1, $3, "");}}
    | expr GREATERTHANEQ expr {{$$ = createNode("GREATERTHANEQ", $1, $3, "");}}
    | expr AND expr {{$$ = createNode("AND", $1, $3, "");}}
    | expr OR expr {{$$ = createNode("OR", $1, $3, "");}};

unary: PLUS expr {{$$ = createNode("PLUS", $2, NULL, "");}}
    | MINUS expr {{$$ = createNode("MINUS", $2, NULL, "");}}
    | NOT expr {{$$ = createNode("NOT", $2, NULL, "");}};

braces: LPAREN expr RPAREN  {{$$ = createNode("braces", $2, NULL, "");}};

call: IDENTIFIER LPAREN optionalListExpr RPAREN  {{$$ = createNode("CALL", $1, $3, "");}};

optionalListExpr: listExpr  {{$$ = createNode("optionalListExpr", $1, NULL, "");}}
    | {{$$ = createNode("optionalListExpr", NULL, NULL, "");}};

listExpr: expr COMMA listExpr {{$$ = createNode("listExpr", $1, $3, "");}}
    | expr {{$$ = createNode("listExpr", $1, NULL, "");}};

place: IDENTIFIER {{$$ = $1;}};

literal: TRUE {{$$ = $1;}}
    | FALSE {{$$ = $1;}}
    | STR {{$$ = $1;}}
    | CHAR {{$$ = $1;}}
    | HEX {{$$ = $1;}}
    | BIN {{$$ = $1;}}
    | DEC {{$$ = $1;}};
%%
