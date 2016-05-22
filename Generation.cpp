//
// Created by oleg on 5/22/16.
//

#include "Generation.h"
#include <string>
#include <map>


void Generation::build() {
    priority["--"] = 2;
    priority["++"] = 2;
    priority["**"] = 3;
    priority["!"] = 3; //right
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
    poliz = get_poliz(5, lexems.size()-1);

}
vector<poliz_item> Generation::get_poliz(int start, int end) {
    stack<poliz_item> operations;
    vector<poliz_item> poliz;
    stack<pair<int, int>> go_back;
    for(int i = start; i< end; i++){
        if(lexems[i].t==lexem_types::ID){
            poliz_item pol;
            pol.type = PolizTypes::VAR;
            pol.name = lexems[i].s;
            poliz.push_back(pol);
        }else if(lexems[i].t==lexem_types::INT_LITERAL){
            poliz_item pol;
            pol.type = PolizTypes::INT;
            pol.int_val = atoi(lexems[i].s.c_str());
            poliz.push_back(pol);
        }else if(lexems[i].t==lexem_types::BOOL_LITERAL){
            poliz_item pol;
            pol.type = PolizTypes::BOOL;
            if(lexems[i].s=="true"){
                pol.bool_val = true;
            } else{
                pol.bool_val = false;
            }
            poliz.push_back(pol);
        }else if(lexems[i].t==lexem_types::FLOAT_LITERAL){
            poliz_item pol;
            pol.type = PolizTypes::DOUBLE;
            pol.double_val = atof(lexems[i].s.c_str());
            poliz.push_back(pol);
        } else if(lexems[i].t==lexem_types::OPERATOR){
            int curp = priority[lexems[i].s];
            bool right_assoc = curp==3 or
                    curp==15;
            while(operations.size()>0 and operations.top().name!="(" and
                        (priority[operations.top().name] < curp or
                        (right_assoc and priority[operations.top().name] == curp))) {
                poliz.push_back(operations.top());
                operations.pop();
            }
            poliz_item pol;
            pol.type = PolizTypes::OPERATION;
            pol.name = lexems[i].s;
            operations.push(pol);
        } else if(lexems[i].t==lexem_types::KEYWORD){
            if(lexems[i].s=="for"){
                int i1 = i+1;
                while(lexems[i1].s!=";"){
                    i1++;
                }
                vector<poliz_item> pol = get_poliz(i+2, i1);
                for(poliz_item p : pol){
                    poliz.push_back(p);
                }
                i1++;
                i = i1;
                while(lexems[i1].s!=";"){
                    i1++;
                }

                pol = get_poliz(i, i1);
                for(poliz_item p : pol){
                    poliz.push_back(p);
                }
                poliz_item p1;
                p1.type = PolizTypes::NUM;
                poliz.push_back(p1);
                int x = poliz.size()-1;
                p1.type =  PolizTypes::COND;
                poliz.push_back(p1);


                for(poliz_item p : pol){
                    poliz.push_back(p);
                }
                p1;
                p1.type = PolizTypes::NUM;
                poliz.push_back(p1);
                int y = poliz.size()-1;
                p1.type =  PolizTypes::COND;
                poliz.push_back(p1);
                go_back.push(make_pair(0, y));
                go_back.push(make_pair(1, x));
            }
        }

    }
    return poliz;
};