#include "lexem_types.cpp"
#include "syntax_analysis.h"
#include "stack"


#include <chrono>
#include <thread>

using namespace std;

lexem c;


typedef map<string, Var_types> id_table;
id_table ids;
//stack<set<string>> memory_stack;
map<string, string> values;
set<string> assigned;


vector<lexem> lexems;

int lexem_number = 0;

map<string, Var_types> string_to_type = {
        {"int",    Var_types::INT},
        {"double", Var_types::DOUBLE},
        {"string", Var_types::STRING},
        {"bool",   Var_types::BOOL}
};


void var() {

    if (c.s != "int" && c.s != "bool" && c.s != "double" && c.s != "string") {
        error(Error_codes::MISC);
    }
    else {
        Var_types type = string_to_type[c.s];
        read();
        if (c.t != lexem_types::ID) {
            error(Error_codes::MISC);
        }
        else {
            do {
                if (c.t != lexem_types::ID) {
                    error(Error_codes::MISC);
                } else {
                    if (ids.find(c.s) == ids.end()) {
                        ids[c.s] = type;
                    } else {
                        error(Error_codes::ALREADY_DECLARED);
                    }
                }
                read();
            } while (c.s == ",");
        }
    }
}

void specatom() {
    if (c.t == lexem_types::FLOAT_LITERAL || c.t == lexem_types::INT_LITERAL || c.t == lexem_types::BOOL_LITERAL) {
        read();
    }
    else if (c.s == "!") {
        read();
        atom();
    }
    else {
        error(Error_codes::MISC);
    }
}

void atom() {
    if (c.t == lexem_types::ID) {
        if (values.find(c.s) == values.end())
            error(Error_codes::NOT_DECLARED);
        read();

    }
    else if (c.s == "(") {
        read();
        expression();
        if (c.s != ")") {
            error(Error_codes::MISC);
        }
        else {
            read();
        }
    }
    else {
        specatom();
    }
}

void atom1() {
    atom();
    if (c.s == "^") {
        read();
        atom();
    }
    else if (c.s == "++" || c.s == "--") {
        read();
    }
}

void term() {
    do {
        atom1();
    } while (c.s == "*" || c.s == "/" || c.s == "&&" || c.s == "div" || c.s == "%");
}

void expression() {
    if (c.t == lexem_types::ID) {
        lexem v = c;
        read();
        if (c.s == "=") {
            if (ids.find(c.s) == ids.end())
                error(Error_codes::NOT_DECLARED);
            read();
            expression();
            assigned.insert(v.s);
        }
        else {
            c = v;
            read();//FIXME: MAY READ GOES AHEAD?
            term();
        }
    }
    else {
        term();
    }
}

void sostoperators() {
    do {
        operators();
    } while (c.s != "}");
}

void dowhileoperator() {
    operators();
    if (c.s != "while") {
        error(Error_codes::MISC);
    }
    else {
        read();
        if (c.s != "(") {
            error(Error_codes::MISC);
        }
        else {
            read();
            expression();
            if (c.s != ")") {
                error(Error_codes::MISC);
            }
            else {
                read();
            }
        }
    }
}

void direction() {
    if (c.s != "to" && c.s != "downto") {
        error(Error_codes::MISC);
    }
}

void foroperator() {
    if (c.s != "(") {
        error;
    }
    else {
        read();
        if (c.t == lexem_types::ID || c.s == "(" || c.t == lexem_types::FLOAT_LITERAL ||
            c.t == lexem_types::INT_LITERAL || c.t == lexem_types::BOOL_LITERAL ||
            c.s == "!") {
            expression();
            if (c.s != ";") {
                error(Error_codes::MISC);
            }
            else {
                read();
                if (c.t == lexem_types::ID || c.s == "(" || c.t == lexem_types::FLOAT_LITERAL ||
                    c.t == lexem_types::INT_LITERAL || c.t == lexem_types::BOOL_LITERAL ||
                    c.s == "!") {
                    expression();
                    if (c.s != ";") {
                        error(Error_codes::MISC);
                    }
                    else {
                        read();
                        if (c.t == lexem_types::ID || c.s == "(" || c.t == lexem_types::FLOAT_LITERAL ||
                            c.t == lexem_types::INT_LITERAL ||
                            c.t == lexem_types::BOOL_LITERAL || c.s == "!") {
                            expression();
                            if (c.s != ")") {
                                error(Error_codes::MISC);
                            }
                        }
                        else {
                            error(Error_codes::MISC);
                        }
                    }
                }
                else {
                    error(Error_codes::MISC);
                }
            }
        }
        else if (c.t == lexem_types::ID) {
            read();
            if (c.s != ":=") {
                error(Error_codes::MISC);
            }
            else {
                read();
                expression();
                direction();
                expression();
                if (c.s != ")") {
                    error(Error_codes::MISC);
                }
            }
        }
        else if (c.s == "int" || c.s == "bool" || c.s == "double") {
            var();
            expression();
            if (c.s != ";") {
                error(Error_codes::MISC);
            }
            else {
                read();
                expression();
                if (c.s != ")") {
                    error(Error_codes::MISC);
                }
            }
        }
        read();
        operators();
        if (c.s == "else") {
            read();
            operators();
        }
    }
}

void coperator() {
    do {
        if (c.s == "<<") {
            read();
            if (c.s != "\"" && c.s != "endl") {
                expression();
            }
            else {
                read();
            }
        }
        else if (c.s == ">>") {
            read();
            if (c.t != lexem_types::ID) {
                error(Error_codes::MISC);
            }
            else {
                read();
            }
        }
    } while (c.s == ">>" || c.s == "<<");
}

void operators() {
    if (c.s == "{") {
        read();
        sostoperators();
        read();
    }
    else if (c.s == "do") {
        read();
        dowhileoperator();
    }
    else if (c.s == "for") {
        read();
        foroperator();
    }
    else if (c.s == "int" || c.s == "bool" || c.s == "double") {
        var();
    }
    else if (c.s == "cinout") {
        read();
        coperator();
    }
    else if (c.t == lexem_types::ID || c.s == "(" || c.t == lexem_types::FLOAT_LITERAL ||
             c.t == lexem_types::INT_LITERAL || c.t == lexem_types::BOOL_LITERAL ||
             c.s == "!") {
        expression();
    }
    else {
        error(Error_codes::MISC);
    }
}

map<Error_codes, string> msg;

void program(vector<lexem> _lexems) {
    msg[Error_codes::MISC] = "Unknown Error";
    msg[Error_codes::NOT_DECLARED] = "Variable not declared yet";
    msg[Error_codes::ALREADY_DECLARED] = "Already declared";

    lexems = _lexems;
    read();
    if (c.t != lexem_types::KEYWORD) {
        error(Error_codes::MISC);
    }
    else {
        string z = c.s;
        read();
        while (c.s != "main") {
            c.s = z;
            var();
            z = c.s;
            read();
        };
        read();
        if (c.s != "(") {
            error(Error_codes::MISC);
        }
        else {
            read();
            if (c.s != ")") {
                error(Error_codes::MISC);
            }
            else {
                read();
                operators();
            }
        }
    }
}


void read() {
    if (lexem_number >= lexems.size()) {
        c.t = lexem_types::NULL_LEXEM;
        c.s = "";
    } else {
        c = lexems[lexem_number];
        lexem_number++;
    }

}


void error(Error_codes error) {

    cerr << "Syntax error:" << endl;
    cerr << msg[error] << endl;
    cerr << "At line: " << c.line << ". Lexem: '" << c.s << "'" << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    exit(228);
}