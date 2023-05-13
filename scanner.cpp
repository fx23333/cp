#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cstring>

using namespace std;

const int N = 100, M = 1e5 + 10;

int e[M], ne[M], h[N], classOfWord[N], idx;
char w[M];
bool terminal[N], hasValue[N];

vector<string> token;
vector<int> UnsignedInt;
vector<string> identifier;

void PrintProgram(string &data);
void Error(int rowNum, int colNum, char *filename);
void Token(string &currentExpression, int &state, int argc, char *argv[]);
// void Dismatch(int lastTerminalState, string currentExpression);
void init(void);
void add(int a, int b, char c);
void addKey1(int source, char another);
void addKey2(int source);
void PrintToken(int argc, char *argv[]);
void PreDeal(string &data);
string IntToString(int temp);
int StringToInt(string &currentExpression);

bool IsDigit(char temp);

bool IsCharacter(char temp);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "No file input" << endl;
        system("PAUSE");
        exit(1);
    }

    // argc = 3, argv[2] = "-g";
    // argv[1] = "C:\\Users\\hp\\Desktop\\cpp\\cp\\test1.cpp";

    memset(h, -1, sizeof h);
    fstream program;

    program.open(argv[1], ios::in);
    if (program.fail())
    {
        cout << "File open failed!" << endl;
        system("PAUSE");
        exit(1);
    }
    init();
    string currentExpression;
    stringstream streambuffer;
    streambuffer << program.rdbuf();
    string data(streambuffer.str());
    PreDeal(data);
    PrintProgram(data);

    int lineNumber = 0;
    int currentLocation = 0;
    int state = 0;
    int lastTerminalState = -1;
    int i = 0;
    int rowNum = 1;
    int colNum = 1;

    while (i < data.size())
    {

        char currentChar = data[i];
        bool currentCharEqualsTargetChar = false;
        for (int t = h[state]; ~t; t = ne[t])
        {
            int j = e[t];
            char targetChar = w[t];
            if (targetChar == currentChar)
            {
                currentCharEqualsTargetChar = true;
                if (currentChar != ' ' && currentChar != '\t' && currentChar != '\n' && currentChar != '\r')
                    currentExpression.push_back(currentChar);
                state = j;
                if (terminal[j])
                    lastTerminalState = j;
                i++;
                if (currentChar == '\n')
                {
                    rowNum++;
                    colNum = 1;
                }
                else
                    colNum++;
            }
        }
        if (!currentCharEqualsTargetChar)
        {
            if (lastTerminalState == -1)
                Error(rowNum, colNum, argv[1]);
            else
            {
                if (state == 0)
                    Error(rowNum, colNum, argv[1]);
                else
                {
                    Token(currentExpression, state, argc, argv);
                    currentExpression.clear();
                    lastTerminalState = -1;
                    state = 0;
                }
            }
        }
    }

    PrintToken(argc, argv);
    system("PAUSE");
    return 0;
}

void PrintProgram(string &data)
{
    puts("Program:");
    int rowNum = 1;
    printf("%d:   ", rowNum);
    for (int i = 0; i < data.size(); i++)
    {
        printf("%c", data[i]);
        if (data[i] == '\n')
            printf("%d:   ", ++rowNum);
    }
    cout << endl;
}

void Error(int rowNum, int ColNum, char *filename)
{
    printf("\n\n%s:\nLexical error: line %d, coloum %d\n", filename, rowNum, ColNum);
    system("PAUSE");
    exit(1);
}

void Token(string &currentExpression, int &state, int argc, char *argv[])
{
    int index = -1;
    int num;
    if (state == 13)
    {
        num = StringToInt(currentExpression);
        auto tempiter = find(UnsignedInt.begin(), UnsignedInt.end(), num);
        if (tempiter == UnsignedInt.end()) // 符号表中没有这个数字
        {
            UnsignedInt.push_back(num);
            index = UnsignedInt.size() - 1;
        }
        else // 符号表中有这个数
            index = tempiter - UnsignedInt.begin();
    }
    if (state == 12 || state == 1 || state == 2 || state == 16 || state == 17 || state == 3 || state == 19 || state == 20 || state == 21) // 说明是标志符
    {
        auto tempiter = find(identifier.begin(), identifier.end(), currentExpression);
        if (tempiter == identifier.end()) // 符号表中没有这个标志符
        {
            identifier.push_back(currentExpression);
            index = identifier.size() - 1;
        }
        else
            index = tempiter - identifier.begin();
    }
    string temp;
    switch (state)
    {
    case 1:
    case 2:
    case 16:
    case 17:
    case 3:
    case 19:
    case 20:
    case 21:
    case 12:
    case 32:
    case 33:
        temp = "<0," + IntToString(index) + ">";
        break;
    case 15:
        temp = "<10>";
        break;
    case 18:
        temp = "<11>";
        break;
    case 22:
        temp = "<12>";
        break;
    case 13:
        temp = "<1," + IntToString(index) + ">";
        break;
    case 4:
        temp = "<27>";
        break;
    case 23:
        temp = "<29>";
        break;
    case 5:
        temp = "<28>";
        break;
    case 24:
        temp = "<30>";
        break;
    case 6:
        temp = "<21>";
        break;
    case 7:
        temp = "<22>";
        break;
    case 8:
        temp = "<23>";
        break;
    case 9:
        temp = "<2>";
        break;
    case 10:
        temp = "<3>";
        break;
    case 25:
        temp = "<31>";
        break;
    case 14:
        temp = "<25>";
        break;
    case 26:
        temp = "<26>";
        break;
    case 27:
        temp = "<4>";
        break;
    case 28:
        temp = "<5>";
        break;
    case 29:
        temp = "<6>";
        break;
    case 30:
        temp = "<7>";
        break;
    case 31:
        temp = "<24>";
        break;
    case 34:
        temp = "<13>";
        break;
    default:
        temp = "<wrong>";
        break;
    }
    if (argc == 3 && strcmp(argv[2], "-g") == 0)
        temp = currentExpression + ": " + temp;
    token.push_back(temp);
}

string IntToString(int num)
{
    string tempstr;
    if (num == 0)
        return "0";
    while (num)
    {
        tempstr.push_back(num % 10 + '0');
        num /= 10;
    }
    reverse(tempstr.begin(), tempstr.end());
    return tempstr;
}

int StringToInt(string &currentExpression)
{
    int num = 0;
    for (int i = 0; i < currentExpression.size(); i++)
        num = num * 10 + (currentExpression[i] - '0');
    return num;
}

void init(void)
{
    add(0, 1, 'i');
    add(0, 2, 'e');
    add(0, 3, 'w');
    add(0, 4, '<');
    add(0, 5, '>');
    add(0, 6, '+');
    add(0, 7, '-');
    add(0, 8, '*');
    add(0, 9, '{');
    add(0, 10, '}');
    add(0, 11, '!');
    add(0, 14, '=');
    add(1, 15, 'f');
    add(2, 16, 'l');
    add(16, 17, 's');
    add(17, 18, 'e');
    add(3, 19, 'h');
    add(19, 20, 'i');
    add(20, 21, 'l');
    add(21, 22, 'e');
    add(4, 23, '=');
    add(5, 24, '=');
    add(11, 25, '=');
    add(14, 26, '=');
    add(0, 27, '(');
    add(0, 28, ')');
    add(0, 0, ' ');
    add(0, 0, '\t');
    add(0, 0, '\n');
    add(0, 0, '\r');
    add(0, 29, '"');
    add(0, 30, ';');
    add(0, 31, '/');  // 新增
    add(0, 32, 'f');  // for
    add(32, 33, 'o'); // for
    add(33, 34, 'r'); // for
    for (int i = '0'; i <= '9'; i++)
    {
        add(0, 13, i);
        add(13, 13, i);
    }

    for (int i = 'a'; i <= 'z'; i++)
        if (i != 'i' && i != 'e' && i != 'w' && i != 'f')
            add(0, 12, i);
    for (int i = 'A'; i <= 'Z'; i++)
        add(0, 12, i);

    addKey1(1, 'f');
    addKey1(2, 'l');
    addKey1(16, 's');
    addKey1(17, 'e');
    addKey1(3, 'h');
    addKey1(19, 'i');
    addKey1(20, 'l');
    addKey1(21, 'e');
    addKey1(32, 'o');
    addKey1(33, 'r');

    addKey2(34);
    addKey2(22);
    addKey2(12);
    addKey2(15);
    addKey2(18);
    for (int i = 1; i <= 34; i++)
        terminal[i] = true;
    terminal[11] = false;
}

void add(int a, int b, char c)
{
    e[idx] = b;
    ne[idx] = h[a];
    w[idx] = c;
    h[a] = idx++;
}

void addKey1(int source, char another)
{
    for (int i = 'a'; i <= 'z'; i++)
        if (i != another)
            add(source, 12, i);
    for (int i = 'A'; i <= 'Z'; i++)
        add(source, 12, i);
    for (int i = '0'; i <= '9'; i++)
        add(source, 12, i);
}

void addKey2(int source)
{
    for (int i = 'a'; i <= 'z'; i++)
        add(source, 12, i);
    for (int i = 'A'; i <= 'Z'; i++)
        add(source, 12, i);
    for (int i = '0'; i <= '9'; i++)
        add(source, 12, i);
}

void PrintToken(int argc, char *argv[])
{
    puts("");
    puts("");
    fstream tokenList;
    fstream UnsignedIntList;
    fstream identifierList;
    tokenList.open("Token.txt", ios::out | ios::trunc);
    UnsignedIntList.open("Constant.txt", ios::out | ios::trunc);
    identifierList.open("Identifier.txt", ios::out | ios::trunc);
    if (!tokenList.is_open() || !UnsignedIntList.is_open() || !identifierList.is_open())
    {
        printf("Writing Error!\n");
        system("PAUSE");
        exit(1);
    }
    puts("Token:");
    for (int i = 0; i < token.size(); i++)
    {
        cout << token[i] << endl;
        tokenList << token[i] << endl;
    }
    tokenList.close();

    printf("\n\nConstant List:\nIndex    Constant\n");
    for (int i = 0; i < UnsignedInt.size(); i++)
    {
        printf("%-9d %d\n", i, UnsignedInt[i]);
        UnsignedIntList << UnsignedInt[i] << endl;
    }
    UnsignedIntList.close();

    printf("\n\nIdentifier List:\nIndex    Identifier\n");
    for (int i = 0; i < identifier.size(); i++)
    {
        printf("%-9d %s\n", i, identifier[i].c_str());
        identifierList << identifier[i] << endl;
    }
    identifierList.close();
}

void PreDeal(string &data)
{
    for (int i = 0; i + 1 < data.size(); i++)
    {
        if (data[i] == '/' && data[i + 1] == '/')
        {
            while (data[i] != '\n' && i < data.size())
                data[i++] = ' ';
        }
    }
}

bool IsDigit(char temp)
{
    return temp >= '0' && temp <= '9';
}
bool IsCharacter(char temp)
{
    return temp >= 'a' && temp <= 'z' || temp >= 'A' && temp <= 'Z';
}