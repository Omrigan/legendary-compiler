//
// Created by oleg on 4/4/16.
//

#ifndef COMPILER_SYNTAX_ANALYSIS_H
#define COMPILER_SYNTAX_ANALYSIS_H

#include "types.cpp"
using namespace std;
void atom();
void expression();
void operators();
void specatom();
void read();
void var();
void program(vector<lexem> lexems);
void var();
void error();

#endif //COMPILER_SYNTAX_ANALYSIS_H
