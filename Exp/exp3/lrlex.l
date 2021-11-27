%{
	#include "parser.tab.h"
	typedef union {
	int type_int;
	char* type_char;
	struct _ast *past;
	} YYLVAL;
#define YYSTYPE YYLVAL
	int line = 1;
%}

%option yylineno

DIGIT   [0-9]
NOTE_S     \/\/(.)*\n
NOTE_M     \/\*(.|\n)*?\*\/
IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*
SPACE      [ \t\r\n]+
/* EOL        \n */

%%
"int"                       {return INT;}
"return"                    {return RETURN;}
"void"                      {return VOID;}
"const"                     {return CONST;}
"if"                        {return IF;}
"else"                      {return ELSE;}
"continue"                  {return CONTINUE;}
"while"                     {return WHILE;}
"break"                     {return BREAK;}
"+"                         {return ADD;}
"-"                         {return SUB;}
"*"                         {return MUL;}
"/"                         {return DIV;}
"%"                         {return MOD;}
"<"                         {return LT;}
">"                         {return GT;}
"<="                        {return LE;}
">="                        {return GE;}
"="                         {return ASSIGN;}
"!="                        {return NE;}
"=="                        {return EQ;}
"||"                        {return OR;}
"&&"                        {return AND;}
"!"                         {return NOT;}
"{"                         {return LP;}
"}"                         {return RP;}
"("                         {return SLP;}
")"                         {return SRP;}
"["                         {return MLP;}
"]"                         {return MRP;}
","                         {return COMMA;}
";"                         {return SEMI;}

{SPACE}                     {}
{NOTE_S}*                   {return COMMENT;}
{NOTE_M}*                   {return COMMENTS;}
{DIGIT}+"."*{DIGIT}* 	    {yylval.type_int = atoi(yytext);return IntConst;}
{IDENTIFIER}                {yylval.type_char = strdup(yytext);return Ident;}


%%

int yywrap(){
    return 1;
}

int isFuncDef(){
	register int c,j;
	int tok = yylex();
	if(tok == Ident){
		c = input();
		if(c == '(' ){
			unput(c);
			yyless(0);
			return 1;
		}else if(c == ' ')
		{
			c = input();
			if(c == '(' ){
				unput(' ');
				unput(c);
				yyless(0);
				return 1;
			}
			unput(' ');
			unput(c);
			yyless(0);
			return 0;
		}else {
				unput(c);
				yyless(0);
				return 0;
		}
	}else{
		yyless(0);
		return 0;
	}
	
	return 0;	
}

void put_back(char* yytext)
{
	char *Str = strdup(yytext);
	int len = strlen(Str);
	int i = len - 1;
	while(i>=0){
		unput(Str[i]);
		i--;
	}
	unput(' ');
}