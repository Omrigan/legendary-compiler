//
// Created by oleg on 4/21/16.
//

#ifndef COMPILER_IDTABLE_H
#define COMPILER_IDTABLE_H
#include <string>
#include <map>

#include "lexem_types.h"
#include "Operands.h"

using namespace std;

class IdTable {
private:
    map<string, data_types> ids;
    map<string, int> int_values;
    map<string, double> double_values;
    map<string, bool> bool_values;
public:
    static IdTable *get_table();

    data_types get_data_type(string name);

    bool is_declared(string name) {
        return ids.find(name) != ids.end();
    }

    void declare(string name, data_types type) {
        ids[name] = type;
    }

    void set_int_value(string name, int value) {
        int_values[name] = value;
    };

    void set_bool_value(string name, bool value) {
        bool_values[name] = value;
    }

    void set_double_value(string name, double value) {
        double_values[name] = value;
    }

    int get_int_value(string name) {
        return int_values[name];
    };

    bool get_bool_value(string name) {
        return bool_values[name];
    }

    double get_double_value(string name) {
        return bool_values[name];
    }


};


#endif //COMPILER_IDTABLE_H
