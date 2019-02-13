#ifndef runprog_hpp
#define runprog_hpp

#include "Calculation.hpp"
#include "DescentParser.hpp"


void run(void);
FILE* OpenTex(Node* tree);
void CloseTex(FILE* TexFile);

#endif
