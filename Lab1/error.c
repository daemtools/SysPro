#include "error.h"
#include <stdio.h>

void yyerror(const char* s){
    fprintf(stderr, "Ошибка: %s\n", s);
}
