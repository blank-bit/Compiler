/*********************************
 * @Author: wang
 * @LastEditTime: 2021-12-02 16:01:34
 * @FilePath: /Compiler/Exp/exp3/semantic.c
 * @Description: 语义分析
 *********************************/

#include "ast.h"

#define max_prime 7

extern pscopeInfo scopeStack;
extern int idCount;

int first = 1;
int slevel = 0;

int getHashID(int idcount)
{
    return (idcount % max_prime);
}

void initSymboltbl()
{
    for (int i = 0; i < MAXLENGTH; i++)
    {
        symbolTable[i].next = NULL;
    }
}

void showScopeStacke()
{
    pscopeInfo s = scopeStack->next;
    while (s)
    {
        printf("id:%d,name:%s\n", s->id, s->name);
        s = s->next;
    }
}

int beginScope()
{
    if (first)
    {
        newScopeStack();
        initSymboltbl();
        first--;
    }
    pscopeInfo s;
    s = (pscopeInfo)malloc(sizeof(scopeInfo));
    s->id = -1;
    s->name = "NULL";
    s->scopeLevel = -1;
    s->flag = '#';
    s->next = scopeStack->next;
    scopeStack->next = s;
    // printf("\nbeginscope:%d\n", slevel);
    // showScopeStacke();
    return 1;
}

int endScope()
{
    pscopeInfo s;
    s = scopeStack->next;
    if (s)
    {
        while (s->id != -1)
        {
            scopeStack->next = s->next;
            free(s);
            s = scopeStack->next;
        }
        if (s->id == -1)
        {
            scopeStack->next = s->next;
            free(s);
            s = scopeStack->next;
            // printf("\nendscope:%d\n", slevel);
            // showScopeStacke();
            return 1;
        }
    }
    return 0;
}

pscopeInfo newScopeInfo()
{
    pscopeInfo s;
    s = (pscopeInfo)malloc(sizeof(scopeInfo));
    s->id = -1;
    s->name = "NULL";
    s->scopeLevel = -1;
    s->flag = '#';
    return s;
}

void freeScope(pscopeInfo s)
{
    if (s == NULL)
        return;
    freeScope(s->next);
    free(s);
}

void newScopeStack()
{
    scopeStack = (pscopeInfo)malloc(sizeof(scopeInfo));
    scopeStack->id = -9;
    scopeStack->next = NULL;
    scopeStack->name = "NULL";
    scopeStack->flag = '#';
    scopeStack->scopeLevel = -9;
}

int semantic_Analysis(past p)
{
    if (first)
    {
        newScopeStack();
        initSymboltbl();
        first--;
    }

    if (p == NULL)
        return 1;
    if (p->flag != '#')
    {
        // printf("\nlevel:%d, %s, %d\n", slevel, p->value.svalue, p->level);
        if (p->level > slevel)
        {
            while (p->level != slevel)
            {
                slevel++;
                beginScope();
            }
        }
        else if (p->level < slevel)
        {
            while (p->level != slevel)
            {
                slevel--;
                endScope();
            }
        }

        if (isExit(p))
        {
            if (p->flag == 'T' || p->flag == 'Y' || p->flag == 'R')
            {
            }
            else
            {
                pushStack(p->value.svalue, p->level, idCount, p->flag);
                if (p->flag == 'F')
                    fillSymtbl(p->value.svalue, p->level, p->type, p->flag);
                else
                    fillSymtbl(p->value.svalue, p->level, 1, p->flag);
                idCount++;
            }
        }
        else
        {
            return 0;
        }

        if (p->flag == 'F' && !strcmp(p->next->nodeType, "FuncFParams"))
        {
            beginScope();
            slevel++;
            if (semantic_FP(p))
            {
                semantic_Analysis(p->left);
                semantic_Analysis(p->right);
                return 1;
            }
            else
                return 0;
        }
    }

    if (!semantic_Analysis(p->left))
        ;
    // return 0;
    if (!semantic_Analysis(p->right))
        ;
    // return 0;
    if (!semantic_Analysis(p->next))
        ;
    // return 0;
}

// 1:正确 0:错误
int isExit(past p)
{
    pscopeInfo s;
    s = scopeStack->next;
    while (s)
    {
        if (!strcmp(s->name, p->value.svalue))
        {
            switch (p->flag)
            {
            case 'F':
                printf("Error:in line %d redefine function: %s\n", p->line, p->value.svalue);
                return 0;
            case 'V':
                if (s->scopeLevel < p->level)
                    return 1;
                else
                {
                    printf("Error:in line %d redefine variable: %s\n", p->line, p->value.svalue);
                    return 0;
                }
            case 'P':
                printf("Error:in line %d symbol: %s\n", p->line, p->value.svalue);
                return 0;
            case 'T':
                if (s->flag == 'V' || s->flag == 'P')
                {
                    return 1;
                }
                else
                {
                    printf("Error:in line %d undefine variable: %s\n", p->line, p->value.svalue);
                    return 0;
                }
            case 'A':
                printf("Error:in line %d define array conflict: %s\n", p->line, p->value.svalue);
                return 0;
            case 'Y':
                if (s->flag == 'A' || s->flag == 'P')
                {
                    return 1;
                }
                else
                {
                    printf("Error:in line %d undefine array: %s\n", p->line, p->value.svalue);
                    return 0;
                }
            case 'R':
                if (s->flag != 'F')
                {
                    printf("Error:in line %d undefine func: %s\n", p->line, p->value.svalue);
                    return 0;
                }
                return 1;
            default:
                return 0;
            }
        }
        s = s->next;
    }
    if (p->flag == 'V' || p->flag == 'F' || p->flag == 'A' || p->flag == 'R') //若允许未定义函数或库函数存在，取消注释
                                                                              // if (p->flag == 'V' || p->flag == 'F' || p->flag == 'A')
        return 1;
    else
    {
        printf("ERROR:in line %d undefine symbol %s\n", p->line, p->value.svalue);
        return 0;
    }
}

// 0:error 1:right
int semantic_FP(past p)
{
    if (!strcmp(p->next->nodeType, "FuncFParams"))
    {
        past s = p->next->left;
        pushStack("NULL", -2, -2, '#');
        while (s)
        {
            if (s->right->flag == 'P')
            {
                pscopeInfo ps = scopeStack->next;
                while (ps->id != -2)
                {
                    if (!strcmp(ps->name, s->right->value.svalue))
                    {
                        printf("ERROR:in line %d parameter %s conflict!\n", p->line, s->right->value.svalue);
                        return 0;
                    }
                    ps = ps->next;
                }
                pushStack(s->right->value.svalue, s->right->level, idCount, s->right->flag);
                fillSymtbl(s->right->value.svalue, s->right->level, 1, s->right->flag);
                idCount++;
            }
            s = s->next;
        }
        // printf("\nfp\n");
        // showScopeStacke();
        return 1;
    }

    return 0;
}

void fillSymtbl(char *name, int level, int type, char flag)
{
    symbol *sym;
    sym = (symbol *)malloc(sizeof(symbol));

    sym->name = name;
    sym->scopeLevel = level;
    sym->type = type;
    sym->flag = flag;
    sym->next = NULL;

    int index = getHashID(idCount);
    if (symbolTable[index].next == NULL)
    {
        symbolTable[index].next = sym;
    }
    else
    {
        sym->next = symbolTable[index].next;
        symbolTable[index].next = sym;
    }
}

void pushStack(char *name, int level, int id, char flag)
{
    pscopeInfo s = newScopeInfo();
    s->id = id;
    s->name = name;
    s->scopeLevel = level;
    s->next = scopeStack->next;
    s->flag = flag;
    scopeStack->next = s;
    // printf("\nputstack\n");
    // showScopeStacke();
}

void showsymbotbl()
{
    symbol *s;
    for (int i = 0; i < MAXLENGTH; i++)
    {
        s = symbolTable[i].next;
        while (s != NULL)
        {
            printf("name:%-6s\t\ttype:%s\t\tLEVEL:%d\n", s->name, (s->type == 1) ? "INT" : "VOID", s->scopeLevel);
            s = s->next;
        }
    }
}