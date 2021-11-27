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
extern int isFuncDef();
extern void put_back(char *yytext);

int tok;

void advance()
{
	tok = yylex();
	printf("tok is:%d %s\n", tok, yytext);
}

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

past newExpr(int oper, past left, past right, past next);
past newIdent(char *strVal, past next);
past newType(int type);
past newNum(int value, past next);
past newAstNode_param(char *strVal, char *nodeType, past left, past right, past next);
past CompUnit();
past Decl();
past FuncDef();
past ConstDecl();
past VarDecl();
past VarDef();
past BType();
past ConstDef();
past ConstInitVal();
past InitVal();
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

past newAstNode();
void showAst(past node, int nest);
void freeAst(past node);

//创建节点
past newAstNode()
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

past newAstNode_param(char *strVal, char *nodeType, past left, past right, past next)
{
	past node = (past)malloc(sizeof(ast));
	if (node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	node->value.svalue = strVal;
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
	var->left = NULL;
	var->right = NULL;
	var->next = NULL;
}

past newIdent(char *IDN, past next)
{
	past var = newAstNode();
	var->nodeType = "Variable";
	var->value.svalue = IDN;
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
	var->nodeType = "Expr";
	var->left = left;
	var->right = right;
	var->next = next;
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
	{
		printf("%s\n", node->nodeType);
	}
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
	else if (!strcmp(node->nodeType, "Expr"))
	{
		if (node->value.svalue, "@")
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

// 编译单元 CompUnit → [ CompUnit ] ( Decl | FuncDef )
past CompUnit()
{
	past l = newAstNode_param("", "CompUnit", NULL, NULL, NULL);
	past tmp = l;
	if (tok == COMMENT || tok == COMMENTS)
	{
		advance();
	}

	if (tok == CONST)
	{
		tmp->left = Decl();
	}
	else if (tok == VOID)
	{
		tmp->left = FuncDef();
	}
	else if (tok == INT)
	{
		if (isFuncDef())
		{
			tmp->left = FuncDef();
			if (tmp->left == NULL)
				return NULL;
		}
		else
		{
			tmp->left = Decl();
			if (tmp->left == NULL)
				return NULL;
		}
	}
	tmp = tmp->left;
	while (tok > 0)
	{
		if (tok == COMMENT || tok == COMMENTS)
		{
			advance();
		}
		else if (tok == CONST)
		{
			tmp->next = Decl();
		}
		else if (tok == VOID)
		{
			tmp->next = FuncDef();
		}
		else if (tok == INT)
		{
			if (isFuncDef())
			{
				tmp->next = FuncDef();
			}
			else
			{
				tmp->next = Decl();
			}
		}
		else
		{
			printf("118ERROR! %s\n", yytext);
			exit(0);
		}
		tmp = tmp->next;
	}
	return l;
}

// 声明 Decl → ConstDecl | VarDecl
past Decl()
{
	past l = newAstNode();
	if (tok == CONST)
	{
		l = ConstDecl();
	}
	else if (tok == INT)
	{
		l = VarDecl();
	}
	else
	{
		return NULL;
	}
	return l;
}

// 常量声明 ConstDecl → 'const' BType ConstDef { ',' ConstDef } ';'
past ConstDecl()
{
	past l = newAstNode_param("", "ConstDecl", NULL, NULL, NULL);
	past tmp = l;
	if (tok == CONST)
	{
		advance();
		tmp->left = BType();
		tmp = tmp->left;
		tmp->next = ConstDef();
		tmp = tmp->next;
		while (tok == COMMA)
		{
			advance();
			tmp->next = ConstDef();
			tmp = tmp->next;
		}
		if (tok == SEMI)
		{
			advance();
			return l;
		}
		else
		{
			printf("160ERROR:Expect a ';'\n");
			exit(0);
		}
	}
	return l;
}

// 基本类型 BType → 'int'
past BType()
{
	past l = NULL;
	if (tok == INT)
	{
		l = newType(tok);
		advance();
	}
	return l;
}

// 常数定义 ConstDef → Ident { '[' ConstExp ']' } '=' ConstInitVal
past ConstDef()
{
	past l = newAstNode_param("", "ConstDef", NULL, NULL, NULL);
	past tmp = l;
	if (tok == IDN)
	{
		char *s = strdup(yytext);
		tok = yylex();
		if (tok == MLP)
		{
			past idn = newIdent(s, NULL);
			idn->nodeType = "ArrayID";
			tmp->left = idn;
			tmp = tmp->left;
		}
		else
		{
			past idn = newIdent(s, NULL);
			tmp->left = idn;
			tmp = tmp->left;
		}
		put_back(yytext);
		advance();
		if (tok == MLP)
		{
			advance();
			past index = newAstNode();
			index->nodeType = "ArrayIndex";
			index->left = ConstExp();
			// tmp->left->nodeType = "ArrayIndex";
			tmp->left = index;
			tmp = tmp->left;
			if (tok == MRP)
			{
				advance();
			}
			else
			{
				printf("189ERROR:Expect a ']'\n");
				exit(0);
			}
		}

		while (tok == MLP)
		{
			advance();
			past index = newAstNode();
			index->nodeType = "ArrayIndex";
			index->left = ConstExp();
			tmp->next = index;
			// tmp->next->nodeType = "ArrayIndex";
			tmp = tmp->next;
			if (tok == MRP)
			{
				advance();
			}
			else
			{
				printf("189ERROR:Expect a ']'\n");
				exit(0);
			}
		}
		if (tok == ASSIGN)
		{
			advance();
			l->right = ConstInitVal();
		}
		else
		{
			printf("200ERROR:Expect a '='\n");
			exit(0);
		}
	}
	return l;
}

// 常量初值 ConstInitVal → ConstExp
// 						| '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
past ConstInitVal()
{
	past l = newAstNode_param("", "ConstInitVal", NULL, NULL, NULL);
	past tmp = l;
	if (tok == LP)
	{
		advance();
		if (tok != RP)
		{
			tmp->left = ConstInitVal();
			tmp = tmp->left;
			while (tok == COMMA)
			{
				advance();
				tmp->next = ConstInitVal();
				tmp = tmp->next;
			}
		}
		if (tok == RP)
		{
			advance();
			return l;
		}
		else
		{
			printf("228ERROR:Expect a '}'\n");
			exit(0);
		}
	}
	tmp->left = ConstExp();
	return l;
}

// 变量声明 VarDecl → BType VarDef { ',' VarDef } ';'
past VarDecl()
{
	past type = BType();
	past l = newAstNode_param("", "VarDecl", type, NULL, NULL);
	l->right = VarDef();
	past tmp = l->right;
	if (!tmp)
		printf("null\n");
	while (tok == COMMA)
	{
		advance();
		tmp->next = VarDef();
		tmp = tmp->next;
	}
	if (tok == SEMI)
	{
		advance();
		return l;
	}
	return l;
}

// 变量定义 VarDef → Ident { '[' ConstExp ']' }
// 					| Ident { '[' ConstExp ']' } '=' InitVal
past VarDef()
{
	past root = newAstNode_param("", "VarDef", NULL, NULL, NULL);
	if (tok == IDN)
	{
		past tmp = root;
		char *s = strdup(yytext);
		past idn = newIdent(s, NULL);
		tok = yylex();
		if (tok == MLP)
		{
			idn->nodeType = "ArrayID";
			tmp->left = idn;
			tmp = tmp->left;
		}
		else
		{
			tmp->left = idn;
			tmp = tmp->left;
		}
		put_back(yytext);

		advance();

		if (tok == MLP)
		{
			advance();
			past index = newAstNode();
			index->nodeType = "ArrayIndex";
			index->left = ConstExp();
			tmp->left = index;
			// tmp->left->nodeType = "ArrayIndex";
			tmp = tmp->left;
			if (tok == MRP)
			{
				advance();
				while (tok == MLP)
				{
					advance();
					past index = newAstNode();
					index->nodeType = "ArrayIndex";
					index->left = ConstExp();
					tmp->next = index;
					// tmp->next->nodeType = "ArrayIndex";
					tmp = tmp->next;
					if (tok == MRP)
					{
						advance();
					}
				}
				if (tok == ASSIGN)
				{
					advance();
					root->right = InitVal();
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
			root->right = InitVal();
			return root;
		}
		else
		{
			return root;
		}
	}
	else
	{
		printf("281ERROR:Expect a Ident\n");
		exit(0);
	}
}

// 变量初值 InitVal → Exp | '{' [ InitVal { ',' InitVal } ] '}'
past InitVal()
{
	past l = newAstNode_param("", "InitVal", NULL, NULL, NULL);
	past tmp = l;
	if (tok == LP)
	{
		advance();
		if (tok != RP)
		{
			tmp->left = InitVal();
			tmp = tmp->left;
			while (tok == COMMA)
			{
				advance();
				tmp->next = InitVal();
				if (tmp->next == NULL)
					return NULL;
				tmp = tmp->next;
			}
		}
		if (tok == RP)
		{
			advance();
			return l;
		}
		else
		{
			printf("307ERROR:Expect a '}'\n");
			exit(0);
		}
	}
	tmp->left = Exp();
	return l;
}

// 函数定义 FuncDef → FuncType Ident '(' [FuncFParams] ')' Block
past FuncDef()
{
	past l = newAstNode();
	l->nodeType = "FuncDef";
	l->left = FuncType();
	past tmp = l->left;
	if (tok == IDN)
	{
		past idn = newIdent(strdup(yytext), NULL);
		idn->nodeType = "FuncName";
		tmp->next = idn;
		tmp = tmp->next;
		advance();
		if (tok == SLP)
		{
			advance();
			if (tok == INT)
			{
				tmp->next = FuncFParams();
				if (tok == SRP)
				{
					advance();
					l->right = Block();
					return l;
				}
				else
				{
					printf("335ERROR:Expect a ')'\n");
					exit(0);
				}
			}
			else
			{
				if (tok == SRP)
				{
					advance();
					l->right = Block();
					return l;
				}
			}
		}
		else
		{
			printf("350ERROR:Expect a '('\n");
		}
	}
	else
	{
		printf("355ERROR:Expect a Ident\n");
		exit(0);
	}
}

// 函数类型 FuncType → 'void' | 'int'
past FuncType()
{
	past l;
	if (tok == VOID)
	{
		l = newType(tok);
		advance();
	}
	else if (tok == INT)
	{
		l = newType(tok);
		advance();
	}
	else
	{
		printf("373ERROR:Expect FuncType with 'past' or 'int'\n");
		exit(0);
	}
	return l;
}

// 函数形参表 FuncFParams → FuncFParam { ',' FuncFParam }
past FuncFParams()
{
	past l = newAstNode_param("", "Count", NULL, NULL, NULL);
	past root = newAstNode_param("", "FuncFParams", l, NULL, NULL);
	l->value.ivalue = 0;
	l->left = FuncFParam();
	l->value.ivalue++;
	past tmp = l->left;
	while (tok == COMMA)
	{
		advance();
		l->value.ivalue++;
		tmp->next = FuncFParam();
		if (tmp->next == NULL)
		{
			printf("748Error: astFuncFParam()\n");
			return NULL;
		}
		tmp = tmp->next;
	}
	return root;
}

// 函数形参 FuncFParam → BType Ident ['[' ']' { '[' Exp ']' }]
past FuncFParam()
{
	past leftint = BType();
	past root = NULL;
	if (tok == IDN)
	{
		root = newAstNode_param("", "FuncFParam", leftint, NULL, NULL);
		past tmp = root;
		char *s = strdup(yytext);
		past idn = newIdent(s, NULL);
		tok = yylex();
		if (tok == MLP)
		{
			idn->nodeType = "ArrayID";
			tmp->right = idn;
			tmp = tmp->right;
		}
		else
		{
			tmp->right = idn;
			tmp = tmp->right;
		}
		put_back(yytext);
		advance();
		if (tok == MLP)
		{
			advance();
			if (tok == MRP)
			{
				past index = newAstNode();
				index->nodeType = "ArrayIndex";
				tmp->left = index;
				tmp = tmp->left;
				advance();
				while (tok == MLP)
				{
					advance();
					index = newAstNode();
					index->nodeType = "ArrayIndex";
					index->left = Exp();
					tmp->next = index;
					if (tmp->next == NULL)
					{
						printf("Error: Exp()\n");
						return NULL;
					}
					tmp = tmp->next;
					if (tok == MRP)
					{
						advance();
					}
					else
					{
						printf("412ERROR:Expect a ']'\n");
						exit(0);
					}
				}
				return root;
			}
			else
			{
				printf("419ERROR:Expect a ']'\n");
				exit(0);
			}
		}
	}
	else
	{
		printf("426ERROR:Expect a Ident\n");
		exit(0);
	}
	return root;
}

// 语句块 Block → '{' { BlockItem } '}'
past Block()
{
	past block = newAstNode_param("", "Block", NULL, NULL, NULL);
	past tmp = block;
	if (tok == LP)
	{
		advance();
		while (tok == COMMENT || tok == COMMENTS)
		{
			advance();
		}
		if (tok != RP)
		{
			tmp->left = BlockItem();
			if (tmp->left == NULL)
			{
				printf("901Error: BlockItem()\n");
				return NULL;
			}
			tmp = tmp->left;
		}
		while (tok != RP)
		{
			while (tok == COMMENT || tok == COMMENTS)
			{
				advance();
			}
			tmp->next = BlockItem();
			if (tmp->next == NULL)
			{
				printf("Error: BlockItem()\n");
				return NULL;
			}
			tmp = tmp->next;
		}
		if (tok == RP)
		{
			advance();
		}
		else
		{
			printf("%d %s447ERROR:Expect a '}'\n", tok, yytext);
			exit(0);
		}
	}
	else
	{
		printf("453ERROR:Expect a '{'\n");
		exit(0);
	}
	return block;
}

// 语句块项 BlockItem → Decl | Stmt
past BlockItem()
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

// 语句 Stmt → LVal '=' Exp ';' | [Exp] ';' | Block
// 			| 'if' '( Cond ')' Stmt [ 'else' Stmt ]
// 			| 'while' '(' Cond ')' Stmt
// 			| 'break' ';' | 'continue' ';'
// 			| 'return' [Exp] ';'
past Stmt()
{
	past root = NULL;
	if (tok == COMMENT || tok == COMMENTS)
		advance();
	else if (tok == IF)
	{
		root = newAstNode();
		root->nodeType = "IfStmt";
		past tmp = root;
		advance();
		if (tok == SLP)
		{
			advance();
			tmp->left = Cond();
			tmp = tmp->left;
			if (tok == SRP)
			{
				advance();
				tmp->next = Stmt();
				if (tok == ELSE)
				{
					past el = newAstNode();
					el->nodeType = "ElseStmt";
					root->right = el;
					advance();
					if (el->left = Stmt())
						return root;
				}
				return root;
			}
			else
			{
				printf("497ERROR:Expect a ')'\n");
				exit(0);
			}
		}
		else
		{
			printf("503ERROR:Expect a '('\n");
			exit(0);
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
			}
			else
			{
				printf("521ERROR:Expect a ')'\n");
				exit(0);
			}
		}
		else
		{
			printf("527ERROR:Expect a '('\n");
			exit(0);
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
		else
		{
			printf("540ERROR:Expect a ';'\n");
			exit(0);
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
		else
		{
			printf("553ERROR:Expect a ';'\n");
			exit(0);
		}
	}
	else if (tok == RETURN)
	{
		root = newAstNode_param("", "ReturnStmt", NULL, NULL, NULL);
		advance();
		if (tok == IDN || tok == NUM || tok == ADD || tok == SUB || tok == NOT || tok == SLP)
		{
			root->left = Exp();
		}

		if (tok == SEMI)
		{
			advance();
			return root;
		}
		else
		{
			printf("571ERROR:Expect a ';'\n");
			exit(0);
		}
	}
	else if (tok == LP)
	{
		return Block();
	}
	else if (tok == IDN)
	{
		int flag = 0;
		char *s = strdup(yytext);
		tok = yylex();
		if (tok == MLP || tok == ASSIGN)
			flag = 1;
		put_back(yytext);
		put_back(s);
		tok = yylex();
		if (flag)
		{
			root = LVal();
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
				else
				{
					printf("593ERROR:Expect a ';'\n");
					exit(0);
				}
			}
			else
			{
				printf("599ERROR:Expect a '='\n");
				exit(0);
			}
		}
		else
		{
			root = Exp();
			if (tok == SEMI)
				advance();
		}
	}
	else if (tok == SEMI)
	{
		root = newAstNode();
		root->nodeType = "EmptyStmt";
		advance();
	}
	else
	{
		root = newAstNode_param("", "expStmt", NULL, NULL, NULL);
		if (tok == IDN || tok == NUM || tok == ADD || tok == SUB || tok == NOT || tok == SLP)
		{
			root->left = Exp();
		}

		if (tok == SEMI)
		{
			advance();
			return root;
		}
		else
		{
			printf("616ERROR:Expect a ';'\n");
			exit(0);
		}
	}
	return root;
}

// 表达式 Exp → AddExp 注：SysY 表达式是 int 型表达式
past Exp()
{
	return AddExp();
}

// 条件表达式 Cond → LOrExp
past Cond()
{
	return LOrExp();
}

// 左值表达式 LVal → Ident {'[' Exp ']'}
past LVal()
{
	int flag = 1;
	if (tok == IDN)
	{
		past root = newIdent(strdup(yytext), NULL);
		past tmp;
		advance();
		while (tok == MLP)
		{
			advance();
			past index = newAstNode();
			index->nodeType = "ArrayIndex";
			index->left = Exp();
			if (flag == 1)
			{
				root->left = index;
				root->nodeType = "ArrayID";
				tmp = index;
				flag++;
			}
			else
			{
				tmp->next = index;
				tmp = tmp->next;
			}
			if (tok == MRP)
			{
				advance();
			}
			else
			{
				printf("650ERROR:Expect a ']'\n");
				exit(0);
			}
		}
		return root;
	}
	else
	{
		printf("657ERROR:Expect LVal with 'Ident'\n");
		exit(0);
	}
}

// 基本表达式 PrimaryExp → '(' Exp ')' | LVal | Number
past PrimaryExp()
{
	if (tok == SLP)
	{
		advance();
		past l = Exp();
		if (tok == SRP)
		{
			advance();
			return l;
		}
		else
		{
			printf("675ERROR:Expect a ')'\n");
			exit(0);
		}
	}
	else if (tok == NUM)
	{
		return Number();
	}
	else if (tok == IDN)
	{
		return LVal();
	}
	else
	{
		printf("689ERROR:Expect a '('\n");
		exit(0);
	}
}

// 数值 Number → IntConst
past Number()
{
	if (tok == NUM)
	{
		past num = newNum(yylval, NULL);
		advance();
		return num;
	}
	else
	{
		printf("703ERROR:Expect a number\n");
		exit(0);
	}
}

// 一元表达式 UnaryExp → PrimaryExp | Ident '(' [FuncRParams] ')'
// 						| UnaryOp UnaryExp
past UnaryExp()
{
	past ue = NULL;
	past root = NULL;
	if (tok == IDN)
	{
		char *s1 = strdup(yytext);
		tok = yylex();
		if (tok == SLP)
			put_back(strdup(yytext));
		else
		{
			put_back(strdup(yytext));
			put_back(s1);
			tok = yylex();
			return LVal();
		}
		ue = newIdent(s1, NULL);
		advance();
		if (tok == SLP)
		{

			advance();
			root = newAstNode();
			root->nodeType = "FuncRefer";
			root->left = ue;
			if (tok != SRP)
				root->right = FuncRParams();

			if (tok == SRP)
			{
				advance();
				return root;
			}
			else
			{
				printf("728ERROR:Expect a ')'\n");
				exit(0);
			}
		}
	}
	else if (tok == ADD || tok == SUB || tok == NOT)
	{
		while (tok == ADD || tok == SUB || tok == NOT)
		{
			int oper = tok;
			advance();
			ue = UnaryExp();
			ue = newExpr(oper, NULL, ue, NULL);
		}
		return ue;
	}
	else if (tok == SLP || tok == NUM)
	{
		return PrimaryExp();
	}
	return root;
}

// 单目运算符 UnaryOp → '+' | '−' | '!' 注：'!'仅出现在条件表达式中
past UnaryOp()
{
	if (tok == ADD || tok == SUB || tok == NOT)
	{
		advance();
	}
	else
	{
		printf("775ERROR:Expect UnaryOp Wrong\n");
		exit(0);
	}
}

// 函数实参表 FuncRParams → Exp { ',' Exp }
past FuncRParams()
{
	if (tok == IDN || tok == NUM || tok == ADD || tok == SUB || tok == NOT || tok == SLP)
	{
		past root = newAstNode();
		root->nodeType = "Count";
		root->value.ivalue = 1;
		past tmp = root;
		tmp->left = Exp();
		tmp->left->nodeType = "FuncRParam";
		tmp = tmp->left;
		while (tok == COMMA)
		{
			root->value.ivalue++;
			advance();
			tmp->next = Exp();
			tmp->next->nodeType = "FuncRParam";
			tmp = tmp->next;
		}
		return root;
	}
	else
	{
		printf("798ERROR:Expect FuncRParams Wrong\n");
		exit(0);
	}
}

// 乘除模表达式 MulExp → UnaryExp | MulExp ('*' | '/' | '%') UnaryExp
past MulExp()
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

// 加减表达式 AddExp → MulExp | AddExp ('+' | '−') MulExp
past AddExp()
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

// 关系表达式 RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
past RelExp()
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

// 相等性表达式 EqExp → RelExp | EqExp ('==' | '!=') RelExp
past EqExp()
{
	past l = RelExp();
	while (tok == NE || tok == EQ)
	{
		int oper = tok;
		advance();
		past r = RelExp();
		l = newExpr(oper, l, r, NULL);
	}
	return l;
}

// 逻辑与表达式 LAndExp → EqExp | LAndExp '&&' EqExp
past LAndExp()
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

// 逻辑或表达式 LOrExp → LAndExp | LOrExp '||' LAndExp
past LOrExp()
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

// 常量表达式 ConstExp → AddExp
past ConstExp()
{
	return AddExp();
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
		showAst(root, 0);
		freeAst(root);
		printf("program done successfully!\n");
	}
	else
		printf("Error!\n");

	return 0;
}