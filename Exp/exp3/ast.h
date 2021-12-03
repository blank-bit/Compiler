/*********************************
 * @Author: wang
 * @LastEditTime: 2021-12-01 14:57:00
 * @FilePath: /Compiler/Exp/exp3/ast.h
 * @Description:
 *********************************/
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "lrparser.tab.h"

#define MAXLENGTH 10

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
    int flag_f;
    int level;
    char flag; //符号标记，函数：'F'  变量定义：'V'   参数：'P'  变量引用：'T' 数组定义：'A' 数组引用：'Y' 函数引用：'R'
    int type;
    int line;
    past left;
    past right;
    past next;
    past tail;
};

typedef struct _symbol
{
    char *name;     //变量或函数名
    int scopeLevel; //层号
    enum
    {
        T_INT = 1,
        T_VOID
    } type;    //变量类型或函数返回值类型
    char flag; //符号标记，函数：'F'  变量定义：'V'   参数：'P'  变量引用：'T' 数组定义：'A' 数组引用：'Y' 函数引用：'R'
    char *hashName;
    struct _symbol *next;
    struct _symbol *pre;
} symbol;

//符号表
struct _symbol symbolTable[MAXLENGTH];

//栈数据结构
typedef struct _scopeInfo *pscopeInfo;
struct _scopeInfo
{
    char *name;
    int id;
    char flag;
    int scopeLevel;
    pscopeInfo next;
} scopeInfo;

pscopeInfo scopeStack;

int tok;
int idCount;
past newExpr(int oper, past left, past right, past next);
past newIdent(char *strVal, past next);
past newType(int type);
past newNum(int value, past next);
past newAstNode_param(char *strVal, char *nodeType, past left, past right, past next);
past newAstNode();
void showAst(past node, int nest);
void freeAst(past node);
void yyerror(char *s);

void initSymboltbl();
int getHashID(int idcount);
int beginScope();
int endScope();
void newScopeStack();
int semantic_Analysis(past p);
int isExit(past p);
int semantic_FP(past p);
pscopeInfo newScopeInfo();
void fillSymtbl(char *name, int level, int type, char flag);
void pushStack(char *name, int level, int id, char flag);
void showsymbotbl();