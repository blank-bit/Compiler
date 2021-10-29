%{
enum yytokentype{
        INT = 1,RETURN,VOID,CONST,IF,ELSE,CONTINUE,WHILE,MAIN,BREAK,IDN,NUM,ADD,
        SUB,MUL,DIV,MOD,LT,GT,LE,GE,ASSIGN,NE,EQ,OR,AND,NOT,LP,RP,SLP,SRP,MLP,MRP,COMMA,SEMI,COMMENT,COMMENTS
};
int yylval;
%}

DIGIT   [0-9]
NOTE_S     \/\/(.)*\n
NOTE_M     \/\*(.|\n)*?\*\/
IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*
SPACE      [ \t\r\n]+


%%
"int"                       {return INT;}
"return"                    {return RETURN;}
"void"                      {return VOID;}
"const"                     {return CONST;}
"if"                        {return IF;}
"else"                      {return ELSE;}
"continue"                  {return CONTINUE;}
"while"                     {return WHILE;}
"main"                      {return MAIN;}
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
{DIGIT}+"."*{DIGIT}* 	    {yylval=atoi(yytext);return NUM;}
{IDENTIFIER}                {return IDN;}

%%

int yywrap(){
    return 1;
}

main(int argc, char **argv){
    int tok;

	while(tok = yylex()) 
	{
		printf("%d", tok);
		if(tok == NUM) 
			printf(" = %d\n", yylval);
		else if(tok == IDN)
            printf(" = %s\n", yytext);
        else
			printf("\n");
	}
}