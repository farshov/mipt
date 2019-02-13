#ifndef descentparser_hpp
#define descentparser_hpp

#include "changes.hpp"

class DescentParser
{
public:
    char *string;
    int cursor;
    
    Node* RecursiveDescentParser();//главная
    Node* GetN();// число
    Node* GetE();// ±
    Node* GetT();//
    Node* GetP();//
    Node* GetD();// для степени
    Node* GetX();
    Node* GetSin();
    Node* GetCos();
    Node* GetLn();
    Node* GetTg();
    Node* GetCtg();
    Node* GetSh();
    Node* GetCh();
    
    
    DescentParser(char *tmp_string);
    ~DescentParser();
}typedef DescentParser;


Node* ConnectionT(Node* left, Node* head, Node* right, int count, bool plus);
Node* ConnectionE(Node* left, Node* head, Node* right, int count, bool plus);

#endif
