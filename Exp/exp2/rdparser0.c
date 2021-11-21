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
	MAIN,
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

past newIdent(char *IDN, past next)
{
	past var = newAstNode();
	var->nodeType = "VarDecl";
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
	var->nodeType = "expr";
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
	if (node->nodeType == "IntValue")
		printf("%s : %d\n", node->nodeType, node->value.ivalue);
	else if (node->nodeType == "decl")
		printf("%s : \n", node->nodeType);
	else if (node->nodeType == "ConstDecl")
		printf("%s : \n", node->nodeType);
	else if (node->nodeType == "ConstDef")
		printf("%s : \n", node->nodeType);
	else if (node->nodeType == "VarDef")
		printf("%s : %s\n", node->nodeType, node->value.svalue);
	else if (node->nodeType == "ConstInitVal")
		printf("%s : \n", node->nodeType);
	else if (node->nodeType == "InitVal")
		printf("%s : \n", node->nodeType);
	else if (node->nodeType == "FuncDef")
		printf("%s : \n", node->nodeType);
	else if (node->nodeType == "FuncFParams")
		printf("%s : \n", node->nodeType);
	else if (node->nodeType == "FuncFParam")
		printf("%s : \n", node->nodeType);
	else if (node->nodeType == "noexp")
		printf("%s : \n", node->nodeType);
	else if (node->nodeType == "CompUnit")
		printf("%s : \n", node->nodeType);
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
	else if (node->nodeType == "ArrayDecl")
		printf("%s : %s\n", node->nodeType, node->value.svalue);
	else if (node->nodeType == "FuncDecl" || node->nodeType == "BlockDecl")
		printf("%s : \n", node->nodeType);
	else if (node->nodeType == "parameter")
		printf("%s : %s\n", node->nodeType, node->value.svalue);
	else if (node->nodeType == "type")
		printf("%s : %s\n", node->nodeType, node->value.svalue);
	else if (node->nodeType == "ArrayDecl")
		printf("%s : %s\n", node->nodeType, node->value.svalue);
	else if (node->nodeType == "AssignStmt" || node->nodeType == "IfStmt" || node->nodeType == "WhileStmt" || node->nodeType == "BreakStmt" || node->nodeType == "ContinueStmt" || node->nodeType == "ReturnStmt" || node->nodeType == "EmptyStmt" || node->nodeType == "expStmt")
		printf("%s : \n", node->nodeType);

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
		// printf("0");
		if (isFuncDef())
		{
			// printf("1");
			tmp->next = FuncDef();
			if (tmp->next == NULL)
				return NULL;
		}
		else
		{
			// printf("2");
			// return NULL;
			tmp->next = Decl();
			if (tmp->next == NULL)
				return NULL;
		}
	}

	while (tok > 0)
	{
		tmp = tmp->next;
		if (tok == COMMENT || tok == COMMENTS)
		{
			advance();
		}
		else if (tok == CONST)
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
			}
			else
			{
				tmp->left = Decl();
			}
		}
		else
		{
			printf("118ERROR!\n");
			exit(0);
		}
	}
	return l;
}

// 声明 Decl → ConstDecl | VarDecl
past Decl()
{
	past l = newAstNode_param("", "Decl", NULL, NULL, NULL);
	if (tok == CONST)
	{
		l->left = ConstDecl();
	}
	else if (tok == INT)
	{
		l->left = VarDecl();
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
		tmp->left = ConstDef();
		tmp = tmp->left;
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
		past idn = newIdent(strdup(yytext), NULL);
		tmp->left = idn;
		tmp = tmp->left;
		advance();
		while (tok == MLP)
		{
			advance();
			tmp->next = ConstExp();
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
			tmp->left = ConstInitVal();
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
	past list = newAstNode_param("", "list", NULL, NULL, NULL);
	past l = newAstNode_param("", "ConstInitVal", list, NULL, NULL);
	list->value.ivalue = 0;
	past tmp = list;
	if (tok == LP)
	{
		advance();
		if (tok == IDN || tok == ADD || tok == SUB || tok == NOT || tok == SLP || tok == NUM)
		{
			tmp->left = ConstInitVal();
			tmp = tmp->left;
			while (tok == COMMA)
			{
				advance();
				tmp->next = ConstInitVal();
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

	tmp->value.ivalue++;
	tmp->left = ConstExp();
	return l;
}

// 变量声明 VarDecl → BType VarDef { ',' VarDef } ';'
past VarDecl()
{
	past type = BType();
	past list = newAstNode_param("", "list", NULL, NULL, NULL);
	past l = newAstNode_param("", "VarDecl", type, list, NULL);
	l->right->value.ivalue = 0;
	past var = VarDef();
	list->value.ivalue++;
	list->left = var;
	past tmp = var;
	while (tok == COMMA)
	{
		advance();
		list->value.ivalue++;
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
	past root;
	if (tok == IDN)
	{
		past l = newAstNode_param(strdup(yytext), "VarDecl", NULL, NULL, NULL);
		root = newAstNode_param(".", "VarDef", l, NULL, NULL);
		advance();
		past list = newAstNode_param("", "list", NULL, NULL, NULL);
		root->left->left = list;
		root->left->left->value.ivalue = 0;
		past tmp = NULL;
		if (tok == MLP)
		{
			root->left->nodeType = "ArrayDecl";
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
	}
	else
	{
		printf("281ERROR:Expect a Ident\n");
		exit(0);
	}
	return root;
}

// 变量初值 InitVal → Exp | '{' [ InitVal { ',' InitVal } ] '}'
past InitVal()
{
	past list = newAstNode_param("", "list", NULL, NULL, NULL);
	past l = newAstNode_param("", "InitVal", list, NULL, NULL);
	list->value.ivalue = 0;
	past tmp = list;
	if (tok == LP)
	{
		advance();
		if (tok == IDN || tok == NUM || tok == SUB || tok == ADD || tok == NOT || tok == SLP)
		{
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
	list->value.ivalue++;
	list->left = Exp();
	return l;
}

// 函数定义 FuncDef → FuncType Ident '(' [FuncFParams] ')' Block
past FuncDef()
{
	past l = newAstNode();
	l->nodeType = "FuncDef";
	past type = FuncType();
	l->left = type;
	if (tok == IDN || tok == MAIN)
	{
		past idn = newIdent(strdup(yytext), NULL);
		type->left = idn;
		advance();
		if (tok == SLP)
		{
			advance();
			if (tok == INT)
			{
				past FuncFParam = FuncFParams();
				idn->left = FuncFParam;
				if (tok == SRP)
				{
					advance();
					past block = Block();
					l->right = block;
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
					past block = Block();
					l->right = block;
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
	return l;
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
			printf("748Error: astFuncFParam()\n");
			return NULL;
		}
		head = head->next;
	}
	return root;
}

// 函数形参 FuncFParam → BType Ident ['[' ']' { '[' Exp ']' }]
past FuncFParam()
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
						printf("Error: astExp()\n");
						return NULL;
					}
					tmp = tmp->next;
					root->right->left->value.ivalue++;
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
	past list = newAstNode_param("", "list", NULL, NULL, NULL);
	list->value.ivalue = 0;
	past block = newAstNode_param("", "BlockDecl", NULL, NULL, NULL);
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
				printf("Error: astBlockItem()\n");
				return NULL;
			}
			tmp = block->left;
		}
		while (tok == INT || tok == CONST || tok == IDN || tok == RETURN || tok == IF || tok == CONTINUE || tok == WHILE || tok == BREAK || tok == NUM || tok == ADD || tok == SUB || tok == NOT || tok == SLP)
		{
			list->value.ivalue++;
			tmp->left = BlockItem();
			if (tmp->left == (void *)(0x10) || tmp->left == NULL)
			{
				printf("Error: astBlockItem()\n");
				return NULL;
			}
			tmp = tmp->left;
		}
		if (tok == RP)
		{
			advance();
		}
		else
		{
			printf("447ERROR:Expect a '}'\n");
			exit(0);
		}
	}
	else
	{
		printf("453ERROR:Expect a '{'\n");
		exit(0);
	}
	return list;
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
	if (tok == IF)
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
			if (tok == SRP)
			{
				advance();
				root->right->left = Stmt();
				if (tok == ELSE)
				{
					advance();
					if (root->right->right = Stmt())
					{
						root->right->value.ivalue++;
						return root;
					}
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
			past exp = Exp();
			if (flag == 1)
			{
				root->left = exp;
				root->nodeType = "ArrayDecl";
				tmp = exp;
				flag++;
			}
			else
			{
				tmp->next = exp;
				tmp = tmp->next;
			}
			if (tok == MRP)
			{
				advance();
				return root;
			}
			else
			{
				printf("650ERROR:Expect a ']'\n");
				exit(0);
			}
		}
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
		ue = newIdent(strdup(yytext), NULL);
		advance();
		if (tok == SLP)
		{
			advance();
			if (tok == IDN || tok == NUM || tok == ADD || tok == SUB || tok == NOT || tok == SLP)
			{
				past root = newAstNode();
				root->nodeType = "FuncDecl";
				root->left = ue;
				past funcRP = FuncRParams();
				root->right = funcRP;
			}
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
		// else
		// {
		// 	while (tok == MLP)
		// 	{
		// 		advance();
		// 		Exp();
		// 		if (tok == MRP)
		// 		{
		// 			advance();
		// 		}
		// 		else
		// 		{
		// 			printf("744ERROR:Expect a ']'\n");
		// 			exit(0);
		// 		}
		// 	}
		// }
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
		advance();
		return ue;
	}
	else if (tok == SLP || tok == NUM)
	{
		return PrimaryExp();
	}
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

// int main(int argc, char **argv)
// {
// 	FILE *fp;
// 	if (argv[1])
// 	{
// 		if ((fp = fopen(argv[1], "r")))
// 		{
// 			printf("reading data from file:%s\n", argv[1]);
// 		}
// 		else
// 		{
// 			printf("Input Format Wrong!\n");
// 			return 0;
// 		}
// 	}
// 	else
// 	{
// 		printf("Input Format Wrong!\n");
// 		return 0;
// 	}

// 	put_scan(fp);
// 	advance();
// 	CompUnit();
// 	printf("program done successfully!\n");
// 	return 0;
// }

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
	showAst(root, 0);
	freeAst(root);
	printf("program done successfully!\n");

	return 0;
}