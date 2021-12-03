/*********************************
 * @Author: wang
 * @LastEditTime: 2021-12-01 14:54:29
 * @FilePath: /Compiler/Exp/exp3/main.c
 * @Description:
 *********************************/
#include "ast.h"

extern int yylineno;
extern char *yytext;
FILE *yyin;

int main(int argc, char *argv[])
{
    yyin = fopen(argv[1], "r");
    if (!yyin)
        return 0;
    yyparse();
    return 0;
}

void yyerror(char *s)
{
    char *s0 = strdup(yytext);
    int len = strlen(s0);
    int i;
    char buf[512] = {0};
    fprintf(stderr, "ERROR: %s at symbol '%s' on line %d\n", s, s0, yylineno);
}
