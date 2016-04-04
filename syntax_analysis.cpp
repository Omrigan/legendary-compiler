#include "types.cpp"
#include "syntax_analysis.h"



using namespace std;

lexem c;


vector<lexem> lexems;

int lexem_number = 0;

void var(){
    if (c.s != "int" && c.s != "bool" && c.s != "double"){
        error();
    }
    else{
        read();
        if (c.t != ID){
            error();
        }
        else{
            read();
            while (c.s == ","){
                read();
                if (c.t != ID){
                    error();
                }
            }
        }
    }
}

void specatom(){
    if (c.t == FLOAT_LITERAL || c.t == INT_LITERAL || c.t == BOOL_LITERAL){
        read();
    }
    else if (c.s == "!"){
        read();
        atom();
    }
    else {
        error();
    }
}

void atom(){
    if (c.t == 2){
        read();
    }
    else if (c.s == "("){
        read();
        expression();
        if (c.s != ")"){
            error();
        }
        else{
            read();
        }
    }
    else {
        specatom();
    }
}

void atom1(){
    atom();
    if (c.s == "^"){
        read();
        atom();
    }
    else if (c.s == "++" || c.s == "--"){
        read();
    }
}

void term(){
    do{
        atom1();
    }while (c.s == "*" || c.s == "/" || c.s == "&&" || c.s == "div" || c.s == "%");
}

void expression(){
    if (c.t == ID){
        types t1 = c.t;
        string c1 = c.s;
        read();
        if (c.s != "="){
            c.t = t1;
            c.s = c1;
            read();
            term();
        }
        else {
            read();
            expression();
        }
    }
    else{
        term();
    }
}

void sostoperators(){
    do{
        operators();
    } while (c.s != "}");
}

void dowhileoperator(){
    operators();
    if (c.s != "while"){
        error();
    }
    else{
        read();
        if (c.s != "("){
            error();
        }
        else{
            read();
            expression();
            if (c.s != ")"){
                error();
            }
            else {
                read();
            }
        }
    }
}

void direction(){
    if (c.s != "to" && c.s != "downto"){
        error();
    }
}

void foroperator(){
    if (c.s != "("){
        error;
    }
    else{
        read();
        if (c.t == ID || c.s == "(" || c.t == FLOAT_LITERAL || c.t == INT_LITERAL || c.t== BOOL_LITERAL || c.s == "!" ){
            expression();
            if (c.s != ";"){
                error();
            }
            else{
                read();
                if (c.t == ID || c.s == "(" || c.t == FLOAT_LITERAL || c.t == INT_LITERAL ||  c.t== BOOL_LITERAL || c.s == "!"){
                    expression();
                    if (c.s != ";"){
                        error();
                    }
                    else{
                        read();
                        if (c.t == ID || c.s == "(" || c.t == FLOAT_LITERAL || c.t == INT_LITERAL ||c.t == BOOL_LITERAL || c.s == "!"){
                            expression();
                            if (c.s != ")"){
                                error();
                            }
                        }
                        else{
                            error();
                        }
                    }
                }
                else{
                    error();
                }
            }
        }
        else if (c.t == ID){
            read();
            if (c.s != ":="){
                error();
            }
            else{
                read();
                expression();
                direction();
                expression();
                if (c.s != ")"){
                    error();
                }
            }
        }
        else if (c.s == "int" || c.s == "bool" || c.s =="double"){
            var();
            expression();
            if (c.s != ";"){
                error();
            }
            else{
                read();
                expression();
                if (c.s != ")"){
                    error();
                }
            }
        }
        read();
        operators();
        if (c.s == "else"){
            read();
            operators();
        }
    }
}

void coperator(){
    do{
        if (c.s == "<<"){
            read();
            if (c.s != "\"" && c.s != "endl"){
                expression();
            }
            else {
                read();
            }
        }
        else if (c.s == ">>"){
            read();
            if (c.t != ID){
                error();
            }
            else{
                read();
            }
        }
    }while (c.s == ">>" || c.s == "<<");
}

void operators(){
    if (c.s=="{"){
        read();
        sostoperators();
        read();
    }
    else if (c.s == "do"){
        read();
        dowhileoperator();
    }
    else if (c.s == "for"){
        read();
        foroperator();
    }
    else if (c.s =="int" || c.s == "bool" || c.s == "double"){
        var();
    }
    else if (c.s == "cinout"){
        read();
        coperator();
    }
    else if (c.t == ID || c.s == "(" || c.t == FLOAT_LITERAL || c.t == INT_LITERAL ||  c.t == BOOL_LITERAL || c.s == "!"){
        expression();
    }
    else{
        error();
    }
}

void program(vector<lexem> lexems){
    if (c.t != KEYWORD) {
        error();
    }
    else{
        string z=c.s;
        read();
        while (c.s != "main"){
            c.s = z;
            var();
            z = c.s;
            read();
        };
        read();
        if (c.s != "("){
            error();
        }
        else{
            read();
            if (c.s != ")"){
                error();
            }
            else{
                read();
                operators();
            }
        }
    }
}


void read(){
    if(lexem_number>=lexems.size()){
        lexem c;
        c.t = NULL_LEXEM;
        c.s = "";
    } else {
        c = lexems[lexem_number];
        lexem_number++;
    }

}

void error(){
    cerr << "Syntax error" << endl;
    exit(228);
}