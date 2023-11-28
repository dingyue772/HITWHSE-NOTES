# include <cstdio> 
# include <cstdlib>
# include <cstring>
#define MAXLENGTH 1000

char *rwtab[27] = {"main", "if", "then", "while", "do", "static", "int", "double", "struct", "break", "else", "long", "switch", "case", "typedef", "char", "return", "const", "float", "short", "continue", "for", "void", "sizeof", "default"};

//判断是否为字母
int isLetter(char c);

//判断是否为数字
int isDigital(char c);

//判断字符串是否为关键字
//1~24分别对应24个关键字，25为ID
//关键字default不在种别码表中，这里直接添加到原表末尾了，编号为41
int isInRwtab(char *in);

//判断是否为单运算符
//返回值为种别码值
int isSingleChar(char in);

//字符串预处理
//去除文本中的空格、回车、换行、注释、tab符
//in为输入字符串，n为字符串长度
void preProcedure(char *in, int n);

//词法分析
//将字符串中具有独立意义的单词以二元组的形式输出
void lexing();

int main()
{
    FILE *fp;
    if ((fp = fopen("in.txt", "r")) == NULL)
    {
        printf("File Open Error!\n");
        exit(0);
    }
    else
    {
        printf("File in.txt Open\n");
    }
    char in[MAXLENGTH], ch;
    int n = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        in[n] = ch;
        n++;
    }
    fclose(fp);
    preProcedure(in, n);
    lexing();
    return 0;
}
//字符串预处理
//去除文本中的空格、回车、换行、注释、tab符
//in为输入字符串，n为字符串长度

void preProcedure(char *in, int n)
{
    FILE *f1;
    f1 = fopen("mid.txt", "w+");
    if (f1 == NULL)
    {
        printf("File Open Error!\n");
        exit(0);
    }
    else
    {
        printf("File 1.txt Open\n");
    }
    for (int i = 0; i < n; i++)
    {
        //将连续的空格符缩减为一个
        if (in[i] == ' ')
        {
            fputc(in[i], f1);
            i++;
            while (in[i] == ' ')
            {
                i++;
            }
        }
        //去掉回车、换行、tab符
        if (in[i] == '\n' || in[i] == '\r' || in[i] == 9)
        {
            //去掉换行后冗余的空格
            while (in[i + 1] == ' ')
            {
                i++;
            }
            fputc(' ', f1);
            continue;
        }
        //去掉注释
        if (in[i] == '/')
        {
            i++;
            //单行注释的处理
            if (in[i] == '/')
            {
                while (in[i] != '\n')
                {
                    i++;
                }
                i++;
            }
            //多行注释的处理
            if (in[i] == '*')
            {
                i++;
                while (!(in[i] == '*' && in[i + 1] == '/'))
                {
                    i++;
                }
                i++;
                continue;
            }
        }
        fputc(in[i], f1);
    }
    printf("Preprocedure Done!\n");
    fclose(f1);
}

//判断是否为字母
int isLetter(char c)
{
    if (c >= 65 && c <= 90)
    {
        return 1;
    }
    else if (c >= 97 && c <= 122)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//判断是否为数字
int isDigital(char c)
{
    if (c >= 48 && c <= 57)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//判断字符串是否为关键字
//1~24分别对应24个关键字，25为ID
//关键字default不在种别码表中，这里直接添加到原表末尾了，编号为41
int isInRwtab(char *in)
{
    for (int i = 0; i < 25; i++)
    {
        if (strcmp(rwtab[i], in) == 0)
        {
            if (i == 24)
            {
                return 41;
            }
            return i + 1;
        }
    }
    return 25;
}


//判断是否为单运算符
//返回值为种别码值
int isSingleChar(char in)
{
    if (in == '+')
        return 27;
    if (in == '-')
        return 28;
    if (in == '/')
        return 30;
    if (in == '[')
        return 39;
    if (in == ']')
        return 40;
    if (in == ';')
        return 41;
    if (in == '(')
        return 42;
    if (in == ')')
        return 43;
    if (in == '#')
        return 0;
    return -1;
}

//词法分析
//将字符串中具有独立意义的单词以二元组的形式输出
void lexing()
{
    FILE *fin, *fout;
    fin = fopen("mid.txt", "r");
    fout = fopen("out.txt", "w+");
    if (fin == NULL || fout == NULL)
    {
        printf("File Open Error!\n");
        exit(0);
    }
    else
    {
        printf("File out.txt Open\n");
    }
    char in[MAXLENGTH], ch, token[27], num[27];
    int n = 0, syn;
    while ((ch = fgetc(fin)) != EOF)
    {
        in[n] = ch;
        n++;
    }
    for (int i = 0; i < n; i++)
    {
        //除去冗余的空格
        while (in[i] == ' ')
        {
            i++;
        }
        //初始化token数组下标
        int index = 0;
        //将token数组初始化
        for (int j = 0; j < 27; j++)
        {
            token[j] = 0;
        }
        //若以字母开头
        if (isLetter(in[i]))
        {
            token[index] = in[i];
            index++;
            i++;
            while (isLetter(in[i]) || isDigital(in[i]))
            {
                token[index] = in[i];
                index++;
                i++;
            }
            token[index] = 0;
            //?
            i--;
            syn = isInRwtab(token);
            //向out.txt文件中输出
            if (syn == 25)
            {
                fprintf(fout, "(ID, %s)\n", token);
            }
            else
            {
                fprintf(fout, "(Keyword, %s)\n", token);
            }
        }
        //若以数字开头
        else if (isDigital(in[i]))
        {
            token[index] = in[i];
            index++;
            i++;
            while (isLetter(in[i]) || isDigital(in[i]))
            {
                token[index] = in[i];
                index++;
                i++;
            }
            token[index] = 0;
            i--;
            //纯数字标识
            int isNum = 1;
            //判断是否为纯数字
            for (int j = 1; token[j] != 0; j++)
            {
                if (isLetter(token[j]))
                {
                    fprintf(fout, "(ERROR!, %s)\n", token);
                    isNum = 0;
                    break;
                }
            }
            if (isNum)
            {
                fprintf(fout, "(NUM, %s)\n", token);
            }
        }
        //单运算符或界符
        else if ((syn = isSingleChar(in[i])) != -1)
        {
            fprintf(fout, "(%d, %c)\n", syn, in[i]);
        }
        //判断是否为**
        else if (in[i] == '*')
        {
            if (in[i + 1] == '*')
            {
                fprintf(fout, "(31, **)\n");
                i++;
            }
            else{
                fprintf(fout, "(29, *)\n");
            }
        }
        //判断是否为==
        else if (in[i] == '=')
        {
            if (in[i + 1] == '=')
            {
                fprintf(fout, "(32, ==)\n");
                i++;
            }
            else{
                fprintf(fout, "(38, =)\n");
            }
        }
        //判断是否为<>或<=
        else if (in[i] == '<')
        {
            if (in[i + 1] == '>')
            {
                fprintf(fout, "(34, <>)\n");
                i++;
            }else if(in[i + 1] == '='){
                fprintf(fout, "(35, <=)\n");
            }
            else{
                fprintf(fout, "(33, <)\n");
            }
        }
        //判断是否为>=
        else if (in[i] == '>')
        {
            if (in[i + 1] == '=')
            {
                fprintf(fout, "(37, >=)\n");
                i++;
            }
            else{
                fprintf(fout, "(36, >)\n");
            }
        }
    }
    fclose(fin);
    fclose(fout);
    printf("Lexing Done!\n");
}



