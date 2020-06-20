#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

ofstream outfile("out.txt",ios::trunc);

enum {
Num = 128, Fun, Sys, Glo, Loc, Id, Char, Else, Enum, If, Int, Return, While,For,Begin,End,Then,Do,Not,Lbr,Rbr,
Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak,Error
};

char *names[100]={
"Num", "Fun", "Sys", "Glo", "Loc", "Id", "Char", "Else", "Enum", "If", "Int", "Return", "While","For","Begin","End","Then","Do","Not","Lbr","Rbr",
"Assign", "Cond", "Lor", "Lan", "Or", "Xor", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Shl", "Shr", "Add", "Sub","Mul", "Div", "Mod", "Inc", "Dec", "Brak","Error"
};

int token,token_val;
char *src;
int poolsize;
int line;


string E();
string Ep(string l);
string T();
string Tp(string l);
string F();

void next()
{
    line++;
    while (token = *src)
    {
        src++;
        if (token >= '0' && token <= '9')
        {
            token_val = token - '0';//当前数字的值
            if (token_val > 0)
            {
                while (*src >= '0' && *src <= '9')
                {
                    token_val = token_val*10 + *src++ - '0';
                }
            }
            return;
        }
    }

}

void match(int tk) {
    if (token_val != tk)
    {
        cout<<"line "<<line<<":expected token: "<<tk<<"("<<names[tk-Num]<<")"<<", but got: "<<token_val<<"("<<names[token_val-Num]<<")"<<endl;
        outfile<<"line "<<line<<":expected token: "<<tk<<"("<<names[tk-Num]<<")"<<", but got: "<<token_val<<"("<<names[token_val-Num]<<")"<<endl;
        //exit(-1);
    }
    next();
}

void program()
{
    cout<<"递归下降过程为:"<<endl;
    next();
    cout<<"递归下降法推导出的式子为:"<<E()<<endl;
}



string E()
{   cout<<"E::TE'"<<endl;
    outfile<<"E::TE'"<<endl;
    string l=T();
    return Ep(l);
}

string Ep(string l)
{
    if(token_val==Add)
    {
        cout<<"E'::+TE'"<<endl;
        outfile<<"E'::+TE'"<<endl;
        match(Add);
        string s=l+"+"+T();
        return Ep(s);
    }
    else if(token_val==Sub)
    {
        cout<<"E'::-TE'"<<endl;
        outfile<<"E'::-TE'"<<endl;
        match(Sub);
        string s=l+"-"+T();
        next();
        return Ep(s);
    }
    else
    {
        cout<<"E'::ε"<<endl;
        outfile<<"E'::ε"<<endl;
        return l;
    }
}

string T()
{
    cout<<"T::FT'"<<endl;
    outfile<<"T::FT'"<<endl;
    string l=F();
    return Tp(l);
}

string Tp(string l)
{
    if(token_val==Mul)
    {
        cout<<"T'::*FT'"<<endl;
        outfile<<"T'::*FT'"<<endl;
        match(Mul);
        string s=l+"*"+F();
        return Tp(s);
    }
    else if(token_val==Div)
    {
        cout<<"T'::/FT'"<<endl;
        outfile<<"T'::/FT'"<<endl;
        match(Div);
        string s=l+"/"+F();
        return Tp(s);
    }
    else
    {
        cout<<"T'::ε"<<endl;
        outfile<<"T'::ε"<<endl;
        return l;
    }
}

string F()
{
    string s;
    if(token_val==Lbr)
    {
        cout<<"F::(E)"<<endl;
        outfile<<"F::(E)"<<endl;
        match(Lbr);
        s="("+E()+")";
        match(Rbr);
    }
    else//注意这里是else，也就是只要不是Lbr的就会认为成id
    {
        cout<<"F::id"<<endl;
        outfile<<"F::id"<<endl;
        s="id";
        match(Id);
    }
    return s;
}

int main()
{
    line=0;

    fstream in("test2.txt");
    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    in.close();

    src=const_cast<char*>(content.c_str());

    program();

    return 0;
}
