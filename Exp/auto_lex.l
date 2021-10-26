%{
#include <stdio.h>
#include <stdlib.h>

%}

INTERGER   [0-9]
OCTALCONS  0[0-7]+
HEXCONS    0[xX][0-9a-fA-F]+
NOTE_S     \/\/(.)*\n
NOTE_M     \/\*(.|\n)*?\*\/
IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*


%%

"int"                       {printf("258\n");}
"continue"                  {printf("265\n");}
"const"                     {printf("260\n");}
"else"                      {printf("262\n");}
"if"                        {printf("261\n");}
"return"                    {printf("266\n");}
"void"                      {printf("259\n");}
"while"                     {printf("263\n");}
"break"                     {printf("264\n");}
"+"                         {printf("267\n");}
"-"                         {printf("268\n");}
"*"                         {printf("269\n");}
"/"                         {printf("270\n");}
"%"                         {printf("271\n");}
"<"                         {printf("272\n");}
"<="                        {printf("273\n");}
">"                         {printf("274\n");}
">="                        {printf("275\n");}
"!="                        {printf("276\n");}
"=="                        {printf("277\n");}
"!"                         {printf("278\n");}
"&&"                        {printf("279\n");}
"||"                        {printf("280\n");}
"="                         {printf("281\n");}
"("                         {printf("282\n");}
")"                         {printf("283\n");}
"{"                         {printf("284\n");}
"}"                         {printf("285\n");}
"["                         {printf("286\n");}
"]"                         {printf("287\n");}
","                         {printf("288\n");}
";"                         {printf("289\n");}
" "                         {}
"\t"                        {}
"\n"                        {}

{NOTE_S}*                   {}
{NOTE_M}*                   {}
{INTERGER}+"."*{INTERGER}* 	{printf("257 %s\n",yytext);}
{OCTALCONS}                 {printf("257 %s\n",yytext);}
{HEXCONS}                   {printf("257 %s\n",yytext);}
{IDENTIFIER}                {printf("256 %s\n",yytext);}

%%

int yywrap(){
    return 1;
}

void main(int argc, char **argv){
    yylex();
}
