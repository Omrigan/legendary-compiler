//
// Created by oleg on 5/9/16.
//


#include "string"

#include "IdTable.h"
#include "syntax_analysis.h"


#ifndef COMPILER_OPERANDS_H
#define COMPILER_OPERANDS_H

class IdTable;
using namespace std;
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
class IntOperand;
class BoolOperand;
class DoubleOperand;
class Operand : public PolizItem {
public:
    bool is_operation = false;
    data_types data_type;

    IdTable *id_table;

    static IntOperand *get_sample_int();

    static BoolOperand *get_sample_bool();

    static DoubleOperand *get_sample_double();

};


class IntOperand : public Operand {
public:
    data_types data_type = data_types::INT;

    virtual int get_value() = 0;


};

class DoubleOperand : public Operand {
public:
    data_types data_type = data_types::DOUBLE;

    virtual double get_value() = 0;
};

class BoolOperand : public Operand {
public:
    data_types data_type = data_types::BOOL;


    virtual bool get_value() = 0;
};


class ConstIntOperand : public IntOperand {
private:
    int value;
public:
    int get_value();
};

class ConstDoubleOperand : public DoubleOperand {
private:
    double value;
public:
    double get_value();
};

class ConstBoolOperand : public BoolOperand {
private:
    bool value;
public:
    bool get_value();
};

class IdBoolOperand : public BoolOperand {
private:
    string name;
public:
    IdBoolOperand(string _name);

    bool get_value();
};

class IdIntOperand : public IntOperand {
private:
    string name;
public:
    IdIntOperand(string _name);

    int get_value();
};

class IdDoubleOperand : public DoubleOperand {
private:
    string name;
public:
    IdDoubleOperand(string _name);

    double get_value();
};

#endif //COMPILER_OPERANDS_H
