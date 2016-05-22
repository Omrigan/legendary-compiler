//
// Created by oleg on 4/4/16.
//

#include <bits/stdc++.h>
#include <iostream>
#ifndef COMPILER_LEXICAL_ANALYSIS_H
#define COMPILER_LEXICAL_ANALYSIS_H

#include "lexem_types.h"



using namespace std;

void build();
bool possible(vertex cur, char c);
vertex go_ahead(vertex cur, char c);
vector<lexem> lexicalAnalysis(string s);
#endif //COMPILER_LEXICAL_ANALYSIS_H
