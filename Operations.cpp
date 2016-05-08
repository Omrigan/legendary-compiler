//
// Created by oleg on 4/21/16.
//
#include "poliz.h"


class Plus : public BinaryOperation {
public:
    const int priority = 5;

    Operand eval(Operand left, Operand right) {


    };

    Operand evalType(Operand left, Operand right) {
//        if (left.is_address or right.is_address) {
//            //Error
//        }
//        if ((left.data_type == data_types::INT or left.data_type == data_types::DOUBLE)
//            and (right.data_type == data_types::INT or right.data_type == data_types::DOUBLE)) {
//            if (left.data_type == data_types::DOUBLE or right.data_type == data_types::DOUBLE) {
//                return ValueOperand(data_types::DOUBLE);
//            }
//            return ValueOperand(data_types::INT);
//
//        } else {
//            //Else
//        }
//

    }
};
