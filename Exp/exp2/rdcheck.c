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

void CompUnit();
void Decl();
void FuncDef();
void ConstDecl();
void VarDecl();
void VarDef();
void BType();
void ConstDef();
void ConstInitVal();
void InitVal();
void FuncType();
void FuncFParams();
void FuncFParam();
void Block();
void BlockItem();
void Stmt();
void Exp();
void Cond();
void LVal();
void PrimaryExp();
void Number();
void UnaryExp();
void UnaryOp();
void FuncRParams();
void MulExp();
void AddExp();
void RelExp();
void EqExp();
void LAndExp();
void LOrExp();
void ConstExp();

int tok;

void advance()
{
	tok = yylex();
	if (tok == 0)
		return;
	printf("tok: %s\n", yytext);
}

// 编译单元 CompUnit → [ CompUnit ] ( Decl | FuncDef )
void CompUnit()
{
	while (tok > 0)
	{
		if (tok == COMMENT || tok == COMMENTS)
		{
			advance();
		}
		else if (tok == CONST)
		{
			Decl();
		}
		else if (tok == VOID)
		{
			FuncDef();
		}
		else if (tok == INT)
		{
			if (isFuncDef())
			{
				FuncDef();
			}
			else
			{
				Decl();
			}
		}
		else
		{
			printf("118ERROR!%d\n", tok);
			exit(0);
		}
	}
}

// 声明 Decl → ConstDecl | VarDecl
void Decl()
{
	if (tok == CONST)
	{
		ConstDecl();
	}
	else if (tok == INT)
	{
		VarDecl();
	}
	else
	{
		return;
	}
}

// 常量声明 ConstDecl → 'const' BType ConstDef { ',' ConstDef } ';'
void ConstDecl()
{
	if (tok == CONST)
	{
		advance();
		BType();
		ConstDef();
		while (tok == COMMA)
		{
			advance();
			ConstDef();
		}
		if (tok == SEMI)
		{
			advance();
		}
		else
		{
			printf("ERROR:Expect a ';'\n");
			exit(0);
		}
	}
}

// 基本类型 BType → 'int'
void BType()
{
	if (tok == INT)
		advance();
}

// 常数定义 ConstDef → Ident { '[' ConstExp ']' } '=' ConstInitVal
void ConstDef()
{
	if (tok == IDN)
	{
		advance();
		while (tok == MLP)
		{
			advance();
			ConstExp();
			if (tok == MRP)
			{
				advance();
			}
			else
			{
				printf("ERROR:Expect a ']'\n");
				exit(0);
			}
		}
		if (tok == ASSIGN)
		{
			advance();
			ConstInitVal();
		}
		else
		{
			printf("ERROR:Expect a '='\n");
			exit(0);
		}
	}
}

// 常量初值 ConstInitVal → ConstExp
// 						| '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
void ConstInitVal()
{
	if (tok == LP)
	{
		advance();
		if (tok == IDN || tok == ADD || tok == SUB || tok == NOT || tok == SLP || tok == NUM)
		{
			ConstInitVal();
			while (tok == COMMA)
			{
				advance();
				ConstInitVal();
			}
		}
		if (tok == RP)
		{
			advance();
		}
		else
		{
			printf("ERROR:Expect a '}'\n");
			exit(0);
		}
	}

	ConstExp();
}

// 变量声明 VarDecl → BType VarDef { ',' VarDef } ';'
void VarDecl()
{
	BType();
	VarDef();
	while (tok == COMMA)
	{
		advance();
		VarDef();
	}
	if (tok == SEMI)
	{
		advance();
	}
}

// 变量定义 VarDef → Ident { '[' ConstExp ']' }
// 					| Ident { '[' ConstExp ']' } '=' InitVal
void VarDef()
{
	if (tok == IDN)
	{
		advance();
		while (tok == MLP)
		{
			advance();
			ConstExp();
			if (tok == MRP)
			{
				advance();
			}
			else
			{
				printf("ERROR:Expect a ']'\n");
				exit(0);
			}
		}
		if (tok == ASSIGN)
		{
			advance();
			InitVal();
		}
	}
	else
	{
		printf("ERROR:Expect a Ident\n");
		exit(0);
	}
}

// 变量初值 InitVal → Exp | '{' [ InitVal { ',' InitVal } ] '}'
void InitVal()
{
	if (tok == LP)
	{
		advance();
		if (tok == IDN || tok == NUM || tok == SUB || tok == ADD || tok == NOT || tok == SLP)
		{
			InitVal();
			while (tok == COMMA)
			{
				advance();
				InitVal();
			}
		}
		if (tok == RP)
		{
			advance();
		}
		else
		{
			printf("ERROR:Expect a '}'\n");
			exit(0);
		}
	}

	Exp();
}

// 函数定义 FuncDef → FuncType Ident '(' [FuncFParams] ')' Block
void FuncDef()
{
	FuncType();
	if (tok == IDN)
	{
		advance();
		if (tok == SLP)
		{
			advance();
			if (tok == INT)
			{
				FuncFParams();
				if (tok == SRP)
				{
					advance();
					Block();
				}
				else
				{
					printf("ERROR:Expect a ')'\n");
					exit(0);
				}
			}
			else
			{
				if (tok == SRP)
				{
					advance();
					Block();
				}
			}
		}
		else
		{
			printf("ERROR:Expect a '('\n");
		}
	}
	else
	{
		printf("ERROR:Expect a Ident\n");
		exit(0);
	}
}

// 函数类型 FuncType → 'void' | 'int'
void FuncType()
{
	if (tok == VOID)
	{
		advance();
	}
	else if (tok == INT)
	{
		advance();
	}
	else
	{
		printf("ERROR:Expect FuncType with 'void' or 'int'\n");
		exit(0);
	}
}

// 函数形参表 FuncFParams → FuncFParam { ',' FuncFParam }
void FuncFParams()
{
	FuncFParam();
	while (tok == COMMA)
	{
		advance();
		FuncFParam();
	}
}

// 函数形参 FuncFParam → BType Ident ['[' ']' { '[' Exp ']' }]
void FuncFParam()
{
	BType();
	if (tok == IDN)
	{
		advance();
		if (tok == MLP)
		{
			advance();
			if (tok == MRP)
			{
				advance();
				while (tok == MLP)
				{
					advance();
					Exp();
					if (tok == MRP)
					{
						advance();
					}
					else
					{
						printf("ERROR:Expect a ']'\n");
						exit(0);
					}
				}
			}
			else
			{
				printf("ERROR:Expect a ']'\n");
				exit(0);
			}
		}
	}
	else
	{
		printf("ERROR:Expect a Ident\n");
		exit(0);
	}
}

// 语句块 Block → '{' { BlockItem } '}'
void Block()
{
	if (tok == LP)
	{
		advance();
		while (tok == INT || tok == CONST || tok == IDN || tok == RETURN || tok == IF || tok == CONTINUE || tok == WHILE || tok == BREAK || tok == NUM || tok == ADD || tok == SUB || tok == NOT || tok == SLP)
		{
			BlockItem();
		}
		if (tok == RP)
		{
			advance();
		}
		else
		{
			printf("ERROR:Expect a '}'\n");
			exit(0);
		}
	}
	else
	{
		printf("ERROR:Expect a '{'\n");
		exit(0);
	}
}

// 语句块项 BlockItem → Decl | Stmt
void BlockItem()
{
	if (tok == CONST || tok == INT)
	{
		Decl();
	}
	else
	{
		Stmt();
	}
}

// 语句 Stmt → LVal '=' Exp ';' | [Exp] ';' | Block
// 			| 'if' '( Cond ')' Stmt [ 'else' Stmt ]
// 			| 'while' '(' Cond ')' Stmt
// 			| 'break' ';' | 'continue' ';'
// 			| 'return' [Exp] ';'
void Stmt()
{
	if (tok == IF)
	{
		advance();
		if (tok == SLP)
		{
			advance();
			Cond();
			if (tok == SRP)
			{
				advance();
				Stmt();
				if (tok == ELSE)
				{
					advance();
					Stmt();
				}
			}
			else
			{
				printf("ERROR:Expect a ')'\n");
				exit(0);
			}
		}
		else
		{
			printf("ERROR:Expect a '('\n");
			exit(0);
		}
	}
	else if (tok == WHILE)
	{
		advance();
		if (tok == SLP)
		{
			advance();
			Cond();
			if (tok == SRP)
			{
				advance();
				Stmt();
			}
			else
			{
				printf("ERROR:Expect a ')'\n");
				exit(0);
			}
		}
		else
		{
			printf("ERROR:Expect a '('\n");
			exit(0);
		}
	}
	else if (tok == BREAK)
	{
		advance();
		if (tok == SEMI)
		{
			advance();
		}
		else
		{
			printf("ERROR:Expect a ';'\n");
			exit(0);
		}
	}
	else if (tok == CONTINUE)
	{
		advance();
		if (tok == SEMI)
		{
			advance();
		}
		else
		{
			printf("ERROR:Expect a ';'\n");
			exit(0);
		}
	}
	else if (tok == RETURN)
	{
		advance();
		if (tok == IDN || tok == NUM || tok == ADD || tok == SUB || tok == NOT || tok == SLP)
		{
			Exp();
		}

		if (tok == SEMI)
		{
			advance();
		}
		else
		{
			printf("ERROR:Expect a ';'\n");
			exit(0);
		}
	}
	else if (tok == LP)
	{
		Block();
	}
	else if (tok == IDN)
	{
		char *s0 = strdup(yytext);
		tok = yylex();
		put_back(yytext);
		put_back(s0);

		if (tok == SLP)
		{
			tok = yylex();
			Exp();
			if (tok == SEMI)
			{
				advance();
			}
		}
		else
		{
			tok = yylex();
			LVal();
			if (tok == ASSIGN)
			{
				advance();
				Exp();
				if (tok == SEMI)
					advance();
			}
		}
	}
	else
	{
		if (tok == NUM || tok == ADD || tok == SUB || tok == NOT || tok == SLP)
		{
			Exp();
		}

		if (tok == SEMI)
		{
			advance();
		}
		else
		{
			printf("ERROR:Expect a ';'\n");
			exit(0);
		}
	}
}

// 表达式 Exp → AddExp 注：SysY 表达式是 int 型表达式
void Exp()
{
	AddExp();
}

// 条件表达式 Cond → LOrExp
void Cond()
{
	LOrExp();
}

// 左值表达式 LVal → Ident {'[' Exp ']'}
void LVal()
{
	if (tok == IDN)
	{
		advance();
		while (tok == MLP)
		{
			advance();
			Exp();
			if (tok == MRP)
			{
				advance();
			}
			else
			{
				printf("ERROR:Expect a ']'\n");
				exit(0);
			}
		}
	}
	else
	{
		printf("ERROR:Expect LVal with 'Ident'\n");
		exit(0);
	}
}

// 基本表达式 PrimaryExp → '(' Exp ')' | LVal | Number
void PrimaryExp()
{
	if (tok == SLP)
	{
		advance();
		Exp();
		if (tok == SRP)
		{
			advance();
		}
		else
		{
			printf("ERROR:Expect a ')'\n");
			exit(0);
		}
	}
	else if (tok == NUM)
	{
		Number();
	}
	else if (tok == IDN)
	{
		LVal();
	}
	else
	{
		printf("ERROR:Expect a '('\n");
		exit(0);
	}
}

// 数值 Number → IntConst
void Number()
{
	if (tok == NUM)
	{
		advance();
	}
	else
	{
		printf("ERROR:Expect a number\n");
		exit(0);
	}
}

// 一元表达式 UnaryExp → PrimaryExp | Ident '(' [FuncRParams] ')'
// 						| UnaryOp UnaryExp
void UnaryExp()
{
	if (tok == IDN)
	{
		advance();
		if (tok == SLP)
		{
			advance();
			if (tok == IDN || tok == NUM || tok == ADD || tok == SUB || tok == NOT || tok == SLP)
			{
				FuncRParams();
			}
			if (tok == SRP)
			{
				advance();
			}
			else
			{
				printf("ERROR:Expect a ')'\n");
				exit(0);
			}
		}
		else
		{
			while (tok == MLP)
			{
				advance();
				Exp();
				if (tok == MRP)
				{
					advance();
				}
				else
				{
					printf("ERROR:Expect a ']'\n");
					exit(0);
				}
			}
		}
	}
	else if (tok == ADD || tok == SUB || tok == NOT)
	{
		UnaryOp();
		UnaryExp();
	}
	else if (tok == SLP || tok == NUM)
	{
		PrimaryExp();
	}
}

// 单目运算符 UnaryOp → '+' | '−' | '!' 注：'!'仅出现在条件表达式中
void UnaryOp()
{
	if (tok == ADD || tok == SUB || tok == NOT)
	{
		advance();
	}
	else
	{
		printf("ERROR:Expect UnaryOp Wrong\n");
		exit(0);
	}
}

// 函数实参表 FuncRParams → Exp { ',' Exp }
void FuncRParams()
{
	if (tok == IDN || tok == NUM || tok == ADD || tok == SUB || tok == NOT || tok == SLP)
	{
		Exp();
		while (tok == COMMA)
		{
			advance();
			Exp();
		}
	}
	else
	{
		printf("ERROR:Expect FuncRParams Wrong\n");
		exit(0);
	}
}

// 乘除模表达式 MulExp → UnaryExp | MulExp ('*' | '/' | '%') UnaryExp
void MulExp()
{
	UnaryExp();
	while (tok == MUL || tok == DIV || tok == MOD)
	{
		advance();
		UnaryExp();
	}
}

// 加减表达式 AddExp → MulExp | AddExp ('+' | '−') MulExp
void AddExp()
{
	MulExp();
	while (tok == ADD || tok == SUB)
	{
		advance();
		MulExp();
	}
}

// 关系表达式 RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
void RelExp()
{
	AddExp();
	while (tok == LT || tok == GT || tok == LE || tok == GE)
	{
		advance();
		AddExp();
	}
}

// 相等性表达式 EqExp → RelExp | EqExp ('==' | '!=') RelExp
void EqExp()
{
	RelExp();
	while (tok == NE || tok == EQ)
	{
		advance();
		RelExp();
	}
}

// 逻辑与表达式 LAndExp → EqExp | LAndExp '&&' EqExp
void LAndExp()
{
	EqExp();
	while (tok == AND)
	{
		advance();
		EqExp();
	}
}

// 逻辑或表达式 LOrExp → LAndExp | LOrExp '||' LAndExp
void LOrExp()
{
	LAndExp();
	while (tok == OR)
	{
		advance();
		LAndExp();
	}
}

// 常量表达式 ConstExp → AddExp
void ConstExp()
{
	AddExp();
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
	advance();
	CompUnit();
	printf("program done successfully!\n");
	return 0;
}
