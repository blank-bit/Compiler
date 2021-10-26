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
#define KEY_NUM 43

FILE *in;
char token[MAX]; //字符组，存放构成单词符号的字符串

typedef struct Key_word // 保留字和特殊符号表结构
{
    char keyWord[MAX]; // 单词符号
    char keySign[MAX]; //助记符
} Key_word;            //初始化保留字表

Key_word Key[KEY_NUM] = {

    {"int", "int"},           // 0保留字
    {"do", "do"},             // 1
    {"return", "return"},     // 2
    {"void", "void"},         // 3
    {"const", "const"},       // 4
    {"for", "for"},           // 5
    {"if", "if"},             // 6
    {"else", "else"},         // 7
    {"continue", "continue"}, // 8
    {"while", "while"},       // 9
    {"main", "main"},         // 10
    {"break", "break"},       // 11

    {"", "id"},  // 12标识符
    {"", "num"}, // 13数字（常数和实数）

    {"+", "+"},    // 14 界符运算符
    {"-", "-"},    // 15
    {"*", "*"},    // 16
    {"/", "/"},    // 17
    {"%", "%"},    // 18
    {"<", "<"},    // 19
    {">", ">"},    // 20
    {"<=", "<="},  // 21
    {">=", ">="},  // 22
    {"=", "="},    // 23
    {"!=", "!="},  // 24
    {"+=", "+="},  // 25
    {"-=", "-="},  // 26
    {"*=", "*="},  // 27
    {"/=", "/="},  // 28
    {"%=", "%="},  // 29
    {"|", "|"},    //30
    {"&", "&"},    //31
    {"||", "or"},  // 32
    {"&&", "and"}, // 33
    {"!", "not"},  // 34
    {"{", "{"},    // 35
    {"}", "}"},    // 36
    {"(", "("},    // 37
    {")", ")"},    // 38
    {"[", "["},    // 39
    {"]", "]"},    // 40
    {",", ","},    // 41
    {";", ";"},    // 42
};

int Init()
{
    char file[MAX];
    // printf("输入要分析的文件：");
    // scanf("%s", file);
    // printf("\n%s", file);
    if ((in = fopen("case.c", "r")) == NULL)
    {
        printf("cannot open file!\n");
        return 0;
    }
    return 0;
}

void Output(char s[], char token[])
{
    printf("< %s, %s >\n", s, token);
}

void IsAlpha()
{
    int i;

    for (i = 0; i < 12; i++)
    {
        if (strcmp(token, Key[i].keyWord) == 0)
        {
            Output("关键字", token);
            break;
        }
    }

    if (i == 12)
        Output("标识符", token);
}

void IsNumber()
{
    Output("常数", token);
}

void IsAnotation()
{
    char ch, pre;
    ch = fgetc(in);

    if (ch == '=')
    {
        Output("运算符", "/=");
    }
    else if (ch == '/')
    {
        ch = fgetc(in);
        while (ch != '\n')
            ch = fgetc(in);
        Output("界符", "//");
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

        Output("界符", "/**/");
        // printf("界符,/**/");
    }
    else
    {
        // int fseek( FILE *stream, long offset, int origin );stream为文件指针 offset为偏移量，正数表示正向偏移，负数表示负向偏移
        // SEEK_SET： 文件开头 SEEK_CUR： 当前位置 SEEK_END： 文件结尾
        fseek(in, -1, SEEK_CUR);
        Output("运算符", "/");
    }
}

void IsOther()
{
    int i = 1;
    int j = 0;
    char ch;

    if (token[0] == '<' || token[0] == '>' || token[0] == '!' || token[0] == '%' ||
        token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '=')
    {
        ch = fgetc(in);
        if (ch == '=')
        {
            token[i++] = ch;
            token[i++] = '\0';
            Output("运算符", token);
        }
        else
        {
            token[i++] = '\0';
            Output("运算符", token);
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
            Output("界符", token);
        }
        else
        {
            token[i++] = '\0';
            Output("界符", token);
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
            Output("界符", token);
        }
        else
        {
            token[i++] = '\0';
            Output("界符", token);
            fseek(in, -1, SEEK_CUR);
        }
        return;
    }

    token[i++] = '\0';

    for (i = 35; i < 43; i++)
    {
        if (strcmp(token, Key[i].keyWord) == 0)
        {
            Output("界符", token);
            // break;
            return;
        }
    }

    if (i == 43)
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
        else if (ch == '/') // 区分 "/" , "/=" , "/*"
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

int main()
{
    //初始化
    Init();
    // printf("Init");
    Scanner();
    // printf("\n 共有 %d 个错误 \n", num);
    // show();
    fclose(in);
    // fclose(out);
    return 0;
}