//
// Created by oleg on 5/22/16.
//

#include "Generation.h"
#include <string>
#include <map>


void Generation::build() {
    priority["--"] = 2;
    priority["++"] = 2;
    priority["**"] = 3; //right
    priority["!"] = 3;
    priority["*"] = 5;
    priority["/"] = 5;
    priority["%"] = 5;
    priority["+"] = 6;
    priority["*"] = 6;
    priority["<<"] = 7;
    priority[">>"] = 7;
    priority["<"] = 8;
    priority[">"] = 8;
    priority["=="] = 9;
    priority["&"] = 10;
    priority["^"] = 11;
    priority["|"] = 12;
    priority["="] = 15; //right
    rpn_queue = get_poliz(5, lexems.size() - 1);

}

struct my_cycle {
    int x_go, //before else substitute
            y_go, // after else substitute
            z; //second cycle position
    int c1, c2; //third section range
    bool is_else_begun;
};

vector<rpn_item> Generation::get_poliz(int start, int end) {
    stack<rpn_item> operations;
    vector<rpn_item> poliz;
    stack<my_cycle> go_back;
    rpn_types context_type;
    my_cycle m;
    go_back.push(m);
    for (int i = start; i < end; i++) {
        if (lexems[i].t == lexem_types::ID) {
            rpn_item pol;
            pol.type = rpn_types::VAR;

            pol.name = lexems[i].s;
            if (id_table.find(pol.name) == id_table.end()) {
                if (context_type == rpn_types::BOOL)
                    id_table[pol.name] = rpn_item::get_bool_item(false);
                if (context_type == rpn_types::INT)
                    id_table[pol.name] = rpn_item::get_int_item(0);
                if (context_type == rpn_types::DOUBLE)
                    id_table[pol.name] = rpn_item::get_double_item(0);

            }


            poliz.push_back(pol);
        } else if (lexems[i].t == lexem_types::INT_LITERAL) {
            rpn_item pol;
            pol.type = rpn_types::INT;
            pol.int_val = atoi(lexems[i].s.c_str());
            poliz.push_back(pol);
        } else if (lexems[i].t == lexem_types::STRING_LITERAL) {
            rpn_item pol;
            pol.type = rpn_types::STRING;
            pol.string_val = lexems[i].s;
            poliz.push_back(pol);
        } else if (lexems[i].t == lexem_types::BOOL_LITERAL) {
            rpn_item pol;
            pol.type = rpn_types::BOOL;
            if (lexems[i].s == "true") {
                pol.bool_val = true;
            } else {
                pol.bool_val = false;
            }
            poliz.push_back(pol);
        } else if (lexems[i].t == lexem_types::FLOAT_LITERAL) {
            rpn_item pol;
            pol.type = rpn_types::DOUBLE;
            pol.double_val = atof(lexems[i].s.c_str());
            poliz.push_back(pol);
        } else if (lexems[i].t == lexem_types::OPERATOR) {
            int curp = priority[lexems[i].s];
            bool right_assoc = curp == 3 or
                               curp == 15;
            while (operations.size() > 0 and operations.top().name != "(" and
                   (priority[operations.top().name] < curp or
                    (!right_assoc and priority[operations.top().name] == curp))) {
                poliz.push_back(operations.top());
                operations.pop();
            }
            rpn_item pol;
            pol.type = rpn_types::OPERATION;
            pol.name = lexems[i].s;
            operations.push(pol);
        } else if (lexems[i].t == lexem_types::KEYWORD) {
            if (lexems[i].s == "for") {
                int i1 = i + 1;
                while (lexems[i1].s != ";") {
                    i1++;
                }
                vector<rpn_item> pol = get_poliz(i + 2, i1);
                for (rpn_item p : pol) {
                    poliz.push_back(p);
                }
                i1++;
                i = i1;
                while (lexems[i1].s != ";") {
                    i1++;
                }

                pol = get_poliz(i, i1);
                for (rpn_item p : pol) {
                    poliz.push_back(p);
                }

                rpn_item p1;
                p1.type = rpn_types::POS;
                poliz.push_back(p1);
                my_cycle cyc;
                cyc.x_go = poliz.size() - 1;
                p1.type = rpn_types::COND;
                poliz.push_back(p1);

                cyc.z = poliz.size();
                for (rpn_item p : pol) {
                    poliz.push_back(p);
                }
                p1;
                p1.type = rpn_types::POS;
                poliz.push_back(p1);
                cyc.y_go = poliz.size() - 1;
                p1.type = rpn_types::COND;
                poliz.push_back(p1);

                i = i1;
                i++;
                i1 = i + 1;
                while (lexems[i1].s != ")") {
                    i1++;
                }
                cyc.c1 = i;
                cyc.c2 = i1;
                go_back.push(cyc);
                i = i1;
                i++;
            } else if (lexems[i].s == "cinout") {
                rpn_item pol;
                pol.type = rpn_types::CINOUT;
                operations.push(pol);
            } else if (lexems[i].s == "bool") {
                context_type = rpn_types::BOOL;
            } else if (lexems[i].s == "int") {
                context_type = rpn_types::INT;
            } else if (lexems[i].s == "double") {
                context_type = rpn_types::DOUBLE;
            }

        } else if (lexems[i].t == lexem_types::SIGN) {
            if (lexems[i].s == "}") {
                if (go_back.top().is_else_begun) {

                    assert(poliz[go_back.top().y_go].type == rpn_types::POS);
                    poliz[go_back.top().y_go].int_val = poliz.size();
                    go_back.pop();

                } else {
                    auto p = go_back.top();
                    vector<rpn_item> pol = get_poliz(p.c1, p.c2);
                    for (rpn_item p : pol) {
                        poliz.push_back(p);
                    }
                    rpn_item poliz_item1;
                    poliz_item1.type = rpn_types::POS;
                    poliz_item1.int_val = p.z;
                    poliz.push_back(poliz_item1);
                    poliz_item1.type = rpn_types::UNCOND;
                    poliz.push_back(poliz_item1);

                    assert(poliz[p.x_go].type == rpn_types::POS);
                    poliz[p.x_go].int_val = poliz.size();
                    if (lexems[i + 1].s == "else") {
                        go_back.top().is_else_begun = true;
                        i++;
                    } else {
                        go_back.pop();

                        assert(poliz[p.y_go].type == rpn_types::POS);

                        poliz[p.y_go].int_val = poliz.size();
                    }
                }
            } else if (lexems[i].s == "(") {
                rpn_item pol;
                pol.type = rpn_types::OPERATION;
                pol.name = lexems[i].s;
                operations.push(pol);
            } else if (lexems[i].s == ")") {
                while (operations.top().name != "(") {
                    poliz.push_back(operations.top());
                    operations.pop();
                }
                operations.pop();
            } else if (lexems[i].s == ";") {
                while (!operations.empty()) {
                    poliz.push_back(operations.top());
                    operations.pop();
                }
            }
        }

    }
    while (!operations.empty()) {
        poliz.push_back(operations.top());
        operations.pop();
    }
    return poliz;
};

void Generation::dump_to_file(string s) {
    ofstream rpnfs(s);
    int cur = 0;
    for (auto it : rpn_queue) {
        rpnfs << cur << " " << names[it.type] << " " << it.name << " " << it.bool_val <<
        " " << it.int_val << " " << it.double_val << endl;
        cur++;
    }
}

void Generation::run() {
    int cur = 0;
    stack<rpn_item> eval;
    while (cur < this->rpn_queue.size()) {
        if (rpn_queue[cur].type == rpn_types::INT) {
            eval.push(rpn_queue[cur]);
        } else if (rpn_queue[cur].type == rpn_types::DOUBLE) {
            eval.push(rpn_queue[cur]);
        } else if (rpn_queue[cur].type == rpn_types::BOOL or
                   rpn_queue[cur].type == rpn_types::STRING) {
            eval.push(rpn_queue[cur]);
        }
        else if (rpn_queue[cur].type == rpn_types::VAR) {
            eval.push(rpn_queue[cur]);
        } else if (rpn_queue[cur].type == rpn_types::POS) {
            eval.push(rpn_queue[cur]);
        } else if (rpn_queue[cur].type == rpn_types::CINOUT) {
            eval.push(rpn_queue[cur]);
        } else if (rpn_queue[cur].type == rpn_types::UNCOND) {
            cur = eval.top().int_val - 1;
            eval.pop();
        } else if (rpn_queue[cur].type == rpn_types::COND) {
            int tmp = eval.top().int_val - 1;
            eval.pop();
            if (!eval.top().bool_val) {
                cur = tmp;
            }
            eval.pop();
        } else if (rpn_queue[cur].type == rpn_types::OPERATION) {
            if (rpn_queue[cur].name == "+") {
                rpn_item p2 = get_value(eval.top());
                eval.pop();
                rpn_item p1 = get_value(eval.top());
                eval.pop();
                double result = 0;
                bool is_double = false;
                if (p1.type == rpn_types::DOUBLE) {
                    is_double = true;
                    result += p1.double_val;
                } else {
                    result += p1.int_val;
                }
                if (p2.type == rpn_types::DOUBLE) {
                    is_double = true;
                    result += p2.double_val;
                } else {
                    result += p2.int_val;
                }
                if (is_double) {
                    rpn_item it;
                    it.type = rpn_types::DOUBLE;
                    it.double_val = result;
                    eval.push(it);
                } else {
                    rpn_item it;
                    it.type = rpn_types::INT;
                    it.int_val = result;
                    eval.push(it);
                }
            }
            if (rpn_queue[cur].name == "-") {
                rpn_item p2 = get_value(eval.top());
                eval.pop();
                rpn_item p1 = get_value(eval.top());
                eval.pop();
                double result = 0;
                bool is_double = false;
                if (p1.type == rpn_types::DOUBLE) {
                    is_double = true;
                    result += p1.double_val;
                } else {
                    result += p1.int_val;
                }
                if (p2.type == rpn_types::DOUBLE) {
                    is_double = true;
                    result -= p2.double_val;
                } else {
                    result -= p2.int_val;
                }
                if (is_double) {
                    rpn_item it;
                    it.type = rpn_types::DOUBLE;
                    it.double_val = result;
                    eval.push(it);
                } else {
                    rpn_item it;
                    it.type = rpn_types::INT;
                    it.int_val = result;
                    eval.push(it);
                }
            }
            if (rpn_queue[cur].name == "*") {
                rpn_item p2 = get_value(eval.top());
                eval.pop();
                rpn_item p1 = get_value(eval.top());
                eval.pop();
                double result = 1;
                bool is_double = false;
                if (p1.type == rpn_types::DOUBLE) {
                    is_double = true;
                    result *= p1.double_val;
                } else {
                    result *= p1.int_val;
                }
                if (p2.type == rpn_types::DOUBLE) {
                    is_double = true;
                    result *= p2.double_val;
                } else {
                    result *= p2.int_val;
                }
                if (is_double) {
                    rpn_item it;
                    it.type = rpn_types::DOUBLE;
                    it.double_val = result;
                    eval.push(it);
                } else {
                    rpn_item it;
                    it.type = rpn_types::INT;
                    it.int_val = result;
                    eval.push(it);
                }
            }
            if (rpn_queue[cur].name == "/") {
                rpn_item p2 = get_value(eval.top());
                eval.pop();
                rpn_item p1 = get_value(eval.top());
                eval.pop();

                if (p1.type == rpn_types::INT) {
                    if (p2.type == rpn_types::INT) {
                        eval.push(rpn_item::get_int_item(p1.int_val / p2.int_val));
                    } else if (p2.type == rpn_types::DOUBLE) {
                        eval.push(rpn_item::get_double_item(p1.int_val / p2.double_val));
                    }
                } else if (p1.type == rpn_types::DOUBLE) {
                    if (p2.type == rpn_types::INT) {
                        eval.push(rpn_item::get_double_item(p1.double_val / p2.int_val));
                    } else if (p2.type == rpn_types::DOUBLE) {
                        eval.push(rpn_item::get_double_item(p1.double_val / p2.double_val));
                    }
                }
            }
            if (rpn_queue[cur].name == "<<") {
                rpn_item p2 = get_value(eval.top());
                eval.pop();
                rpn_item p1 = get_value(eval.top());
                eval.pop();

                if (p1.type == rpn_types::CINOUT) {
                    if (p2.type == rpn_types::DOUBLE) {
                        cout << p2.double_val;
                    } else if (p2.type == rpn_types::INT) {
                        cout << p2.int_val;
                    } else if (p2.type == rpn_types::BOOL) {
                        cout << p2.bool_val;
                    } else if (p2.type == rpn_types::STRING) {
                        cout << p2.string_val;
                    }
                    eval.push(p1);
                } else if (p1.type == rpn_types::INT) {
                    if (p2.type == rpn_types::INT) {
                        eval.push(rpn_item::get_int_item(p1.int_val << p2.int_val));
                    }
                }

            }
            if (rpn_queue[cur].name == ">>") {
                rpn_item p2 = eval.top();
                eval.pop();
                rpn_item p1 = eval.top();
                eval.pop();

                if (p1.type == rpn_types::CINOUT) {
                    if (p2.type == rpn_types::VAR) {
                        rpn_types cur_t = id_table[p2.name].type;
                        if (cur_t == rpn_types::INT) {
                            cin >> id_table[p2.name].int_val;
                        } else if (cur_t == rpn_types::DOUBLE) {
                            cin >> id_table[p2.name].double_val;
                        }


                    }
                    eval.push(p1);
                } else if (p1.type == rpn_types::INT) {
                    if (p2.type == rpn_types::INT) {
                        eval.push(rpn_item::get_int_item(p1.int_val >> p2.int_val));
                    }
                }

            }
            if (rpn_queue[cur].name == "<") {
                rpn_item p2 = get_value(eval.top());
                eval.pop();
                rpn_item p1 = get_value(eval.top());
                eval.pop();

                bool res;
                if (p1.type == rpn_types::DOUBLE) {
                    if (p2.type == rpn_types::DOUBLE) {
                        res = p1.double_val < p2.double_val;
                    } else if (p2.type == rpn_types::INT) {
                        res = p1.double_val < p2.int_val;
                    }
                } else if (p2.type == rpn_types::INT) {
                    if (p2.type == rpn_types::DOUBLE) {
                        res = p1.int_val < p2.double_val;
                    } else if (p2.type == rpn_types::INT) {
                        res = p1.int_val < p2.int_val;
                    }
                }
                eval.push(rpn_item::get_bool_item(res));
            }
            if (rpn_queue[cur].name == "++") {

                rpn_item p1 = eval.top();


                rpn_item res;

                if (id_table[p1.name].type == rpn_types::INT) {
                    id_table[p1.name].int_val++;
                } else if (id_table[p1.name].type == rpn_types::DOUBLE) {
                    id_table[p1.name].double_val++;
                }
            }
            if (rpn_queue[cur].name == "--") {

                rpn_item p1 = eval.top();


                rpn_item res;

                if (id_table[p1.name].type == rpn_types::INT) {
                    id_table[p1.name].int_val--;
                } else if (id_table[p1.name].type == rpn_types::DOUBLE) {
                    id_table[p1.name].double_val--;
                }
            }
            if (rpn_queue[cur].name == "=") {
                rpn_item p2 = get_value(eval.top());
                eval.pop();
                rpn_item p1 = eval.top();
                eval.pop();


                if (p2.type == rpn_types::BOOL) {
                    id_table[p1.name] = rpn_item::get_bool_item(p2.bool_val);
                } else if (p2.type == rpn_types::INT) {
                    id_table[p1.name] = rpn_item::get_int_item(p2.int_val);
                } else if (p2.type == rpn_types::DOUBLE) {
                    id_table[p1.name] = rpn_item::get_double_item(p2.double_val);
                }
                eval.push(p1);
            }
        }

        cur++;
    }
}