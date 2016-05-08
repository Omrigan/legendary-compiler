//
// Created by oleg on 4/21/16.
//


#include <queue>
#include "stack"
#ifndef COMPILER_POLIZQUEUE_H
#define COMPILER_POLIZQUEUE_H

#include "Operands.cpp"
using namespace std;
class PolizQueue {
private:
    vector<PolizItem*> poliz_queue;
    stack<Operation*> operations_stack;
    stack<Operand*> operand_stack;

public:
    void add_item(PolizItem* item);
    void close_braket();
    Operand run_type();
    Operand run();


};


#endif //COMPILER_POLIZQUEUE_H
