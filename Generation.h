//
// Created by oleg on 5/22/16.
//

#ifndef COMPILER_GENERATION_H
#define COMPILER_GENERATION_H

#include "lexical_analysis.h"
#include "map"


enum class PolizTypes{
    OPERATION,
    BOOL,
    INT,
    DOUBLE,
    VAR,
    NUM,
    COND,
    UNCOND
};

class poliz_item {
public:

    PolizTypes type;
    string name;
    int int_val;
    double double_val;
    bool bool_val;
};

class Generation {
public:
    vector<lexem> lexems;
    vector<poliz_item> poliz;
    map<string, poliz_item> id_table;
    map<string, int> priority;


    void build();
    vector<poliz_item> get_poliz(int start, int end);
    bool runType();
    void run();

};


#endif //COMPILER_GENERATION_H
