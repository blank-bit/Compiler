/*********************************************
 @Author       : Mr.Wang
 @Date         : 2021-10-15 16:16:53
 @FilePath     : /lex.c
 @Description  : message
*********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 30
#define KEY_NUM 37

FILE *in;
char token[MAX]; //字符组，存放构成单词符号的字符串

typedef struct Key_word // 保留字和特殊符号表结构
{
    char keyWord[MAX]; // 单词符号
    char keySign[MAX]; //助记符
    char keyvalue[MAX];
} Key_word; //初始化保留字表

Key_word Key[KEY_NUM] = {

    {"int", "INT", "-"},           // 0保留字
    {"return", "RETURN", "-"},     // 1
    {"void", "VOID", "-"},         // 2
    {"const", "CONST", "-"},       // 3
    {"if", "IF", "-"},             // 4
    {"else", "ELSE", "-"},         // 5
    {"continue", "CONTINUE", "-"}, // 6
    {"while", "WHILE", "-"},       // 7
    {"main", "MAIN", "-"},         // 8
    {"break", "BREAK", "-"},       // 9

    {"", "IDN", ""}, // 10标识符
    {"", "NUM", ""}, // 11数字（常数和实数）

    {"+", "ADD", "+"},            // 12 界符运算符
    {"-", "SUB", "-"},            // 13
    {"*", "MUL", "*"},            // 14
    {"/", "DIV", "/"},            // 15
    {"%", "MOD", "%"},            // 16
    {"<", "LT", "<"},             // 17
    {">", "GT", ">"},             // 18
    {"<=", "LE", "<="},           // 19
    {">=", "GE", ">="},           // 20
    {"=", "ASSIGN", "="},         // 21
    {"!=", "NE", "!="},           // 22
    {"==", "EQ", "=="},           // 23
    {"||", "OR", "||"},           // 24
    {"&&", "AND", "&&"},          // 25
    {"!", "NOT", "!"},            // 26
    {"{", "LP", "{"},             // 27
    {"}", "RP", "}"},             // 28
    {"(", "SLP", "("},            // 29
    {")", "SRP", ")"},            // 30
    {"[", "MLP", "["},            // 31
    {"]", "MRP", "]"},            // 32
    {",", "COMMA", ","},          // 33
    {";", "SEMI", ";"},           // 34
    {"//", "COMMENT", "//"},      // 35
    {"/**/", "COMMENTS", "/**/"}, // 36
};

void Output(char token[], int m)
{
    if (m == 11 || m == 10)
        printf("< %s, %s >\n", Key[m].keySign, token);
    else
        printf("< %s, %s >\n", Key[m].keySign, Key[m].keyvalue);
}

void IsAlpha()
{
    int i;

    for (i = 0; i < 10; i++)
    {
        if (strcmp(token, Key[i].keyWord) == 0)
        {
            Output(token, i);
            break;
        }
    }

    if (i == 10)
        Output(token, i);
}

void IsNumber()
{
    Output(token, 11);
}

void IsAnotation()
{
    char ch, pre;
    ch = fgetc(in);

    if (ch == '/')
    {
        ch = fgetc(in);
        while (ch != '\n')
            ch = fgetc(in);
        Output("//", 35);
    }
    else if (ch == '*')
    {
        pre = fgetc(in);
        ch = fgetc(in);

        while (!(ch == '/' && pre == '*'))
        {
            pre = ch;
            ch = fgetc(in);
        }

        Output("/**/", 36);
    }
    else
    {
        // int fseek( FILE *stream, long offset, int origin );stream为文件指针 offset为偏移量，正数表示正向偏移，负数表示负向偏移
        // SEEK_SET： 文件开头 SEEK_CUR： 当前位置 SEEK_END： 文件结尾
        fseek(in, -1, SEEK_CUR);
        Output("/", 15);
    }
}

void IsOther()
{
    int i = 1;
    int j = 0;
    int n;
    char ch;

    if (token[0] == '<' || token[0] == '>' || token[0] == '!' || token[0] == '%' ||
        token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '=')
    {
        ch = fgetc(in);
        if (ch == '=')
        {
            token[i++] = ch;
            token[i++] = '\0';
            for (n = 19; n < 24; n++)
                if (strcmp(Key[n].keyWord, token) == 0)
                    break;
            Output(token, n);
        }
        else
        {
            token[i++] = '\0';
            for (n = 12; n < 27; n++)
                if (strcmp(Key[n].keyWord, token) == 0)
                    break;
            Output(token, n);
            fseek(in, -1, SEEK_CUR);
        }
        return;
    }
    else if (token[0] == '|')
    {
        ch = fgetc(in);
        if (ch == '|')
        {
            token[i++] = ch;
            token[i++] = '\0';
            Output(token, 24);
        }
        else
        {
            printf("非法字符：%s\n", token);
            fseek(in, -1, SEEK_CUR);
        }
        return;
    }
    else if (token[0] == '&')
    {
        ch = fgetc(in);
        if (ch == '&')
        {
            token[i++] = ch;
            token[i++] = '\0';
            Output(token, 25);
        }
        else
        {
            printf("非法字符：%s\n", token);
            fseek(in, -1, SEEK_CUR);
        }
        return;
    }

    token[i++] = '\0';

    for (i = 27; i < 35; i++)
    {
        if (strcmp(token, Key[i].keyWord) == 0)
        {
            Output(token, i);
            return;
        }
    }

    if (i == 35)
        printf("非法字符：%s\n", token);
}

void Scanner()
{
    char ch;
    int i;

    ch = fgetc(in);
    while (!feof(in))
    {
        while (ch == ' ' || ch == '\t' || ch == '\n')
        {
            ch = fgetc(in);
            if (ch == EOF)
                return;
        }

        if (isalpha(ch) || ch == '_')
        {
            i = 1;
            token[0] = ch;
            ch = fgetc(in);
            while (isalnum(ch) || ch == '_')
            {
                token[i++] = ch;
                ch = fgetc(in);
            }
            token[i++] = '\0';
            IsAlpha();
        }
        else if (isdigit(ch))
        {
            i = 1;
            token[0] = ch;
            ch = fgetc(in);
            while (isdigit(ch))
            {

                token[i++] = ch;
                ch = fgetc(in);
            }
            if (ch == '.')
            {
                token[i++] = ch;
                ch = fgetc(in);
                while (isdigit(ch))
                {
                    token[i++] = ch;
                    ch = fgetc(in);
                }
            }
            token[i++] = '\0';
            IsNumber();
        }
        else if (ch == '/') // 区分 "/" , "/*"
        {
            i = 1;
            token[0] = ch;
            IsAnotation();
            if (!feof(in))
                ch = fgetc(in);
        }
        else //特殊字符
        {
            i = 1;
            token[0] = ch;
            IsOther();
            if (!feof(in))
                ch = fgetc(in);
        }
    }
}

int main(int argc, char **argv)
{
    //初始化
    if (argc != 2)
    {
        printf("input file is needed.\n");
        return 0;
    }
    if ((in = fopen(argv[1], "r")) == NULL)
    {
        printf("cannot open file!\n");
        return 0;
    }
    Scanner();
    fclose(in);
    return 0;
}