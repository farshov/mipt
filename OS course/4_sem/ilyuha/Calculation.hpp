#ifndef Calculation_hpp
#define Calculation_hpp

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

class DescentParser1
{
public:
    char *string;
    int cursor;

    double RecursiveDescentParser1();//главная
    double GetN1();// число
    double GetE1();// ±
    double GetT1();//
    double GetP1();//
    double GetD1();


    DescentParser1(char *tmp_string);
    ~DescentParser1();
}typedef DescentParser1;


#endif /* Calculation_hpp */
