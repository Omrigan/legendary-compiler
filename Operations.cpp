//
// Created by oleg on 4/21/16.
//
#include "Operands.h"
#include "Operations.h"


Operand *Plus::eval(Operand *left, Operand *right) {


};

Operand *Plus::evalType(Operand *left, Operand *right) {
    if ((left->data_type == data_types::INT or left->data_type == data_types::DOUBLE)
        and (right->data_type == data_types::INT or right->data_type == data_types::DOUBLE)) {
        if (left->data_type == data_types::DOUBLE or right->data_type == data_types::DOUBLE) {
            return Operand::get_sample_double();
        }
        return Operand::get_sample_int();
    } else {
        //Else
        return Operand::get_sample_double();
    }
}


