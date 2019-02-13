#ifndef changes_hpp
#define changes_hpp
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>


const int MAX_STR_SIZE = 4;

const char deuce_str[] = "2";
const char neg_unit_str[] = "-1";
const char unit_str[] = "1";
const char var_str[] = "x";
const char null_str[] = "0";
const char sin_str[] = "sin";
const char cos_str[] = "cos";
const char ln_str[] = "ln";
const char log_str[] = "log";
const char tg_str[] = "tg";
const char ctg_str[] = "ctg";
const char sh_str[] = "sh";
const char ch_str[] = "ch";
const char th_str[] = "th";
const char cth_str[] = "cth";
const char mul_str[] = "*";
const char add_str[] = "+";
const char sub_str[] = "-";
const char dev_str[] = "/";
const char deg_str[] = "^";
const char exp_str[] = "e";

enum data_type
{
    VARIABLE,
    OPERAND,
    VALUE
}typedef Data_type;

class Node
{
public:

    void ChangesForDeg();//^
    void ChangesForMul();//*
    void ChangesForDev();// :
    void ChangesForOther();// sin cos
    void ProcDerivative();
    void Add(char *tmp_value_q, const char *tmp_value_arc);
    void ReadingFromFile(FILE *fp);
    void PrintTree();
    void PrintToFile(FILE *fp);
    void ProcessingTree();
    void GoDownForRefuseDer();
    void BuildGraphWithGraphViz(FILE* fp);
    void BuildTexWithLaTex(FILE* TexFile);
    void Conventer();
    void BackConventer();

    bool typeDerivative;
    Data_type type;
    char *value;
    Node *left;
    Node *right;

    Node();
    ~Node();

};

void AssignmentFunc(Node* node, int type, bool typeDiv); // присваивание (куда значение арг и тип дифф)
void CopyNode(Node* b, Node* a);
void CopyNodeSpecial(Node* b, Node* a);
void CopyNodeTotal(Node* b, Node* a);
void openGraph(Node* tree);
void openTex(Node* tree);
void ConnectionNode(Node* left, Node* head, Node* right);
bool ComparisonNode(Node* a, Node* b);
bool isdigitmore(char* str);
int GetN1(char* string);
void SingnedValue(char* str);

#endif /* changes_hpp */
