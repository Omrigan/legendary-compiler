//
// Created by oleg on 4/20/16.
//


#include "IdTable.h"
#include "Operands.h"


int ConstIntOperand::get_value() {
    return value;
}

double ConstDoubleOperand::get_value() {
    return value;
}


bool ConstBoolOperand::get_value() {
    return value;
}

IdBoolOperand::IdBoolOperand(string _name) {
    name = _name;
}

bool IdBoolOperand::get_value() {
    return IdTable::get_table()->get_bool_value(name);
}


IdIntOperand::IdIntOperand(string _name) {
    name = _name;
}

int IdIntOperand::get_value() {
    return IdTable::get_table()->get_int_value(name);
}


IdDoubleOperand::IdDoubleOperand(string _name) {
    name = _name;
}

double IdDoubleOperand::get_value() {
    return IdTable::get_table()->get_double_value(name);
}


