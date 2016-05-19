//
// Created by oleg on 4/21/16.
//

#include "PolizQueue.h"
#include "Operands.h"

using namespace std;

void PolizQueue::add_item(PolizItem *item) {
    if (item->is_operation) {
        Operation *item_op = (Operation *) item;
        while (operations_stack.top()->priority < item_op->priority or (
                operations_stack.top()->priority == item_op->priority and (!item_op->left_assosiative)
        )) {
            poliz_queue.push_back(operations_stack.top());
            operations_stack.pop();
        }
        operations_stack.push(item_op);
    } else {
        poliz_queue.push_back(item);

    }
}

void PolizQueue::close_braket() {
    while (!operations_stack.top()->is_skobka) {
        poliz_queue.push_back(operations_stack.top());
        operations_stack.pop();
    }
}

Operand PolizQueue::run_type() {
    for (auto it = poliz_queue.begin(); it != poliz_queue.end(); ++it) {
        if ((*it)->is_operation) {
            Operation *item_op = (Operation *) *it;
            if (item_op->is_unary) {
                UnaryOperation *item_unop = (UnaryOperation *) item_op;
                Operand res = item_unop->typeEval(*operand_stack.top());
                operand_stack.pop();
                operand_stack.push(&res);
            } else {
                BinaryOperation *item_binop = (BinaryOperation *) item_op;
                Operand left = *operand_stack.top();
                operand_stack.pop();
                Operand right = *operand_stack.top();
                operand_stack.pop();
                Operand res = item_binop->typeEval(left, right);
                operand_stack.push(&res);
            }
        } else {
            operand_stack.push((Operand *) *it);
        }
    }
    return *operand_stack.top();
}

//AddressOperand::AddressOperand(data_types _data_type) {
//    data_type = _data_type;
//}
//
//ValueOperand::ValueOperand(data_types _data_type) {
//    data_type = _data_type;
//}
//
