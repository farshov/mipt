#include "DescentParser.hpp"
#include "changes.hpp"
#include <math.h>
#include <string.h>


DescentParser::DescentParser(char *tmp_string):
string(new char[50]),
cursor(0)
{
    strcpy(string, tmp_string);
}

DescentParser::~DescentParser()
{
    delete[] string;
}

Node* DescentParser::RecursiveDescentParser()
{
    Node* head = GetE();
    if (string[cursor] != '\0')
    {
        fputs("1Ошибка чтения\n", stderr);
        exit(100);
    }
    return head;
}

Node* DescentParser::GetN()
{
    Node* head = new Node();
    head -> left = NULL;
    head -> right = NULL;
    char str_tmp[10] = "";
    double value1 = 0;
    double value2 = 0;
    double k = 1;
    if ((string[cursor] == 0) || (string[cursor] == ')') || (string[cursor] == '('))
    {
        fputs("1Ошибка чтения\n", stderr);
        exit(100);
    }
    if (string[cursor] == 'e')
    {
        strcpy(head -> value, "e");
        cursor ++;
        head -> type = VALUE;
        return head;
    }
    while (isdigit(string[cursor]))
    {
        value1 = value1 * 10;
        value1 = value1 + string[cursor] - '0';
        cursor ++;
    }
    if (string[cursor] == '.')
    {
        cursor++;
        while (isdigit(string[cursor]))
        {
            k = k / 10;
            value2 = value2 * 10;
            value2 = value2 + (string[cursor] - '0');
            cursor ++;
        }
        value2 = value2 * k;
    }
    sprintf(str_tmp, "%g", value1 + value2);
    strcpy(head -> value, str_tmp);
    head -> type = VALUE;
    return head;
}

Node* DescentParser::GetE()
{
    bool plus = true;
    Node* sign1 = new Node();
    Node* value1 = GetT();
    int count = 0;
    while (string[cursor] == '+' || string[cursor] == '-')
    {
        ++count;
        plus = (string[cursor] == '+');
        cursor++;
        if (plus)
            strcpy(sign1 -> value, "+");
        else
            strcpy(sign1 -> value, "-");
        Node* value2 = GetT();
        sign1 -> type = OPERAND;
        sign1 = ConnectionE(value1, sign1, value2, count, plus);
    }
    if (!count)
        return value1;
    else
        return sign1;
}
Node* DescentParser::GetT()
{
    Node* value1 = GetD();
    Node* sign1 = new Node();
    bool mul = true;
    int count = 0;
    while (string[cursor] == '*' || string[cursor] == '/')
    {
        ++count;
        mul = (string[cursor] == '*');
        cursor ++;
        if (mul)
            strcpy(sign1 -> value, "*");
        else
            strcpy(sign1 -> value, "/");
        Node* value2 = GetD();
        sign1 -> type = OPERAND;
        sign1 = ConnectionT(value1, sign1, value2, count, mul);
    }
    if (!count)
        return value1;
    else
        return sign1;
}

Node* DescentParser::GetD()
{
    Node* value1 = GetLn();
    if  (string[cursor] != '^')
        return value1;
    cursor++;
    Node* deg = new Node();
    deg -> type = OPERAND;
    strcpy(deg -> value, "^");
    Node* value2 = GetLn();
    deg -> left = value1;
    deg -> right = value2;
    return deg;
}

Node* DescentParser::GetLn()
{
    if (string[cursor] == 'l' && string[cursor + 1] == 'n')
    {
        Node* NewNodeLn = new Node();
        strcpy(NewNodeLn -> value, "ln");
        cursor += 2;
        Node* LeftLn = GetSin();
        NewNodeLn -> left = LeftLn;
        NewNodeLn -> type = VARIABLE;
        return NewNodeLn;
    }
    else
        return GetSin();
}

Node* DescentParser::GetSin()
{
    if (string[cursor] == 's' && string[cursor + 1] == 'i' && string[cursor + 2] == 'n')
    {
        cursor += 3;
        Node* NewNodeSin = new Node();
        Node* LeftSin = GetTg();
        NewNodeSin -> left = LeftSin;
        NewNodeSin -> type = VARIABLE;
        strcpy(NewNodeSin -> value, "sin");
        return NewNodeSin;
    }
    else
        return GetTg();
}
Node*DescentParser::GetTg()
{
    if (string[cursor] == 't' && string[cursor + 1] == 'g')
    {
        Node* NewNodeTg = new Node();
        cursor += 2;
        Node* LeftTg = GetCtg();
        NewNodeTg -> left = LeftTg;
        strcpy(NewNodeTg -> value, "tg");
        NewNodeTg-> type = VARIABLE;
        return NewNodeTg;
    }
    else
        return GetCtg();
}

Node*DescentParser::GetCtg()
{
    if (string[cursor] == 'c' && string[cursor + 1] == 't' && string[cursor + 2] == 'g')
    {
        Node* NewNodeCtg = new Node();
        cursor += 3;
        Node* LeftCtg = GetCh();
        NewNodeCtg -> left = LeftCtg;
        strcpy(NewNodeCtg -> value, "ctg");
        NewNodeCtg-> type = VARIABLE;
        return NewNodeCtg;
    }
    else
        return GetCh();
}

Node*DescentParser::GetCh()
{
    if (string[cursor] == 'c' && string[cursor + 1] == 'h')
    {
        Node* NewNodeCh = new Node();
        cursor += 2;
        Node* LeftCh = GetSh();
        NewNodeCh -> left = LeftCh;
        strcpy(NewNodeCh -> value, "ch");
        NewNodeCh -> type = VARIABLE;
        return NewNodeCh;
    }
    else
        return GetSh();
}


Node*DescentParser::GetSh()
{
    if (string[cursor] == 's' && string[cursor + 1] == 'h')
    {
        Node* NewNodeSh = new Node();
        cursor += 2;
        Node* LeftSh = GetCos();
        NewNodeSh -> left = LeftSh;
        strcpy(NewNodeSh -> value, "sh");
        NewNodeSh -> type = VARIABLE;
        return NewNodeSh;
    }
    else
        return GetCos();
}


Node* DescentParser::GetCos()
{
    if (string[cursor] == 'c' && string[cursor + 1] == 'o' && string[cursor + 2] == 's')
    {
        Node* NewNodeCos = new Node();
        cursor += 3;
        Node* LeftCos = GetX();
        NewNodeCos -> left = LeftCos;
        strcpy(NewNodeCos -> value, "cos");
        NewNodeCos -> type = VARIABLE;
        return NewNodeCos;
    }
    else
        return GetX();
}

Node* DescentParser::GetX()
{
    if (string[cursor] == 'x')
    {
        Node* NewNodeX = new Node();
        strcpy(NewNodeX -> value, var_str);
        NewNodeX -> type = VARIABLE;
        cursor ++;
        return NewNodeX;
    }
    return GetP();
}

Node* DescentParser::GetP()
{
    if (string[cursor] != '(')
        return GetN();
    cursor ++ ;
    Node* value = GetE();
    if (string[cursor] != ')')
    {
        fputs("2Ошибка чтения\n", stderr);
        exit(100);
    }
    cursor ++;
    return value;
}


Node* ConnectionE(Node* left, Node* head, Node* right, int count, bool plus)
{
    if (count == 1)
    {
        head -> left = left;
        head -> right = right;
        return head;
    }
    else
    {
        Node* NewNodeHead = new Node();
        NewNodeHead -> left = head;
        NewNodeHead -> right = right;
        if (plus)
            strcpy(NewNodeHead -> value, "+");
        else
            strcpy(NewNodeHead -> value, "-");
        NewNodeHead -> type = OPERAND;
        return NewNodeHead;
    }
}

Node* ConnectionT(Node* left, Node* head, Node* right, int count, bool plus)
{
    if (count == 1)
    {
        head -> left = left;
        head -> right = right;
        return head;
    }
    else
    {
        Node* NewNodeHead = new Node();
        NewNodeHead -> left = head;
        NewNodeHead -> right = right;
        if (plus)
            strcpy(NewNodeHead -> value, "*");
        else
            strcpy(NewNodeHead -> value, "/");
        NewNodeHead -> type = OPERAND;
        return NewNodeHead;
    }

}








