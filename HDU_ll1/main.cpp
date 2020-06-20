#include<bits/stdc++.h>

using namespace std;

vector<char> N;
vector<char> Z;
vector<string> splitstr(const string& str, const string& pattern);
//使用的结构
struct item{
    char left;
    vector<char> right;
    item(char left,vector<char>right):left(left),right(right){}
};
vector <item> gram;
set<char> mfirst(char c);
int invec(vector<char> v,char c);
set<char> mfollow(char C);
set<char> getinfirst(vector<char> V);
set<char> mselect(item I);
string a, b;
int n = 0;
bool iszhongjie(char ch);
void queue_out();
void LL1();
map <int, string> LL1_map[200];
string zhongjie="+-*/()i$";
string fuhao="EXTYFAM";
bool isVaild = true;
stack <char> s;
queue <char> q, matches;
char str[256];
char ch1, ch2;



int main(){
    string setN="EXTYF";
    string setZ="ei+-*/()";
    cout<<"非终结符: ";
    for(int i=0;i<setN.size();i++)
    {
        cout<<setN[i]<<" ";
        N.push_back(setN[i]);
    }
    cout<<endl;
    cout<<"终结符: ";
    for(int i=0;i<setZ.size();i++)
    {
        cout<<setZ[i]<<" ";
        Z.push_back(setZ[i]);
    }
    cout<<endl;
    ifstream input;
    input.open("wenfa.txt");
    string line;
    string in;
    while(getline(input,line))
    {
        vector<string> strlist=splitstr(line,":=");
        vector<char> temp_right;
        for(auto i:strlist[1]) temp_right.push_back(i);
        gram.push_back(item(strlist[0][0],temp_right));
    }
    cout<<endl;
    cout<<"First集为:"<<endl;
    for(auto i:N){
        set<char> res1=mfirst(i);
        cout<<"First("<<i<<"): ";
        for(auto j:res1)cout<<setw(2)<<j<<" ";
        cout<<endl;
    }
    cout<<endl;
    cout<<"Follow集为:"<<endl;
    for(auto i:N)
    {
        set<char> res2=mfollow(i);
        cout<<"Follow("<<i<<"): ";
        for(auto j:res2)cout<<setw(2)<<j<<" ";
        cout<<endl;
    }
    cout<<endl;
    cout<<"Select集为:"<<endl;
    for(auto i:gram)
    {
        cout<<"Select("<<i.left<<"->";
        for(auto j:i.right) cout<<j;
        cout<<"): ";
        for(auto j:mselect(i))cout<<setw(2)<<j<<" ";
        cout<<endl;
    }
    cout<<endl;
    cout<<"是否是LL(1)文法:"<<endl;
    //判断条件：上下文无关
    for(int i=0;i<gram.size();i++)
    {
        for(int j=0;j<gram.size();j++)
        {
            if(gram[i].left==gram[j].left&&(i!=j))
            {
                set<char> jiao;
                set <char> a=mselect(gram[i]);
                set <char> b=mselect(gram[j]);
                set_intersection(a.begin(),a.end(),b.begin(),b.end(),inserter(jiao,jiao.begin()));
                if(jiao.size()>=1)
                {
                    cout<<"No!"<<endl;
                    return -1;
                }
            }
        }
    }
    cout<<"Yes!"<<endl;
    cout<<endl<<endl<<"LL1预测分析过程为:"<<endl;
    LL1();
    return 0;

}


//参考实现，作用为拆分pattern左右部string并存入vector中
vector<string> splitstr(const string& str, const string& pattern)
{
    vector<string>  li;
    string subStr;
    string tPattern;
    size_t patternLen =pattern.length();
    size_t strLen= str.length();
    for(size_t i = 0; i < str.length(); i++)
    {
        if(pattern[0] == str[i] && ((strLen - i) >= patternLen))
        {
            if(memcmp(&pattern[0],&str[i], patternLen) == 0)
            {
                i += patternLen - 1;
                if(!subStr.empty())
                {
                    li.push_back(subStr);
                    subStr.clear();
                }
            }
            else
            {
                subStr.push_back(str[i]);
            }
        }
        else
        {
            subStr.push_back(str[i]);
        }
    }
    if(!subStr.empty())
    {
        li.push_back(subStr);
    }
    return li;
}

set<char> mfirst(char c){
    set<char> first_res;
    if(invec(Z,c)){
        first_res.insert(c);
        return first_res;
    }else{
        for(int i=0;i<gram.size();i++){
            if(gram[i].left==c){
                for(int j=0;j<gram[i].right.size();j++){
                        set<char> temp=mfirst(gram[i].right[j]);
                        if(temp.find('e')!=temp.end()&&temp.size()>1){
                            temp.erase(temp.find('e'));
                            set_union(first_res.begin(),first_res.end(),temp.begin(),temp.end(),inserter(first_res,first_res.begin()));
                        }else{
                            set_union(first_res.begin(),first_res.end(),temp.begin(),temp.end(),inserter(first_res,first_res.begin()));
                            break;
                        }
                }
            }
        }
        return first_res;
    }
}
int invec(vector<char> v,char c)
{
    int i;
    auto it=find(v.begin(),v.end(),c);
    i=(it==v.end())?0:1;
    return i;
}

set<char> mfollow(char C)
{
    set<char> follow_res;
    if(invec(N,C))
        {
        for(int i=0;i<gram.size();i++)
        {
            if(gram[i].left==C)
            {
                if(i==0){
                    follow_res.insert('$');
                }
            }
            else{
                int j;
                for(j=0;j<gram[i].right.size();j++)
                {
                    if(gram[i].right[j]==C)
                    {
                        break;
                    }
                }
                for(j;j<gram[i].right.size();j++)
                {
                        if(j+1==gram[i].right.size())
                        {
                                set<char> temp=mfollow((gram[i].left));
                                set_union(follow_res.begin(),follow_res.end(),temp.begin(),temp.end(),inserter(follow_res,follow_res.begin()));
                        }
                        else
                        {
                            set<char> temp2=mfirst(gram[i].right[j+1]);
                            if(temp2.find('e')!=temp2.end()){
                                temp2.erase(temp2.find('e'));
                                set_union(follow_res.begin(),follow_res.end(),temp2.begin(),temp2.end(),inserter(follow_res,follow_res.begin()));
                            }
                            else
                            {
                                set_union(follow_res.begin(),follow_res.end(),temp2.begin(),temp2.end(),inserter(follow_res,follow_res.begin()));
                                break;
                            }
                        }
                }
            }

        }
        return follow_res;
    }
    else
    {
        cout<<"Error: Follow集只能输入终结符"<<endl;
    }
}

set<char> getinfirst(vector<char> V)
{
    set<char> res;
    for(auto i:V)
    {
        set<char> tmp=mfirst(i);
        set_union(res.begin(),res.end(),tmp.begin(),tmp.end(),inserter(res,res.begin()));
        if(tmp.find('e')!=tmp.end()) continue;
        else break;
    }
    return res;
}

set<char> mselect(item I)
{
    set<char> res;
    set<char> temp=getinfirst(I.right);
    //求select集的两个分支
    if(temp.find('e')!=temp.end())
    {
        temp.erase('e');
        set<char> tt=mfollow(I.left);
        set_union(temp.begin(),temp.end(),tt.begin(),tt.end(),inserter(res,res.begin()));
    }
    else res=temp;
    return res;
}
bool iszhongjie(char ch) {
    for (int i = 0; i < 8; i++) {
        if (ch == zhongjie[i])
            return true;
    }
    return false;
}
void queue_out()
{
    queue <char> q1 = matches;
    while (!q1.empty())
    {
        cout << q1.front();
        q1.pop();
    }
}
void LL1()
{
    for (auto i:fuhao)
        for (auto j:zhongjie) LL1_map[i][j] = "NULL";
    LL1_map['E']['('] = "TX"; LL1_map['E']['i'] = "TX"; LL1_map['X']['+'] = "ATX"; LL1_map['X']['-'] = "ATX"; LL1_map['X'][')'] = "#";
    LL1_map['X']['$'] = "#";   LL1_map['T']['('] = "FY"; LL1_map['T']['i'] = "FY"; LL1_map['Y']['+'] = "#";   LL1_map['Y']['-'] = "#";
    LL1_map['Y']['*'] = "MFY"; LL1_map['Y']['/'] = "MFY"; LL1_map['Y'][')'] = "#";   LL1_map['Y']['$'] = "#";   LL1_map['F']['('] = "(E)";
    LL1_map['F']['i'] = "i";   LL1_map['A']['+'] = "+";   LL1_map['A']['-'] = "-";   LL1_map['M']['/'] = "/";   LL1_map['M']['*'] = "*";


    string str="i+(i*+i)";
    for (auto i:str) {
        q.push(i);
    }
    q.push('$');
    s.push('$');
    s.push('E');
    int step = 1;
    queue_out();
    while (!s.empty() && s.top()!= '$')
    {
        cout << endl;
        ch1 = s.top();
        ch2 = q.front();
        if (iszhongjie(ch1) || ch1 == '$') {
            if (ch1 == ch2) {
                matches.push(ch1);
                s.pop();
                q.pop();
                queue_out();
                cout << "   match:" << ch1;
            }
            else
            {
                cout << setw(6);
                cout << "   out:";
                isVaild = false;
                s.pop();
            }
        }
        else
        {
            b = LL1_map[ch1][ch2];
            if (b != "NULL")
            {
                s.pop();
                if (b != "#")
                    for (int i = b.length() - 1; i >= 0; i--)
                        s.push(b[i]);
                queue_out();
                if (ch1 == 'X')
                    cout << "   out:" << "E'" << "->";
                else if (ch1 == 'Y')
                    cout << "   out:" << "T'" << "->";
                else
                    cout << "   out:" << ch1 << "->";
                for (int i = 0; i < b.length(); i++)
                {
                    if (b[i] == 'Y')
                        cout << "T'";
                    else if (b[i] == 'X')
                        cout << "E'";
                    else
                        cout << b[i];
                }
            }
            else
            {
                cout << "error";
                isVaild = false;
                q.pop();
            }

        }
    }
    cout<<endl<<"Result is:";
    if (isVaild)cout << "yes" <<endl;
    else cout << "no" <<endl;
}
