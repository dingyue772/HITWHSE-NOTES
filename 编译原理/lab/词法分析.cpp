# include <cstdio> 
# include <cstdlib>
# include <cstring>
#define MAXLENGTH 1000

char *rwtab[27] = {"main", "if", "then", "while", "do", "static", "int", "double", "struct", "break", "else", "long", "switch", "case", "typedef", "char", "return", "const", "float", "short", "continue", "for", "void", "sizeof", "default"};

//�ж��Ƿ�Ϊ��ĸ
int isLetter(char c);

//�ж��Ƿ�Ϊ����
int isDigital(char c);

//�ж��ַ����Ƿ�Ϊ�ؼ���
//1~24�ֱ��Ӧ24���ؼ��֣�25ΪID
//�ؼ���default�����ֱ�����У�����ֱ����ӵ�ԭ��ĩβ�ˣ����Ϊ41
int isInRwtab(char *in);

//�ж��Ƿ�Ϊ�������
//����ֵΪ�ֱ���ֵ
int isSingleChar(char in);

//�ַ���Ԥ����
//ȥ���ı��еĿո񡢻س������С�ע�͡�tab��
//inΪ�����ַ�����nΪ�ַ�������
void preProcedure(char *in, int n);

//�ʷ�����
//���ַ����о��ж�������ĵ����Զ�Ԫ�����ʽ���
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
//�ַ���Ԥ����
//ȥ���ı��еĿո񡢻س������С�ע�͡�tab��
//inΪ�����ַ�����nΪ�ַ�������

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
        //�������Ŀո������Ϊһ��
        if (in[i] == ' ')
        {
            fputc(in[i], f1);
            i++;
            while (in[i] == ' ')
            {
                i++;
            }
        }
        //ȥ���س������С�tab��
        if (in[i] == '\n' || in[i] == '\r' || in[i] == 9)
        {
            //ȥ�����к�����Ŀո�
            while (in[i + 1] == ' ')
            {
                i++;
            }
            fputc(' ', f1);
            continue;
        }
        //ȥ��ע��
        if (in[i] == '/')
        {
            i++;
            //����ע�͵Ĵ���
            if (in[i] == '/')
            {
                while (in[i] != '\n')
                {
                    i++;
                }
                i++;
            }
            //����ע�͵Ĵ���
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

//�ж��Ƿ�Ϊ��ĸ
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


//�ж��Ƿ�Ϊ����
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


//�ж��ַ����Ƿ�Ϊ�ؼ���
//1~24�ֱ��Ӧ24���ؼ��֣�25ΪID
//�ؼ���default�����ֱ�����У�����ֱ����ӵ�ԭ��ĩβ�ˣ����Ϊ41
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


//�ж��Ƿ�Ϊ�������
//����ֵΪ�ֱ���ֵ
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

//�ʷ�����
//���ַ����о��ж�������ĵ����Զ�Ԫ�����ʽ���
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
        //��ȥ����Ŀո�
        while (in[i] == ' ')
        {
            i++;
        }
        //��ʼ��token�����±�
        int index = 0;
        //��token�����ʼ��
        for (int j = 0; j < 27; j++)
        {
            token[j] = 0;
        }
        //������ĸ��ͷ
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
            //��out.txt�ļ������
            if (syn == 25)
            {
                fprintf(fout, "(ID, %s)\n", token);
            }
            else
            {
                fprintf(fout, "(Keyword, %s)\n", token);
            }
        }
        //�������ֿ�ͷ
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
            //�����ֱ�ʶ
            int isNum = 1;
            //�ж��Ƿ�Ϊ������
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
        //�����������
        else if ((syn = isSingleChar(in[i])) != -1)
        {
            fprintf(fout, "(%d, %c)\n", syn, in[i]);
        }
        //�ж��Ƿ�Ϊ**
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
        //�ж��Ƿ�Ϊ==
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
        //�ж��Ƿ�Ϊ<>��<=
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
        //�ж��Ƿ�Ϊ>=
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



