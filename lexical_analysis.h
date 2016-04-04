//
// Created by oleg on 4/4/16.
//

#ifndef COMPILER_LEXICAL_ANALYSIS_H
#define COMPILER_LEXICAL_ANALYSIS_H

#include "types.cpp"



using namespace std;
void build();
bool possible(vertex cur, char c);
vertex go_ahead(vertex cur, char c);
vector<lexem> lexicalAnalysis(string s);
#endif //COMPILER_LEXICAL_ANALYSIS_H
