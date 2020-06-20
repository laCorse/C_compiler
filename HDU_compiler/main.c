#include <stdio.h>
#include <stdlib.h>


enum {
Num = 128, Fun, Sys, Glo, Loc, Id, Char, Else, Enum, If, Int, Return, While,For,Begin,End,Then,Do,Not,Lbr,Rbr,
Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak,Error
};

char *names[100]={
"Num", "Fun", "Sys", "Glo", "Loc", "Id", "Char", "Else", "Enum", "If", "Int", "Return", "While","For","Begin","End","Then","Do","Not","Lbr","Rbr",
"Assign", "Cond", "Lor", "Lan", "Or", "Xor", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Shl", "Shr", "Add", "Sub","Mul", "Div", "Mod", "Inc", "Dec", "Brak","Error"
};

int token;                    // ��ǰtoken
int token_val;                // ��ǰtoken��ֵ
char *src, *old_src;          // ָ��ԭ�ַ�������ָ��
int poolsize;                 // default size of text/data/stack
int line;                     // ����

int *current_id,*symbols;
char *data;                   // ���

//��ʶ�� Token:��ʶ���ı�� Hash����ϣֵ Name����ű�ʶ�������� Class����ʶ����� Type����ʶ�����ͣ���int����ʲô Value����ʶ����ֵ
enum {Token, Hash, Name, Type, Class, Value, BType, BClass, BValue, IdSize};

void next1()
{
    token=*src++;
    return;
}



void next() {
    //char *last_pos;
    //int hash;

    while (token = *src) {
        ++src;

        while(token==' ' || token == '\t')
        {
            token=*src++;
        }

        if (token == '\n')
        {
            ++line;
        }
        else if(token=='w')
        {
            char *src_tmp=src;
            char flag = *src++ != 'h' ? 0 : *src++ != 'i'?0:*src++ != 'l'?0:*src++!='e'?0:1;
            if(flag)
            {
                token=While;
            }
            else
            {
                src=src_tmp;
                while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                {
                    //������ȡ��ʶ��
                    src++;
                }
                token=Id;
            }
            return;
        }
        else if(token=='i')
        {
            char *src_tmp=src;
            if(*src=='f')
            {
                token=If;
            }
            else
            {
                src=src_tmp;
                while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                {
                    //������ȡ��ʶ��
                    src++;
                }
                token=Id;
            }
            return;
        }
        else if(token=='e')
        {
            char *src_tmp=src;
            char flag1 = *src++ != 'l' ? 0 : *src++ != 's'?0:*src++ != 'e'?0:1;
            src=src_tmp;
            char flag2 = *src++ != 'n' ? 0 : *src++ != 'd'?0:1;
            if(flag1 && !flag2)
            {
                token=Else;
            }
            else if (flag2 && !flag1)
            {
                token=End;
            }
            else
            {
                src=src_tmp;
                while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                {
                    //������ȡ��ʶ��
                    src++;
                }
                token=Id;
            }
            return;
        }
        else if(token=='f')
        {
            char *src_tmp=src;
            char flag = *src++ != 'o' ? 0 : *src++ != 'r'?0:1;
            if(flag)
            {
                token=For;
            }
            else
            {
                src=src_tmp;
                while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                {
                    //������ȡ��ʶ��
                    src++;
                }
                token=Id;
            }
            return;
        }
        else if(token=='b')
        {
            char *src_tmp=src;
            char flag = *src++ != 'e' ? 0 : *src++ != 'g'?0:*src++ != 'i'?0:*src++!='n'?0:1;
            if(flag)
            {
                token=Begin;
            }
            else
            {
                src=src_tmp;
                while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                {
                    //������ȡ��ʶ��
                    src++;
                }
                token=Id;
            }
            return;
        }

        else if(token=='t')
        {
            char *src_tmp=src;
            char flag = *src++ != 'h' ? 0 : *src++ != 'e'?0:*src++ != 'n'?0:1;
            if(flag)
            {
                token=Then;
            }
            else
            {
                src=src_tmp;
                while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                {
                    //������ȡ��ʶ��
                    src++;
                }
                token=Id;
            }
            return;
        }
        else if(token=='d')
        {
            char *src_tmp=src;
            if(*src=='o')
            {
                token=Do;
            }
            else
            {
                src=src_tmp;
                while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                {
                    //������ȡ��ʶ��
                    src++;
                }
                token=Id;
            }
            return;
        }
        else if(token=='a')
        {
            char *src_tmp=src;
            char flag = *src++ != 'n' ? 0 : *src++ != 'd'?0:1;
            if(flag)
            {
                token=And;
            }
            else
            {
                src=src_tmp;
                while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                {
                    //������ȡ��ʶ��
                    src++;
                }
                token=Id;
            }
            return;
        }
        else if(token=='o')
        {
            char *src_tmp=src;
            if(*src=='r')
            {
                token=Or;
            }
            else
            {
                src=src_tmp;
                while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                {
                    //������ȡ��ʶ��
                    src++;
                }
                token=Id;
            }
            return;
        }
        else if(token=='n')
        {
            char *src_tmp=src;
            char flag = *src++ != 'o' ? 0 : *src++ != 't'?0:1;
            if(flag)
            {
                token=Not;
            }
            else
            {
                src=src_tmp;
                while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                {
                    //������ȡ��ʶ��
                    src++;
                }
                token=Id;
            }
            return;
        }
//        //������ʶ��
//        else if((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_'))
//        {
//            last_pos=src-1;//��¼����һ���ַ���λ��
//            hash=token;//��¼�µ�ǰ�ַ���ascii��Ϊhashֵ
//
//            while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
//            {
//                //������ȡ��ʶ��
//                hash=hash*147+*src;
//                src++;
//            }
//            //�����еı�ʶ����Ѱ���Ƿ���ֹ�
//            current_id=symbols;
//            while(current_id[Token])
//            {
//                //���Hash�ֶζԵ��ϣ������ڴ��д�ŵ�������һ���ģ�˵����ͬһ����ʶ��
//                if (current_id[Hash] == hash && !memcmp((char *)current_id[Name], last_pos, src - last_pos))
//                {
//                    token=current_id[Token];
//                    return;//�ҵ���ֱ�ӷ���
//                }
//                current_id=current_id+IdSize;
//            }
//            //û�ҵ��ʹ�ŵ�ǰ��ʶ��
//            current_id[Name]=(int)last_pos;
//            current_id[Hash]=hash;
//            token=current_id[Token]=Id;
//            return;
//        }
        else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z'))
        {
            while((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
            {
                //������ȡ��ʶ��
                src++;
            }
            token=Id;
            return;
        }
        else if (token >= '0' && token <= '9')
        {
            token_val = token - '0';//��ǰ���ֵ�ֵ
            if((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
            {
                printf("Lexer Error: Line %d : ���ֿ�ͷ�ı�ʶ�� \n",line);
            }
            if (token_val > 0)
            {
                while (*src >= '0' && *src <= '9')
                {
                    token_val = token_val*10 + *src++ - '0';
                    if((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                    {
                        printf("Lexer Error: Line %d : ���ֿ�ͷ�ı�ʶ�� \n",line);
                    }
                }
            }
            token=Num;
            return;
        }
        else if (token == '#')
        {
            while (*src != 0 && *src != '\n')
            {
                ++src;
            }
        }
        else if(token==':')
        {
            if(*src == '=')
            {
                src++;
                token = Assign;
            }
            return;
        }
        else if (token == '=')
        {
            token = Eq;
            return;
        }
        else if (token == '+')
        {
            // '+' �� '++'
            if (*src == '+')
            {
                src ++;
                token = Inc;
            }
            else
            {
                token = Add;
            }
            return;
        }
        else if (token == '-')
        {
            // '-' �� '--'
            if (*src == '-')
            {
                src ++;
                token = Dec;
            }
            else
            {
                token = Sub;
            }
            return;
        }
        else if (token == '<')
        {
            // '<=', '<<' , '<' , '<>'
            if (*src == '=') {
                src ++;
                token = Le;
            }
            else if (*src == '<')
            {
                src ++;
                token = Shl;
            }
            else if (*src == '>')
            {
                src++;
                token = Ne;
            }
            else
            {
                token = Lt;
            }
            return;
        }
        else if (token == '/')
        {
            token = Div;
            return;
        }
        else if (token == '>')
        {
            // '>=', '>>' or '>'
            if (*src == '<')
            {
                printf("Lexer Error: Line %d : ����ʹ���� >< ? \n",line);
            }
            if (*src == '=')
            {
                src ++;
                token = Ge;
            }
            else if (*src == '>')
            {
                src ++;
                token = Shr;
            }
            else
            {
                token = Gt;
            }
            return;
        }
        else if (token == '|')
        {
            // '|' or '||'
            if (*src == '|')
            {
                src ++;
                token = Lor;
            }
            else
            {
                token = Or;
            }
            return;
        }
        else if (token == '&')
        {
            // '&'  '&&'
            if (*src == '&') {
                src ++;
                token = Lan;
            }
            else
            {
                token = And;
            }
            return;
        }
        else if (token == '^')
        {
            token = Xor;
            return;
        }
        else if (token == '%')
        {
            token = Mod;
            return;
        }
        else if (token == '*')
        {
            token = Mul;
            return;
        }
        else if (token == '[')
        {
            token = Brak;
            return;
        }
        else if (token == '?')
        {
            token = Cond;
            return;
        }
        else if (token == '(')
        {
            token = Lbr;
            return;
        }
        else if (token == ')')
        {
            token = Rbr;
            return;
        }
        else if (token == ';' || token == '{' || token == '}'|| token == ']' || token == ',' || token == '~')
        {
            return;
        }
        else
        {
            printf("Lexer Error: Line %d: Token: %c �޷�ʶ��Ĵʷ� \n",line,token);
            token=Error;
            return;
        }


    }
    return;
}


void program()
{
    FILE *fp;
    if( (fp=fopen("test4_result.txt","wt+")) == NULL ){
        puts("Fail to open file!");
        exit(0);
    }

    next();
    while(token>0)
    {
        if(token>Num-1) fprintf(fp,"%d,%s\n",token,names[token-Num]);
        next();
    }
    fclose(fp);
}


int main()
{
    int i, fd;

    poolsize = 256 * 1024;
    line = 1;

    if ((fd = open("test4.txt", 0)) < 0)
    {
        return -1;
    }
    if (!(src = old_src = malloc(poolsize)))
    {
        printf("could not malloc(%d) for source area\n", poolsize);
    }
    if ((i = read(fd, src, poolsize-1)) <= 0)
    {
        return -1;
    }
    src[i] = 0;
    close(fd);

    program();
    return 0;
}
