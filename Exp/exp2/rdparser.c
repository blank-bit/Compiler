#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype
{
    INT = 1,
    RETURN,
    VOID,
    CONST,
    IF,
    ELSE,
    CONTINUE,
    WHILE,
    BREAK,
    IDN,
    NUM,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    LT,
    GT,
    LE,
    GE,
    ASSIGN,
    NE,
    EQ,
    OR,
    AND,
    NOT,
    LP,
    RP,
    SLP,
    SRP,
    MLP,
    MRP,
    COMMA,
    SEMI,
    COMMENT,
    COMMENTS
};

extern int yylex();
extern int yylval;
extern char *yytext;
extern FILE *yyin;
extern void put_back(char *yytext);

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
    past left;
    past right;
    past next;
};

void showAst(past node, int nest);
past newExpr(int oper, past left, past right, past next);
past newIdent(char *strVal, past next);
past newType(int type);
past newNum(int value, past next);
past newAstNode_param(char *strVal, char *nodeType, past left, past right, past next);
past newAstNode();
past CompUnit();
past Decl();
past ConstDecl();
past BType();
past ConstDef();
past ConstInitVal();
past VarDecl();
past VarDef();
past InitVal();
past FuncDef();
past FuncType();
past FuncFParams();
past FuncFParam();
past Block();
past BlockItem();
past Stmt();
past Exp();
past Cond();
past LVal();
past PrimaryExp();
past Number();
past UnaryExp();
past UnaryOp();
past FuncRParams();
past MulExp();
past AddExp();
past RelExp();
past EqExp();
past LAndExp();
past LOrExp();
past ConstExp();

int tok;

void advance()
{
    tok = yylex();
    if (tok == 0)
        return;
    printf("tok: %s\n", yytext);
}

void error(void)
{
    printf("The match failed, please check\n");
    exit(0);
}

past newAstNode(void)
{
    past node = malloc(sizeof(ast));
    if (node == NULL)
    {
        printf("run out of memory.\n");
        exit(0);
    }
    memset(node, 0, sizeof(ast));
    return node;
}

void freeAst(past node)
{
    if (node == NULL)
        return;
    freeAst(node->left);
    freeAst(node->right);
    freeAst(node->next);
    free(node);
}

past newAstNode_param(char *str, char *nodeType, past left, past right, past next)
{
    past node = (past)malloc(sizeof(ast));
    if (node == NULL)
    {
        printf("run out of memory.\n");
        exit(0);
    }
    memset(node, 0, sizeof(ast));
    node->value.svalue = str;
    node->nodeType = nodeType;
    node->left = left;
    node->right = right;
    node->next = next;
    return node;
}

past newNum(int value, past next)
{
    past var = newAstNode();
    var->nodeType = "IntValue";
    var->value.ivalue = value;
    var->next = next;
    return var;
}

past newType(int type)
{
    past var = newAstNode();
    var->nodeType = "type";
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
    var->left = NULL;
    var->right = NULL;
    var->next = NULL;
}

past newIdent(char *strVal, past next)
{
    past var = newAstNode();
    var->nodeType = "VarDecl";
    var->value.svalue = strVal;
    var->next = next;
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
    var->nodeType = "expr";
    var->left = left;
    var->right = right;
    var->next = next;
    return var;
}

void showAst(past node, int nest)
{
    if (node == NULL)
        return;
    int i = 0;
    for (i = 0; i < nest; i++)
        printf("  ");
    if (node->nodeType == "IntValue")
        printf("%s : %d\n", node->nodeType, node->value.ivalue);
    else if (node->nodeType == "Decl")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "ConstDecl")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "ConstDef")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "VarDef")
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (node->nodeType == "ConstInitVal")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "InitVal")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "FuncDef")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "FuncFParams")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "FuncFParam")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "noexp")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "CompUnit")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "expr")
    {
        if (node->value.svalue == "@")
            printf("%s : '%c'\n", node->nodeType, '@');
        else
            printf("%s : %s\n", node->nodeType, node->value.svalue);
    }
    else if (node->nodeType == "list")
        printf("%s : %d\n", node->nodeType, node->value.ivalue);
    else if (node->nodeType == "VarDecl")
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (node->nodeType == "arrayDecl")
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (node->nodeType == "funcDecl" || node->nodeType == "blockDecl")
        printf("%s\n", node->nodeType);
    else if (node->nodeType == "parameter")
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (node->nodeType == "type")
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (node->nodeType == "arrayDecl")
        printf("%s : %s\n", node->nodeType, node->value.svalue);
    else if (node->nodeType == "AssignStmt" || node->nodeType == "IfStmt" || node->nodeType == "WhileStmt" || node->nodeType == "BreakStmt" || node->nodeType == "ContinueStmt" || node->nodeType == "ReturnStmt" || node->nodeType == "EmptyStmt" || node->nodeType == "expStmt")
        printf("%s\n", node->nodeType);

    showAst(node->left, nest + 1);
    showAst(node->right, nest + 1);
    showAst(node->next, nest);
}

past CompUnit(void)
{
    past root = newAstNode_param("", "CompUnit", NULL, NULL, NULL);
    past tmp = root;
    if (tok == COMMENT || tok == COMMENTS)
    {
        advance();
    }
    if (tok == CONST || tok == VOID || tok == INT)
    {
        if (tok == CONST)
        {
            tmp->left = Decl();
            if (tmp->left == NULL)
                return NULL;
        }
        else if (tok == VOID)
        {
            tmp->left = FuncDef();
            if (tmp->left == NULL)
                return NULL;
        }
        else
        {
            char *s0 = strdup(yytext);
            advance();
            if (tok == IDN)
            {
                char *s1 = strdup(yytext);
                advance();
                if (tok == SLP)
                {
                    put_back(yytext);
                    put_back(s1);
                    put_back(s0);
                    advance();
                    tmp->left = FuncDef();
                    if (tmp->left == NULL)
                        return NULL;
                }
                else
                {
                    put_back(yytext);
                    put_back(s1);
                    put_back(s0);
                    advance();
                    tmp->left = Decl();
                    if (tmp->left == NULL)
                        return NULL;
                }
            }
            else
                return NULL;
        }
    }
    else
        return NULL;
    while (tok > 0)
    {

        if (tok == COMMENT || tok == COMMENTS)
        {
            advance();
        }
        if (tok == CONST || tok == VOID || tok == INT)
        {
            tmp = tmp->left;
            if (tok == CONST)
            {
                tmp->left = Decl();
                if (tmp->left == NULL)
                {
                    printf("Error : Decl()\n");
                    return NULL;
                }
            }
            else if (tok == VOID)
            {
                tmp->left = FuncDef();
                if (tmp->left == NULL)
                {
                    printf("Error : FuncDef()\n");
                    return NULL;
                }
            }
            else
            {
                char *s0 = strdup(yytext);
                advance();
                if (tok == IDN)
                {
                    char *s1 = strdup(yytext);
                    advance();
                    if (tok == SLP)
                    {
                        put_back(yytext);
                        put_back(s1);
                        put_back(s0);
                        advance();
                        tmp->next = FuncDef();
                        if (tmp->next == NULL)
                        {
                            printf("Error : FuncDef()\n");
                            return NULL;
                        }
                    }
                    else
                    {
                        put_back(yytext);
                        put_back(s1);
                        put_back(s0);
                        advance();
                        tmp->next = Decl();
                        if (tmp->next == NULL)
                        {
                            printf("Error : Decl()\n");
                            return NULL;
                        }
                    }
                }
                else
                    return NULL;
            }
        }
        else
            return NULL;
    }

    return root;
}

past Decl(void)
{
    past root = newAstNode_param("", "Decl", NULL, NULL, NULL);
    past tmp = root;
    if (tok == CONST)
    {
        root->left = ConstDecl();
        if (root->left == NULL)
        {
            printf("Error : ConstDecl()\n");
            return NULL;
        }
    }
    else if (tok == INT)
    {
        root->left = VarDecl();
        if (root->left == NULL)
        {
            printf("Error : VarDecl()\n");
            return NULL;
        }
    }
    else
        return NULL;

    return root;
}

past ConstDecl(void)
{
    past root = newAstNode_param("", "ConstDecl", NULL, NULL, NULL);
    past tmp = root;
    if (tok == CONST)
    {
        advance();
        tmp->next = BType();
        if (tmp->left == NULL)
        {
            printf("Error : BType()\n");
            return NULL;
        }
        tmp = tmp->left;
        tmp->left = ConstDef();
        if (tmp->left == NULL)
        {
            printf("Error : ConstDef()\n");
            return NULL;
        }
        tmp = tmp->left;
        while (tok == COMMA)
        {
            advance();
            tmp->next = ConstDef();
            if (tmp->next == NULL)
            {
                printf("Error : ConstDef()\n");
                return NULL;
            }
            tmp = tmp->next;
        }
        if (tok == SEMI)
        {
            advance();
            return root;
        }
        else
            return NULL;
    }
    else
        return NULL;
    return root;
}

past BType(void)
{
    past root = NULL;
    if (tok == INT)
    {
        root = newType(tok);
        advance();
    }
    else
        printf("Error : BType()\n");
    return root;
}

past ConstDef(void)
{
    past root = newAstNode_param("", "ConstDef", NULL, NULL, NULL);
    past tmp = root;
    if (tok == IDN)
    {
        past id = newIdent(strdup(yytext), NULL);
        tmp->left = id;
        tmp = tmp->left;
        advance();
        while (tok == MLP)
        {
            advance();
            tmp->next = ConstExp();
            if (tmp->next == NULL)
            {
                printf("Error : ConstExp()\n");
                return NULL;
            }
            tmp = tmp->next;
            if (tok == MRP)
            {
                advance();
            }
            else
                return NULL;
        }
        if (tok == ASSIGN)
        {
            advance();
            tmp->left = ConstInitVal();
            if (tmp->left == NULL)
            {
                printf("Error : ConstInitVal()\n");
                return NULL;
            }
            tmp = tmp->left;
        }
        else
            return NULL;
    }
    else
        return NULL;

    return root;
}

past ConstInitVal(void)
{
    past list = newAstNode_param("", "list", NULL, NULL, NULL);
    past root = newAstNode_param("", "ConstInitVal", list, NULL, NULL);
    list->value.ivalue = 0;
    past tmp = list;
    if (tok == LP)
    {
        advance();
        tmp->left = ConstInitVal();
        if (tmp->left == NULL)
        {
            printf("Error : ConstInitVal()\n");
            return NULL;
        }
        tmp = tmp->left;
        while (tok == COMMA)
        {
            advance();
            tmp->next = ConstInitVal();
            if (tmp->next == NULL)
            {
                printf("Error : ConstInitVal()\n");
                return NULL;
            }
            tmp = tmp->next;
        }
        if (tok == RP)
        {
            advance();
            return root;
        }
        else
            return NULL;
    }
    else
    {
        tmp->value.ivalue++;
        tmp->left = ConstExp();
        if (tmp->left == NULL)
        {
            printf("Error : ConstExp()\n");
            return NULL;
        }
    }

    return root;
}

past VarDecl(void)
{
    past tmp = NULL;
    if (tok == INT)
    {
        past type = newType(tok);
        past item = newAstNode_param("", "list", NULL, NULL, NULL);
        past root = newAstNode_param("", "VarDecl", type, item, NULL);
        root->right->value.ivalue = 0;
        advance();
        if (tok == IDN)
        {
            past var = VarDef();
            item->value.ivalue++;
            item->left = var;
            tmp = var;
        }
        while (tok == COMMA)
        {
            advance();
            item->value.ivalue++;
            tmp->next = VarDef();
            tmp = tmp->next;
        }
        if (tok == SEMI)
        {
            advance();
            return root;
        }
        else
        {
            printf("Error : VarDecl()\n");
            return NULL;
        }
    }
    else
    {
        printf("Error : VarDecl()\n");
        return NULL;
    }
}

past VarDef(void)
{
    past root = NULL;
    if (tok == IDN)
    {
        past l = newAstNode_param(strdup(yytext), "VarDecl", NULL, NULL, NULL);
        root = newAstNode_param("", "VarDef", l, NULL, NULL);
        advance();
        past list = newAstNode_param("", "list", NULL, NULL, NULL);
        root->left->left = list;
        root->left->left->value.ivalue = 0;
        past tmp = NULL;
        if (tok == MLP)
        {
            root->left->nodeType = "arrayDecl";
            advance();
            past head = ConstExp();
            root->left->left->left = head;
            root->left->left->value.ivalue++;
            past tmp = head;
            if (tok == MRP)
            {
                advance();
                while (tok == MLP)
                {
                    advance();
                    past first = ConstExp();
                    tmp->next = first;
                    tmp = tmp->next;
                    root->left->left->value.ivalue++;
                    if (tok == MRP)
                    {
                        advance();
                    }
                }
                if (tok == ASSIGN)
                {
                    advance();
                    past init = InitVal();
                    root->right = init;
                    return root;
                }
                else
                {
                    return root;
                }
            }
        }
        else if (tok == ASSIGN)
        {
            advance();
            past rightnode = InitVal();
            root->right = rightnode;
            return root;
        }
        else
        {
            return root;
        }
    }
    else
        return NULL;

    return root;
}

past InitVal(void)
{
    past list = newAstNode_param("", "list", NULL, NULL, NULL);
    past root = newAstNode_param("", "InitVal", list, NULL, NULL);
    list->value.ivalue = 0;
    past tmp = list;
    if (tok == LP)
    {
        advance();
        tmp->left = InitVal();
        if (tmp->left != NULL)
        {
            tmp = tmp->left;
            list->value.ivalue++;

            while (tok == COMMA)
            {
                advance();
                tmp->next = InitVal();
                if (tmp->next == NULL)
                    return NULL;
                tmp = tmp->next;
                list->value.ivalue++;
            }
        }

        if (tok == RP)
        {
            advance();
        }
        else
            return NULL;
    }
    else
    {
        list->value.ivalue++;
        list->left = Exp();
        if (list->left != NULL)
            return root;
        else
        {
            printf("Error : ConstInitVal()\n");
            return NULL;
        }
    }

    return root;
}

past FuncDef(void)
{
    past root = newAstNode();
    root->nodeType = "FuncDef";
    if (tok == VOID || tok == INT)
    {
        past type = FuncType();
        root->left = type;
        if (tok == IDN)
        {
            past id = newIdent(strdup(yytext), NULL);
            type->left = id;
            advance();
            if (tok == SLP)
            {
                advance();
                if (tok != SRP)
                {
                    past funcFP = FuncFParams();
                    id->left = funcFP;
                    advance();
                }
                else
                {
                    advance();
                }
                past block = Block();
                root->right = block;
                return root;
            }
            else
            {
                printf("Error : FuncDef()\n");
                return NULL;
            }
        }
        else
        {
            printf("Error : FuncDef()\n");
            return NULL;
        }
    }
    else
    {
        printf("Error : FuncDef()\n");
        return NULL;
    }

    return root;
}

past FuncType(void)
{
    past root = NULL;
    if (tok == VOID || tok == INT)
    {
        root = newType(tok);
        advance();
    }
    else
    {
        printf("Error : FuncType()\n");
        return NULL;
    }
    return root;
}

past FuncFParams(void)
{
    int list = 0;
    past l = newAstNode_param("", "list", NULL, NULL, NULL);
    past root = newAstNode_param("", "FuncFParams", l, NULL, NULL);
    root->left->value.ivalue = list;
    past head = FuncFParam();
    root->left->left = head;
    root->left->value.ivalue++;
    while (tok == COMMA)
    {
        advance();
        root->left->value.ivalue++;
        head->next = FuncFParam();
        if (head->next == NULL)
        {
            printf("Error : FuncFParam()\n");
            return NULL;
        }
        head = head->next;
    }
    return root;
}

past FuncFParam(void)
{
    int list = 0;
    past leftint = BType();
    past root = NULL;
    if (tok == IDN)
    {
        past r = newIdent(strdup(yytext), NULL);
        past root = newAstNode_param("", "FuncFParam", leftint, r, NULL);
        advance();
        past listnode = newAstNode_param("", "list", NULL, NULL, NULL);
        root->right->left = listnode;
        root->right->left->value.ivalue = list;
        past tmp = NULL;
        if (tok == MLP)
        {
            advance();
            if (tok == MRP)
            {
                past head = newAstNode_param("", "noexp", NULL, NULL, NULL);
                root->right->left->left = head;
                root->right->left->value.ivalue++;
                tmp = head;
                advance();
                while (tok == MLP)
                {
                    advance();
                    tmp->next = Exp();
                    if (tmp->next == NULL)
                    {
                        printf("Error : Exp()\n");
                        return NULL;
                    }
                    tmp = tmp->next;
                    root->right->left->value.ivalue++;
                    if (tok == MRP)
                    {
                        advance();
                    }
                }
                return root;
            }
        }
        else
        {
            return root;
        }
    }
    else
        return NULL;

    return root;
}

past Block(void)
{
    past list = newAstNode_param("", "list", NULL, NULL, NULL);
    list->value.ivalue = 0;
    past block = newAstNode_param("", "blockDecl", NULL, NULL, NULL);
    list->left = block;
    past tmp = block;
    if (tok == LP)
    {
        advance();
        if (tok != RP)
        {
            list->value.ivalue++;
            block->left = BlockItem();
            if (block->left == NULL)
            {
                printf("Error : BlockItem()\n");
                return NULL;
            }
            tmp = block->left;
        }
        while (tok != RP)
        {
            list->value.ivalue++;
            tmp->left = BlockItem();
            if (tmp->left == (void *)(0x10) || tmp->left == NULL)
            {
                printf("Error : BlockItem()\n");
                return NULL;
            }
            tmp = tmp->left;
        }
    }
    advance();
    return list;
}

past BlockItem(void)
{
    if (tok == CONST || tok == INT)
    {
        return Decl();
    }
    else
    {
        return Stmt();
    }
}

past Stmt(void)
{
    past root = NULL;
    if (tok == IDN)
    {
        char *s0 = strdup(yytext);
        advance();
        put_back(yytext);
        put_back(s0);
        if (tok == SLP)
        {
            advance();
            root = Exp();
            if (root != NULL)
            {
                if (tok == SEMI)
                {
                    advance();
                }
                else
                {
                    printf("Error : Exp()->SEMI\n");
                    return NULL;
                }
            }
            else
            {
                printf("Error : Exp()\n");
                return NULL;
            }
        }
        else
        {
            advance();
            root = LVal();
            if (root != NULL)
            {
                if (tok == ASSIGN)
                {
                    advance();
                    past exp = Exp();
                    past head = newAstNode_param("", "AssignStmt", root, exp, NULL);
                    if (tok == SEMI)
                    {
                        advance();
                        return head;
                    }
                }
                else
                {
                    printf("Error : Exp()->ID->ASSIGN\n");
                    return NULL;
                }
            }
        }
    }
    else if (tok == IF)
    {
        int list = 1;
        root = newAstNode();
        root->nodeType = "IfStmt";
        root->right = newAstNode_param("", "list", NULL, NULL, NULL);
        root->right->value.ivalue = list;
        advance();
        if (tok == SLP)
        {
            advance();
            past cond = Cond();
            root->left = cond;
            if (tok == SRP)
            {
                advance();
                if (root->right->left = Stmt())
                {
                    if (tok == ELSE)
                    {
                        advance();
                        if (root->right->right = Stmt())
                        {
                            root->right->value.ivalue++;
                            return root;
                        }
                        else
                        {
                            printf("Error : Exp()->IF->SLP->SRP->ELSE\n");
                            return NULL;
                        }
                    }
                    return root;
                }
                else
                {
                    printf("Error : Exp()->IF->SLP->SRP->astStmt()\n");
                    return NULL;
                }
            }
            else
            {
                printf("Error : Stmt()->IF->SRP\n");
                return NULL;
            }
        }
        else
        {
            printf("Error : Stmt()->IF\n");
            return NULL;
        }
    }
    else if (tok == WHILE)
    {
        root = newAstNode_param("", "WhileStmt", NULL, NULL, NULL);
        advance();
        if (tok == SLP)
        {
            advance();
            past cond = Cond();
            root->left = cond;
            if (tok == SRP)
            {
                advance();
                if (root->right = Stmt())
                {
                    return root;
                }
                else
                {
                    printf("Error : Stmt()->WHILE\n");
                    return NULL;
                }
            }
            else
            {
                printf("Error : Stmt()->WHILE\n");
                return NULL;
            }
        }
        else
        {
            printf("Error : Stmt()->WHILE\n");
            return NULL;
        }
    }
    else if (tok == BREAK)
    {
        root = newAstNode_param("", "BreakStmt", NULL, NULL, NULL);
        advance();
        if (tok == SEMI)
        {
            advance();
            return root;
        }
    }
    else if (tok == CONTINUE)
    {
        root = newAstNode_param("", "ContinueStmt", NULL, NULL, NULL);
        advance();
        if (tok == SEMI)
        {
            advance();
            return root;
        }
    }
    else if (tok == RETURN)
    {
        root = newAstNode_param("", "ReturnStmt", NULL, NULL, NULL);
        advance();
        root->left = Exp();
        if (tok == SEMI)
        {
            advance();
            return root;
        }
    }
    else if (tok == LP)
    {
        return Block();
    }
    else if (tok == SEMI)
    {
        root = newAstNode_param("", "EmptyStmt", NULL, NULL, NULL);
        advance();
        return root;
    }
    else
    {
        past root = newAstNode_param("", "expStmt", NULL, NULL, NULL);
        if (root->left = Exp())
        {
            if (tok == SEMI)
            {
                advance();
                return root;
            }
            return NULL;
        }
        return NULL;
    }
    return root;
}

past Exp()
{
    past a = AddExp();
    return a;
}

past Cond(void)
{
    past a = LOrExp();
    return a;
}

past LVal(void)
{
    int flag = 1;
    if (tok != IDN)
        return NULL;
    past root = newIdent(strdup(yytext), NULL);
    past tmp;
    advance();
    while (tok == MLP)
    {
        advance();
        past exp = Exp();
        if (flag == 1)
        {
            root->left = exp;
            root->nodeType = "arrayDecl";
            tmp = exp;
            flag++;
        }
        else
        {
            tmp->next = exp;
            tmp = tmp->next;
        }
        if (tok == MRP)
            advance();
        else
        {
            printf("Error : LVal()\n");
            return NULL;
        }
    }
    return root;
}

past PrimaryExp(void)
{
    if (tok == SLP)
    {
        advance();
        past pri = Exp();
        if (tok == SRP)
        {
            advance();
            return pri;
        }
        else
            printf("Error : PrimaryExp()->SLP\n");
    }
    else if (tok == IDN)
        return LVal();
    else if (tok == NUM)
        return Number();
    else
        printf("Error : PrimaryExp()\n");
    return NULL;
}

past Number(void)
{
    if (tok == NUM)
    {
        past num = newNum(yylval, NULL);
        advance();
        return num;
    }
    return NULL;
}

past UnaryExp(void)
{
    past ue = NULL;
    past root = NULL;
    if (tok == ADD || tok == SUB || tok == NOT)
    {
        while (tok == ADD || tok == SUB || tok == NOT)
        {
            int oper = tok;
            advance();
            ue = UnaryExp();
            ue = newExpr(oper, NULL, ue, NULL);
        }
        advance();
        return ue;
    }
    else if (tok == IDN)
    {
        char *s1 = strdup(yytext);
        ue = newIdent(strdup(yytext), NULL);
        advance();
        if (tok == SLP)
        {
            advance();
            if (tok == SRP)
            {
                advance();
                return ue;
            }
            else
            {
                past root = newAstNode();
                root->nodeType = "funcDecl";
                root->left = ue;
                past funcRP = FuncRParams();
                root->right = funcRP;
                if (tok == SRP)
                {
                    advance();
                    return root;
                }
                else
                    printf("Error : UnaryExp()\n");
            }
            return NULL;
        }
        else
        {
            if (tok == SRP)
                return ue;
            put_back(strdup(yytext));
            put_back(s1);
            advance();
            return LVal();
        }
    }
    else
    {
        root = PrimaryExp();
        return root;
    }

    return root;
}

past FuncRParams(void)
{
    int list = 1;
    past root = newAstNode();
    root->nodeType = "list";
    past func = Exp();
    past head = func;
    while (tok == COMMA)
    {
        list++;
        advance();
        past brother = Exp();
        func->next = brother;
        func = func->next;
    }
    root->value.ivalue = list;
    root->left = head;
    head->nodeType = "parameter";
    head = head->next;
    while (head != NULL)
    {
        head->nodeType = "parameter";
        head = head->next;
    }
    return root;
}

past MulExp(void)
{

    past l = UnaryExp();
    while (tok == MUL || tok == DIV || tok == MOD)
    {
        int oper = tok;
        advance();
        past r = UnaryExp();
        l = newExpr(oper, l, r, NULL);
    }
    return l;
}

past AddExp(void)
{

    past l = MulExp();
    while (tok == ADD || tok == SUB)
    {
        int oper = tok;
        advance();
        past r = MulExp();
        l = newExpr(oper, l, r, NULL);
    }
    return l;
}

past RelExp(void)
{

    past l = AddExp();
    while (tok == LT || tok == GT || tok == LE || tok == GE)
    {
        int oper = tok;
        advance();
        past r = RelExp();
        l = newExpr(oper, l, r, NULL);
    }
    return l;
}

past EqExp(void)
{

    past l = RelExp();
    while (tok == EQ || tok == NE)
    {
        int oper = tok;
        advance();
        past r = RelExp();
        l = newExpr(oper, l, r, NULL);
    }
    return l;
}

past LAndExp(void)
{

    past l = EqExp();
    while (tok == AND)
    {
        int oper = tok;
        advance();
        past r = EqExp();
        l = newExpr(oper, l, r, NULL);
    }
    return l;
}

past LOrExp(void)
{

    past l = LAndExp();
    while (tok == OR)
    {
        int oper = tok;
        advance();
        past r = LAndExp();
        l = newExpr(oper, l, r, NULL);
    }
    return l;
}

past ConstExp(void)
{
    past a = AddExp();
    return a;
}

int main(int argc, char **argv)
{
    if (argv[1])
    {
        if ((yyin = fopen(argv[1], "r")))
        {
            printf("reading data from file:%s\n", argv[1]);
        }
    }
    else
    {
        printf("Input Format Wrong!\n");
        return 0;
    }
    setbuf(stdout, NULL);
    advance();
    past root = CompUnit();
    if (root)
    {
        printf("\n");
        showAst(root, 0);
        freeAst(root);
        printf("\nprogram done successfully!\n");
    }
    else
        printf("ERROR!\n");

    return 0;
}