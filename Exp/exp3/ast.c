#include "ast.h"

past newAstNode()
{
    past node = (past)malloc(sizeof(ast));
    if (node == NULL)
    {
        printf("run out of memory.\n");
        exit(0);
    }
    memset(node, 0, sizeof(ast));
    return node;
}

past newAstNode_param(char *strVal, char *nodeType, past left, past right, past next)
{
    past node = (past)malloc(sizeof(ast));
    if (node == NULL)
    {
        printf("run out of memory.\n");
        exit(0);
    }
    memset(node, 0, sizeof(ast));
    node->flag = 0;
    node->value.svalue = strVal;
    node->nodeType = nodeType;
    node->left = left;
    node->right = right;
    node->next = next;
    node->tail = NULL;
    return node;
}

past newNum(int value, past next)
{
    past var = newAstNode();
    var->nodeType = "IntValue";
    var->flag = 0;
    var->value.ivalue = value;
    var->next = next;
    var->tail = NULL;
    return var;
}

past newType(int type)
{
    past var = newAstNode();
    var->nodeType = "Type";
    switch (type)
    {
    case VOID:
        var->value.svalue = "VOID";
        break;
    case INT:
        var->value.svalue = "INT";
        break;
    case CONST:
        var->value.svalue = "CONST";
        break;
    default:
        break;
    }
    var->flag = 0;
    var->left = NULL;
    var->right = NULL;
    var->next = NULL;
    var->tail = NULL;
}

past newIdent(char *IDN, past next)
{
    past var = newAstNode();
    var->nodeType = "Variable";
    var->flag = 0;
    var->value.svalue = IDN;
    var->next = next;
    var->tail = NULL;
    return var;
}

past newExpr(int oper, past left, past right, past next)
{
    past var = newAstNode();
    switch (oper)
    {
    case LE:
        var->value.svalue = "<=";
        break;
    case GE:
        var->value.svalue = ">=";
        break;
    case NE:
        var->value.svalue = "!=";
        break;
    case EQ:
        var->value.svalue = "==";
        break;
    case AND:
        var->value.svalue = "&&";
        break;
    case OR:
        var->value.svalue = "||";
        break;
    case ADD:
        var->value.svalue = "+";
        break;
    case SUB:
        var->value.svalue = "-";
        break;
    case MUL:
        var->value.svalue = "*";
        break;
    case DIV:
        var->value.svalue = "/";
        break;
    case MOD:
        var->value.svalue = "%";
        break;
    case LT:
        var->value.svalue = "<";
        break;
    case GT:
        var->value.svalue = ">";
        break;
    case NOT:
        var->value.svalue = "!";
        break;
    default:
        var->value.svalue = "@";
        break;
    }
    var->nodeType = "Expr";
    var->flag = 0;
    var->left = left;
    var->right = right;
    var->next = next;
    var->tail = NULL;
    return var;
}

//输出语法树
void showAst(past node, int nest)
{
    if (node == NULL)
        return;
    int i = 0;
    for (i = 0; i < nest; i++)
        printf("  ");
    if (!strcmp(node->nodeType, "FuncName"))
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (!strcmp(node->nodeType, "ElseStmt"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "ArrayIndex"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "IntValue"))
        printf("%s : %d\n", node->nodeType, node->value.ivalue);
    else if (!strcmp(node->nodeType, "ConstDecl"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "ConstDef"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "VarDef"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "ConstInitVal"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "InitVal"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "FuncDef"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "FuncRefer"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "FuncFParams"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "FuncFParam"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "EmptyStmt"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "CompUnit"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "Expr") || !strcmp(node->nodeType, "PreExpr"))
    {
        if (!strcmp(node->value.svalue, "@"))
            printf("%s : '%c'\n", node->nodeType, '@');
        else
            printf("%s : %s\n", node->nodeType, node->value.svalue);
    }
    else if (!strcmp(node->nodeType, "Count"))
        printf("%s : %d\n", node->nodeType, node->value.ivalue);
    else if (!strcmp(node->nodeType, "Variable"))
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (!strcmp(node->nodeType, "VarDecl"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "ArrayID"))
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (!strcmp(node->nodeType, "Block"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "FuncRParams"))
        printf("%s\n", node->nodeType);
    else if (!strcmp(node->nodeType, "FuncRParam"))
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (!strcmp(node->nodeType, "Type"))
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (!strcmp(node->nodeType, "AssignStmt") || !strcmp(node->nodeType, "IfStmt") || !strcmp(node->nodeType, "WhileStmt") || !strcmp(node->nodeType, "BreakStmt") || !strcmp(node->nodeType, "ContinueStmt") || !strcmp(node->nodeType, "ReturnStmt") || !strcmp(node->nodeType, "EmptyStmt") || !strcmp(node->nodeType, "expStmt"))
        printf("%s\n", node->nodeType);

    showAst(node->left, nest + 1);
    showAst(node->right, nest + 1);
    showAst(node->next, nest);
}

//释放结点
void freeAst(past node)
{
    if (node == NULL)
        return;
    freeAst(node->left);
    freeAst(node->right);
    freeAst(node->next);
    free(node);
}