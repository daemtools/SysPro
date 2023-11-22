.PHONY: all clean
all: result

clean:
	rm -rf *.o result

lex.yy.c: lexer.l
	flex lexer.l

error.o: error.c
	gcc -c -o error.o error.c

cfg.o: cfg.c
	gcc -c -o cfg.o cfg.c

grammar.tab.c: grammar.y
	bison -d -t grammar.y
	echo '#include "ast.h"' | cat - grammar.tab.h > temp && mv temp grammar.tab.h

main.o: main.c
	gcc -c -o main.o main.c

ast.o: ast.c
	gcc -c -o ast.o ast.c

lex.yy.o: lex.yy.c
	gcc -c -o lex.yy.o lex.yy.c

grammar.tab.o: grammar.tab.c
	gcc -c -o grammar.tab.o grammar.tab.c

result: ast.o grammar.tab.o lex.yy.o main.o error.o cfg.o
	gcc main.o grammar.tab.o lex.yy.o ast.o error.o cfg.o -o result && chmod +x result

run: result
	./result input1.txt input2.txt
