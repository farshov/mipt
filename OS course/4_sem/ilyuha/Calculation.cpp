#include "Calculation.hpp"


DescentParser1::DescentParser1(char *tmp_string):
string(new char[50]),
cursor(0)
{
    strcpy(string, tmp_string);
}

DescentParser1::~DescentParser1()
{
    delete[] string;
}

double DescentParser1::RecursiveDescentParser1()
{
    double head = GetE1();
    if (string[cursor] != '\0')
    {
        fputs("1Ошибка чтения\n", stderr);
        exit(100);
    }
    return head;
}

double DescentParser1::GetN1()
{
    double value1 = 0;
    double value2 = 0;
    double k = 1;
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
    return value1 + value2;
}

double DescentParser1::GetE1()
{
    bool plus = true;
    double value1 = GetT1();
    while (string[cursor] == '+' || string[cursor] == '-')
    {
        plus = (string[cursor] == '+');
        cursor ++ ;

        double value2 = GetT1();
        if (plus)
            value1 += value2;
        else value1 -= value2;
    }
    return value1;
}
double DescentParser1::GetT1()
{
    double value1 = GetD1();
    bool mul = true;
    while (string[cursor] == '*' || string[cursor] == '/')
    {
        mul = (string[cursor] == '*');
        cursor ++;

        double value2 = GetD1();
        if (mul)
            value1 *= value2;
        else value1 /= value2;
    }
    return value1;
}
double DescentParser1::GetD1()
{
    double value1 = GetP1();

    if  (string[cursor] != '^')
        return value1;
    cursor++;
    double value2 = GetP1();

    return pow(value1, value2);
}

double DescentParser1::GetP1()
{
    if (string[cursor] != '(')
        return GetN1();
    cursor ++ ;
    double value = GetE1();
    if (string[cursor] != ')')
    {
        fputs("Ошибка чтения\n", stderr);
        exit(100);
    }
    cursor ++ ;
    return value;
}

