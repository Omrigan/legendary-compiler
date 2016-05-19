//
// Created by oleg on 5/9/16.
//

#ifndef COMPILER_OPERATIONS_H
#define COMPILER_OPERATIONS_H

#include "Operations.h"
class Plus : public BinaryOperation {
public:
    const int priority = 5;

    Operand *eval(Operand *left, Operand *right);

    Operand *evalType(Operand *left, Operand *right);
};

class Substruct : public BinaryOperation {
public:
    const int priority = 5;

    Operand *eval(Operand *left, Operand *right);

    Operand *evalType(Operand *left, Operand *right);
};

class Assignation : public BinaryOperation {
public:
    const int priority = 228;

};

#endif //COMPILER_OPERATIONS_H
