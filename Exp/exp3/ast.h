#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "parser.tab.h"

typedef struct _ast ast;
typedef struct _ast *past;
struct _ast
{
    union
    {
        int ivalue;
        char *svalue;
    } value;
    char *nodeType;
    int flag;
    past left;
    past right;
    past next;
    past tail;
};
int tok;
past newExpr(int oper, past left, past right, past next);
past newIdent(char *strVal, past next);
past newType(int type);
past newNum(int value, past next);
past newAstNode_param(char *strVal, char *nodeType, past left, past right, past next);
past newAstNode();
void showAst(past node, int nest);
void freeAst(past node);
