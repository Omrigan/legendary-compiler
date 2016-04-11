//
// Created by oleg on 4/4/16.
//

#ifndef COMPILER_SYNTAX_ANALYSIS_H
#define COMPILER_SYNTAX_ANALYSIS_H

#include "lexem_types.cpp"
using namespace std;
enum class Var_types {
    INT, DOUBLE, BOOL, STRING
};

enum class Error_codes {
    ALREADY_DECLARED,
    NOT_DECLARED,
    MISC,
};

void atom();
void expression();
void operators();
void specatom();
void read();
void var();
void program(vector<lexem> lexems);
void var();
void error(Error_codes error);

#endif //COMPILER_SYNTAX_ANALYSIS_H
