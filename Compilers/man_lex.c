#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 10     // 保留字符号长度
#define MAXSIZE 45 // 保留字和特殊符号表长度
#define NUM 30     // 标识符和常数的个数

FILE *in, *out; // 指向文件的指针

char infile[MAX];      // 文件名字
char token[MAX];       //字符组，存放构成单词符号的字符串
char Word[NUM][MAX];   // 标识符
char consts[NUM][MAX]; // 常数
char lownum[NUM][MAX] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                         "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                         "21", "22", "23", "24", "25", "26", "27", "28", "29"}; //自定义表

int n = 1;              // 行号
int num = 0;            // 错误统计数
typedef struct Key_word // 保留字和特殊符号表结构
{
    // int keyNum; //种别编码
    char keyWord[MAX];  // 单词符号
    char keySign[MAX];  //助记符
    char keyValue[MAX]; //内码值
} Key_word;             //初始化保留字表

Key_word Key[MAXSIZE] = //保留字和特殊符号数组
    {
        {"int", "int", "-"},           //保留字 1
        {"do", "do", "-"},             // 2
        {"return", "return", "-"},     // 3
        {"void", "void", "-"},         // 4
        {"const", "const", "-"},       // 5
        {"for", "for", "-"},           // 6
        {"if", "if", "-"},             // 7
        {"else", "else", "-"},         // 8
        {"continue", "continue", "-"}, // 9
        {"while", "while", "-"},       // 10
        {"main", "main", "-"},         // 11
        {"break", "break", "-"},       // 12

        {"", "", ""},         // 13空余出来，以便扩充
        {"", "", ""},         // 14
        {"{", "{", "-"},      // 15
        {"}", "}", "-"},      // 16
        {"", "id", ""},       // 17标示符
        {"", "num", ""},      // 18数字（常数和实数）
        {"+", "+", "-"},      // 19
        {"-", "-", "-"},      // 20
        {"*", "*", "-"},      // 21
        {"/", "/", "-"},      // 22
        {"%", "%", "-"},      // 23
        {"(", "(", "-"},      // 24
        {")", ")", "-"},      // 25
        {"[", "[", "-"},      // 26
        {"]", "]", "-"},      // 27
        {"", "", ""},         // 28
        {"<", "rlop", "<"},   // 29
        {">", "rlop", ">"},   // 30
        {"<=", "rlop", "<="}, // 31
        {">=", "rlop", ">="}, // 32
        {"=", "rlop", "="},   // 33
        {"!=", "rlop", "!="}, // 34
        {",", ",", "-"},      // 35
        {";", ";", "-"},      // 36
        {"/=", "/=", "-"},    // 37
        {"+=", "+=", "-"},    // 38
        {"-=", "-=", "-"},    // 39
        {"*=", "*=", "-"},    // 40
        {"%=", "%=", "-"},    // 41
        {"||", "or", "-"},    // 42
        {"&&", "and", "-"},   // 43
        {"!", "not", "-"},    // 44
        {"=", "=", "-"}       // 45
};

//打开关闭文件
int Initscanner()
{
    printf(" 请输入要分析的文件 :\n");
    scanf("%s", infile);
    //读取指定文件
    if ((in = fopen(infile, "r")) == NULL)
    {
        printf("cannot open infile!\n");
        return 0;
    }

    //将文件输出
    if ((out = fopen("word.txt", "w+")) == NULL)
    {
        printf("cannot open outfile!\n");
        return 0;
    }
    return 0;
}

//写入文件
void Output(int n, int m)
{
    //在文件中写入二元组（Key[n].keySign，
    fputs("(", out);
    fputs(Key[n].keySign, out);
    fputs(",", out);
    //如果是关键字或特殊字符，
    //在文件中写入（Key[n].keySign，Key[n].keyValue
    // Key[n].keyValue都是 —
    if (m == -1)
        fputs(Key[n].keyValue, out);
    //如果是标识符，写入标识符在自定义表中对应的下标的值（Key[n].keySign，lownum[m]
    else
        fputs(lownum[m], out); // 下标
    //在文件中写入（Key[n].keySign，Key[n].keyValue）
    fputs(")", out);
}

//扫描指针回退一个字符
void remove1()
{
    // int fseek( FILE *stream, long offset, int origin );stream为文件指针 offset为偏移量，正数表示正向偏移，负数表示负向偏移
    // SEEK_SET： 文件开头 SEEK_CUR： 当前位置 SEEK_END： 文件结尾
    fseek(in, -1, SEEK_CUR);
}

// Isalpha 函数：识别保留字和标识符
int Isalpha()
{
    int i;
    // 关键字，16个关键字，循环匹配
    for (i = 0; i < 16; i++)
    { //判断token是否等于Key[i].keyWord
        if (strcmp(token, Key[i].keyWord) == 0)
        {
            Output(i, -1); //匹配到对应的关键字，传回关键字在数组中的位置，-1表示是关键字
            return 0;
        }
    }
    //标识符 Word[NUM][MAX]
    // i=表示符id
    for (i = 0; i < NUM; i++)
    {
        //如果标示符表【i】位置不为空，并且标示符表【i】中存在该标示符
        if ((Word[i][0] != NULL) && strcmp(token, Word[i]) == 0)
        {
            Output(16, i);
            return 0;
        }
        //如果标示符表【i】位置为空
        if (Word[i][0] == NULL)
        {
            //将token中的值复制到标示符表Word[i]中
            strcpy(Word[i], token);
            Output(16, i);
            return 0;
        }
    }
    return 0;
}
// Isnumber 函数 :consts[NUM][MAX] 识别整数，如有精力，可加入识别实数部分工功能
int Isnumber()
{
    int i;
    for (i = 0; i < NUM; i++)
    {
        //如果数字表(consts[i]位置不为空，并且数字表中存在该数字
        if ((consts[i][0] != NULL) && strcmp(token, consts[i]) == 0)
        {
            Output(17, i);
            return 0;
        }
        // 如果数字表(consts[i]位置为空
        if (consts[i][0] == NULL)
        {
            //将token中的值复制到数字表consts[i]中
            strcpy(consts[i], token);
            Output(17, i);
            return 0;
        }
    }
    return 0;
}

// Isanotation 函数：处理除号 /和注释
void Isanotation() //提示第一个字符已经存为 /了
{
    char ch, pre;
    ch = getc(in);
    if (ch == '=') // "/="
        Output(36, -1);
    else if (ch == '*') // "/*"
    {
        ch = getc(in);
        do
        {
            //如果文件结束，则返回非0值，否则返回0
            if (feof(in))
            {
                num++; //记录错误数量
                printf("\n 第(%d) 行 : 注释错误 : /*后面的字符全部当做注释，缺少结束注释*/\n", n);
                break;
            }
            pre = ch;
            ch = getc(in);
        } while (pre != '*' || ch != '/');
    }
    else if (ch == '/') // "//"
    {
        ch = getc(in);
        while (ch != '\n')
        {
            ch = getc(in);
        }
        n++;
    }
    else // "/"
    {
        remove1(); //多读入一个字符，指针回退一个字符
        Output(21, -1);
    }
}

// Isother 函数识别其他特殊字符
void Isother()
{
    int i = 1;
    int j = 0;
    char ch;
    //()，[]
    //, ;
    //< > ! %     <= >= != %=
    //+ - * =
    if (token[0] == '<' || token[0] == '>' || token[0] == '!' || token[0] == '%' ||
        token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '=')
    {
        ch = getc(in);
        if (ch == '=')
            token[i++] = ch;
        else
            remove1();
    }
    //|  ||
    else if (token[0] == '|')
    {
        ch = getc(in);
        if (ch == '|')
            token[i++] = ch;
        else
            remove1();
    }
    //&  &&
    else if (token[0] == '&')
    {
        ch = getc(in);
        if (ch == '&')
            token[i++] = ch;
        else
            remove1();
    }
    //字符串数组结尾就是'\0',表示这串字符到结尾了
    token[i] = '\0';
    //匹配字符表是否存在该字符

    for (i = 18; i < 45; i++)
    {
        if (strcmp(token, Key[i].keyWord) == 0)
        {
            Output(i, -1); //-1表示特殊字符
            break;
        }
    }
    //如果不存在，输出非法字符
    if (i == 45)
    {
        num++;
        printf("\n 第(%d)行 : 字符错误 : %s 字符非法 \n", n, token);
    }
}
// Scanner函数：若文件未结束，反复调用lexscan函数识别单词。
void Scanner()
{
    char ch;
    int i;
    ch = getc(in);
    while (!feof(in))
    {
        while (ch == ' ' || ch == '\t' || ch == '\n') // 先统计行数
        {
            if (ch == '\n')
                ++n;
            ch = getc(in);
        }
        // 标识符 isalpha(ch)单词
        if (((ch >= 'a') && (ch <= 'z')) || (ch == '_'))
        {
            i = 1;
            token[0] = ch;
            ch = getc(in);
            //当ch为非字母或数字时，终止循环
            while (((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) // word number
            {
                token[i++] = ch;
                ch = getc(in);
            }
            token[i] = '\0';
            //识别保留字和标识符
            Isalpha();
        }
        // 整数
        else if ((ch >= '0') && (ch <= '9'))
        {
            i = 1;
            token[0] = ch;
            ch = getc(in);
            //当ch不是数字时终止循环
            while (isdigit(ch))
            {
                token[i++] = ch;
                ch = getc(in);
            }
            //如果下一位为.
            if (ch == '.')
            {
                token[i++] = ch;
                ch = getc(in);
                while (isdigit(ch))
                {
                    token[i++] = ch;
                    ch = getc(in);
                }
            }
            token[i] = '\0';
            Isnumber();
        }
        else if (ch == '/') // 区分 "/" , "/=" , "/*"
        {
            token[0] = ch;
            Isanotation();
            ch = getc(in);
        }
        else //特殊字符
        {
            token[0] = ch;
            Isother();
            ch = getc(in);
        }
    }
}

void show()
{
    int i;
    char ch;
    printf("分析程序：\n");
    rewind(in);
    char c;
    while (!feof(in))
    {
        c = getc(in);
        if (c != EOF)
            printf("%c", c);
    }
    printf("\n 二元式 :\n");
    rewind(out);
    while (!feof(out))
    {
        c = getc(out);
        if (c != EOF)
            printf("%c", c);
    }
    printf("\n 标志符表 \n");
    for (i = 0; i < NUM; i++)
    {
        if (Word[i][0] != NULL)
            printf("%d,%s\n", i, Word[i]);
    }
    printf("\n 常数表 \n");
    for (i = 0; i < NUM; i++)
    {
        if (consts[i][0] != NULL)
            printf("%d,%s\n", i, consts[i]);
    }
}

int main()
{
    //初始化
    Initscanner();
    Scanner();
    printf("\n 共有 %d 个错误 \n", num);
    show();
    fclose(in);
    fclose(out);
    return 0;
}
