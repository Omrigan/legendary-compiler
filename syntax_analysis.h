//
// Created by oleg on 4/4/16.
//

#ifndef COMPILER_SYNTAX_ANALYSIS_H
#define COMPILER_SYNTAX_ANALYSIS_H

#include "lexem_types.cpp"
#include "IdTable.h"
using namespace std;

enum class Error_codes {
    ALREADY_DECLARED,
    NOT_DECLARED,
    MISC,
};

class SyntaxAnalysis {
private:
    IdTable* id_table;
    set<string> assigned;
    vector<lexem> lexems;

    int lexem_number = 0;
    bool need_generate;

    void atom();

    void expression();

    void operators();

    void direction();

    void foroperator();

    void coperator();

    void sostoperators();

    void dowhileoperator();

    void term();
    void atom1();

    void specatom();

    void read();

    void var();


    void error(Error_codes error);

public:
    SyntaxAnalysis(vector<lexem> _lexems, bool _need_generate);
    void program();
};

#endif //COMPILER_SYNTAX_ANALYSIS_H
