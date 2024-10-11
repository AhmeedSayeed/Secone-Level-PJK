/*
 File: A1_T1_S21_20230021.cpp
 Purpose: Solving problems {1,4,7,10}
 Author: Ahmed Sayed Ahmed
 Section: S21 , S22
 ID: 20230021
 TA: I dont know
 Date: 12 Oct 2024
 This code has menu make the user choose which problem he wants or to exit the program
 1. Problem 1: take line by line and collect the whole string and push word by word in the output vector when i found a space
 then, print string by string and print a space between them.

 2. Problem 4: Using sieve algorithm to print the prime numbers to number N.

 3. Problem 7: Using DFS algorithm and dp bitmasks to solve this problem recursive
 DFS : start with the first domino and see if there are any dominos can be putted in the chain
 dp : like DFS but it save the stats to reduce the time

 4. Problem 10: Using map to save the word and its alternative
*/


#include <bits\stdc++.h>
#include <io.h>
#include <fcntl.h>
#include <locale>
#include <codecvt>
using namespace std;

void P1()
{
    cout << "Please, Enter the sentence:\n";
    string s = "" , t , word = "";
    while(true)
    {
        getline(cin,t);
        s += t + ' ';
        bool f = 0;
        for (int i = 0; i < size(t); ++i)
            if(t[i] == '.') f = 1;
        if(f || size(s) > 100)
            break;
    }
    vector <string> v;
    while(isspace(s[0]))
        s.erase(s.begin());
    while(size(s) > 100)
        s.pop_back();
    int idx = 0 , words = 0;
    bool dot;
    while(idx < size(s))
    {
        if(isspace(s[idx]))
        {
            if(word != "")
                words++ , v.push_back(word) , word = "";
            idx++;
            continue;
        }
        if(!idx) word += toupper(s[idx]);
        else word += tolower(s[idx]);
        if(s[idx] == '.')
        {
            if(word == ".")
            {
                if(!size(v)) v.push_back(word);
                else v.back() += word , word = "";
            }
            else v.push_back(word) , word = "";
            dot = 1;
            break;
        }
        idx++;
    }
    if(word != "") v.push_back(word);
    if(!dot) v.back() += '.';
    cout << "The sentence after correcting:\n";
    for(int i = 0; i < size(v); ++i)
        cout << v[i] << ' ';
    cout << '\n';
}

void P4()
{
    cout << "Please, Enter a number:\n";
    string s;
    int n;
    while(true)
    {
        getline(cin,s);
        bool isnINT = 0;
        for(auto c : s)
            if(!isdigit(c)) isnINT = 1;
        if(isnINT)
        {
            cout << "Invalid input!\n"
                    "Please, Enter a number:\n";
            continue;
        }
        break;
    }
    n = stoi(s);
    bool is_Prime[n+2];
    fill(is_Prime , is_Prime + n + 2 , 1);
    is_Prime[0] = is_Prime[1] = 0;
    for (int i = 2; i * i <= n; ++i)
    {
        if(is_Prime[i])
        {
            for (int j = i * i; j <= n; j += i)
                is_Prime[j] = 0;
        }
    }
    vector <int> ans;
    for (int i = 2; i <= n; ++i)
        if(is_Prime[i]) ans.push_back(i);
    if(!size(ans))
        cout << "There are no prime numbers until the number you entered.\n";
    else
    {
        cout << "The prime numbers until the number you entered:\n";
        for (int i = 0; i < size(ans); ++i)
            cout << ans[i] << ' ';
        cout << '\n';
    }
}


struct domino{
    int leftDots;
    int rightDots;
};

#define isON(n,k) ((n) >> (k) & 1ll)    // check if the k th bit in n is on
vector <domino> v;
deque <domino> ans;
short int dp[7][7][1<<24];
bool f = 0;
bool vis[28];

bool DFS(int i , int l , int r , bool ri , vector <domino> &d)
{
    vis[i] = 1;
    if(ri) ans.push_back(v[i]);
    else ans.push_front(v[i]);
    for (int j = 0; j < size(d); ++j)
    {
        if(i == j) continue;
        if(l == v[j].rightDots)
        {
            if(!vis[j])
                DFS(j,v[j].leftDots,r,0,d);
        }
        if(r == v[j].leftDots)
        {
            if (!vis[j])
                DFS(j,l,v[j].rightDots,1,d);
        }
    }
    for (int j = 0; j < size(d); ++j)
    {
        if(!vis[j])
        {
            vis[i] = 0;
            if(ri) ans.pop_back();
            else ans.pop_front();
            return 0;
        }
    }
    return 1;
}

bool calc(int l , int r , domino taken , bool ri , int mask)
{
    if(f) return 1;
    if(ri) ans.push_back(taken);
    else ans.push_front(taken);
    if(__popcount(mask) == size(v))
    {
        f = 1;
        return 1;
    }
    short int &ret = dp[l][r][mask];
    if(~ret)
    {
        if(ri) ans.pop_back();
        else ans.pop_front();
        return ret;
    }
    ret = 0;
    for (int i = 0; i < size(v); ++i)
    {
        if(!isON(mask,i))
        {
            if(v[i].leftDots == r) ret |= calc(l,v[i].rightDots,v[i],1,mask | (1<<i));
            if(v[i].rightDots == l) ret |= calc(v[i].leftDots,r,v[i],0,mask | (1<<i));
        }
    }
    if(f) return 1;
    if(!ret)
    {
        if(ri) ans.pop_back();
        else ans.pop_front();
    }
    return ret;
}

bool FormsDominoChain(vector <domino> & d)
{
    ans.clear() , v = d , f = 0;
    memset(vis,0,sizeof vis);
    if(size(d) > 24)
        return DFS(0,v[0].leftDots,v[0].rightDots,1,d);
    for (int j = 0; j < 7; ++j)
        for (int k = 0; k < 7; ++k)
            for (int m = 0; m < 1<<size(d); ++m)
                dp[j][k][m] = -1;
    return calc(v[0].leftDots,v[0].rightDots,v[0],1,1);
}

void P10()
{
    string alt , post , output;
    while(true)
    {
        cout << "Please, Enter name of the file contains table of alternatives (without extension):\n";
        cin >> alt;
        ifstream table {alt+".txt"};
        if(!table.is_open())
        {
            cout << "Error, table file cant open!\n";
            continue;
        }
        break;
    }

    while(true)
    {
        cout << "Please, Enter name of the file contains the post (without extension):\n";
        cin >> post;
        ifstream in {post+".txt"};
        if(!in.is_open())
        {
            cout << "Error, input file cant open!\n";
            continue;
        }
        break;
    }

    cout << "Please, Enter name of the output file (without extension):\n";
    cin >> output;


    _setmode(_fileno(stdin), _O_U8TEXT);
    _setmode(_fileno(stdout), _O_U8TEXT);
    wifstream table , in;
    wofstream out(output+".txt");
    table.open(alt+".txt", ios::binary) , in.open(post+".txt", ios::binary);
    table.imbue(locale(table.getloc(),new codecvt_utf8<wchar_t>));
    in.imbue(locale(in.getloc(),new codecvt_utf8<wchar_t>));
    out.imbue(locale(out.getloc(),new codecvt_utf8<wchar_t>));

    wstring str;

    map <wstring,wstring> alters;
    wstring a , b;
    while(table >> a >> b)
        alters[a] = b;

    wstring w;
    vector <wstring> ans;
    while(in >> w)
    {
        if(alters.count(w))
            w = alters[w];
        ans.push_back(w);
    }

    wcout << L"The post after changing with alternatives:\n";
    for(auto x : ans)
    {
        wcout << x << ' ';
        out << x << ' ';
    }

    _setmode(_fileno(stdin), _O_TEXT);
    _setmode(_fileno(stdout), _O_TEXT);
    cout << endl;
    cin.ignore();
}

signed main()
{
    cout << "---- Welcome to our program ----\n";
    while(true)
    {
        cout << "   --------- Menu ---------\n";
        cout << "Please, Choose an option:\n"
                "1. Problem 1\n"
                "2. Problem 4\n"
                "3. Problem 7\n"
                "4. Problem 10\n"
                "5. Exit program\n";
        int choice;
        string ch;
        while(true)
        {
            getline(cin,ch);
            bool isnINT = 0;
            for(auto c : ch)
                if(!isdigit(c)) isnINT = 1;
            if(isnINT)
            {
                cout << "Invalid input!\n"
                        "Please, Choose an option:\n";
                continue;
            }
            break;
        }
        choice = stoi(ch);
        if(choice == 1)
            P1();
        else if(choice == 2)
            P4();
        else if(choice == 3)
        {
            cout << "Please, Enter the number of dominos:\n";
            int n;
            cin >> n;
            vector <domino> v(n);
            cout << "Please, Enter the dominos in this form -> Left Dots  Right Dots:\n";
            for (int i = 0; i < n; ++i)
                cin >> v[i].leftDots >> v[i].rightDots;
            bool ok = FormsDominoChain(v);
            if(ok)
            {
                cout << "A possible chain is:\n";
                for (int i = 0; i < n; ++i)
                {
                    cout << ans[i].leftDots << '|' << ans[i].rightDots;
                    if(i < n-1)
                        cout << " - ";
                }
                cout << '\n';
            }
            else
                cout << "There is no possible chain.\n";
            cin.ignore();
        }
        else if(choice == 4)
            P10();
        else
        {
            cout << "---- Thanks to use our program ----\n";
            break;
        }
    }
}
