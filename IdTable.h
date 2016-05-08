//
// Created by oleg on 4/21/16.
//

#ifndef COMPILER_IDTABLE_H
#define COMPILER_IDTABLE_H

#include "lexem_types.cpp"
#include <string>
#include <map>
#include "poliz.h"

using namespace std;

class IdTable {
private:
    map<string, data_types > ids;
    map<string, int> int_values;
    map<string, int> double_values;
    map<string, int> bool_values;
public:
    bool is_declared(string name) {
        return ids.find(name)!=ids.end();
    }

    void declare(string name, data_types type) {
        ids[name] = type;
    }

    void set_int_value(string name, int value) {
        int_values[name] = value;
    };
    void set_bool_value(string name, bool value){
        bool_values[name] = value;
    }


};


#endif //COMPILER_IDTABLE_H
