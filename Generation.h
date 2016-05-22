//
// Created by oleg on 5/22/16.
//

#ifndef COMPILER_GENERATION_H
#define COMPILER_GENERATION_H

#include "lexical_analysis.h"
#include "map"


enum class rpn_types{
    OPERATION,
    BOOL,
    INT,
    DOUBLE,
    STRING,
    VAR,
    NUM,
    COND,
    UNCOND,
    CINOUT
};

class rpn_item {
public:

    rpn_types type;
    string name;
    int int_val;
    double double_val;
    bool bool_val;
    string string_val;
    static rpn_item get_bool_item(bool value){
        rpn_item item;
        item.type = rpn_types ::BOOL;
        item.bool_val = value;
        return item;
    }
    static rpn_item get_int_item(int value){
        rpn_item item;
        item.type = rpn_types ::INT;
        item.int_val = value;
        return item;
    }
    static rpn_item get_double_item(double value){
        rpn_item item;
        item.type = rpn_types ::DOUBLE;
        item.double_val = value;
        return item;
    }

};

class Generation {
public:
    vector<lexem> lexems;
    vector<rpn_item> rpn_queue;
    map<string, rpn_item> id_table;
    map<string, int> priority;
    map<rpn_types, string> names = {
            {rpn_types::OPERATION, "OPERATION"},
            {rpn_types::BOOL, "BOOL"},
            {rpn_types::INT, "INT"},
            {rpn_types::DOUBLE, "DOUBLE"},
            {rpn_types::STRING, "STRING"},
            {rpn_types::VAR, "VAR"},
            {rpn_types::NUM, "NUM"},
            {rpn_types::COND, "COND"},
            {rpn_types::UNCOND, "UNCOND"},
            {rpn_types::CINOUT, "CINOUT"}
    };



    void build();
    vector<rpn_item> get_poliz(int start, int end);
    void dump_to_file(string s);
    bool runType();
    void run();
    rpn_item get_value(rpn_item a){
        if(a.type==rpn_types::VAR){
            return id_table[a.name];
        }
        return a;
    }

};


#endif //COMPILER_GENERATION_H
