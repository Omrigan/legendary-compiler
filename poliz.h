//
// Created by oleg on 4/20/16.
//

#ifndef COMPILER_POLIZ_H
#define COMPILER_POLIZ_H

#include "IdTable.h"

using namespace std;

class IdTable;

class PolizItem {
public:
    bool is_operation;

    PolizItem();
};


class Operation : public PolizItem {
public:
    bool is_operation = true;
    bool is_unary;
    bool is_skobka = false;
    int priority;
    bool left_assosiative;

};

class Skobka : public Operation {
public:
    bool is_skobka = true;
    int priority = 20;
};

class Operand;

class UnaryOperation : public Operation {
public:
    bool is_unary = true;

    virtual Operand eval(Operand left);

    virtual Operand typeEval(Operand left);
};

class BinaryOperation : public Operation {
public:
    bool is_unary = false;

    virtual Operand eval(Operand left, Operand right);

    virtual Operand typeEval(Operand left, Operand right);
};

class Operand : public PolizItem {
public:
    bool is_operation = false;
    data_types data_type;
    IdTable *id_table;

    Operand(IdTable *_id_table) {
        id_table = _id_table;
    }
};


class IntOperand : public Operand {
public:
    data_types data_type = data_types::INT;
    virtual int get_value();
};

class DoubleOperand : public Operand {
public:
    data_types data_type = data_types::DOUBLE;
    virtual double get_value();
};

class BoolOperand : public Operand {
public:
    data_types data_type = data_types::BOOL;
    virtual bool get_value();
};


class ConstIntOperand : public IntOperand {
private:
    int value;
public:
    int get_value() {
        return value;
    }
};

class ConstDoubleOperand : public DoubleOperand {
private:
    double value;
public:
    double get_value() {
        return value;
    }
};

class ConstBoolOperand : public BoolOperand {
private:
    bool value;
public:
    bool get_value() {
        return value;
    }
};


#endif //COMPILER_POLIZ_H
