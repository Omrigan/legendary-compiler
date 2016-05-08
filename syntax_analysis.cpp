#include "lexem_types.cpp"
#include "syntax_analysis.h"
#include "stack"
#include "lexical_analysis.h"


#include <chrono>
#include <thread>

using namespace std;

lexem c;


map<string, data_types> string_to_type = {
        {"int",    data_types::INT},
        {"double", data_types::DOUBLE},
        {"bool",   data_types::BOOL}
};


void SyntaxAnalysis::var() {

    if (c.s != "int" && c.s != "bool" && c.s != "double" && c.s != "string") {
        error(Error_codes::MISC);
    }
    else {
        data_types type = string_to_type[c.s];
        read();
        if (c.t != lexem_types::ID) {
            error(Error_codes::MISC);
        }
        else {
            do {
                if (c.t != lexem_types::ID) {
                    error(Error_codes::MISC);
                } else {
                    if (!id_table->is_declared(c.s)) {
                        id_table->declare(c.s, type);
                    } else {
                        error(Error_codes::ALREADY_DECLARED);
                    }
                }
                read();
            } while (c.s == ",");
        }
    }
}

void SyntaxAnalysis::specatom() {
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

void SyntaxAnalysis::atom() {
    if (c.t == lexem_types::ID) {
        if (!id_table->is_declared(c.s))
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

void SyntaxAnalysis::atom1() {
    atom();
    if (c.s == "^") {

        read();
        atom();
    }
    else if (c.s == "++" || c.s == "--") {
        read();
    }
}

void SyntaxAnalysis::term() {
    do {
        atom1();
    } while (c.s == "*" || c.s == "/" || c.s == "&&" || c.s == "div" || c.s == "%");
}


void SyntaxAnalysis::expression() {
    if (c.t == lexem_types::ID) {
        lexem v = c;
        read();
        if (c.s == "=") {
            if (!id_table->is_declared(v.s))
                error(Error_codes::NOT_DECLARED);
            read();
            expression();
            assigned.insert(v.s);
        }
        else {
            c = v;
            term();
        }
    }
    else {
        term();
    }
}

void SyntaxAnalysis::sostoperators() {
    do {
        operators();
    } while (c.s != "}");
}

void SyntaxAnalysis::dowhileoperator() {
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

void SyntaxAnalysis::direction() {
    if (c.s != "to" && c.s != "downto") {
        error(Error_codes::MISC);
    }
}

void SyntaxAnalysis::foroperator() {

    if (c.s != "(") {
        error(Error_codes::MISC);
    }
    else {
        read();
        if (c.t == lexem_types::ID) {
            lexem v = c;
            read();
            if (c.s != ":=") {
                c = v;
                if (c.s == "int" || c.s == "bool" || c.s == "double") {
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
                else {
                    expression();
                    if (c.s != ";") {
                        error(Error_codes::MISC);
                    }
                    else {
                        read();
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
                }
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

            read();
            operators();
            if (c.s == "else") {
                read();
                operators();
            }
        }
    }
}


void SyntaxAnalysis::coperator() {

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
                if (id_table->is_declared(c.s)) {
                    error(Error_codes::NOT_DECLARED);
                }
                else {
                    read();
                }
            }
        }
    } while (c.s == ">>" || c.s == "<<");

}

void SyntaxAnalysis::operators() {
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

void SyntaxAnalysis::program() {
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


void SyntaxAnalysis::read() {
    if (lexem_number >= lexems.size()) {
        c.t = lexem_types::NULL_LEXEM;
        c.s = "";
    } else {
        c = lexems[lexem_number];
        lexem_number++;
    }

}


void SyntaxAnalysis::error(Error_codes error) {


    string err = "Error: %s\nLine: %d. Lexem: '%s'";
//    logfs << err;
    fprintf(stderr, err.c_str(), msg[error].c_str(), c.line, c.s.c_str());
    exit(228);
}


SyntaxAnalysis::SyntaxAnalysis(vector<lexem>
                               _lexems, bool
                               _need_generate) {
    id_table = new IdTable();
    lexems = _lexems;
    msg[Error_codes::MISC] = "Unknown Error";
    msg[Error_codes::NOT_DECLARED] = "Variable not declared yet";
    msg[Error_codes::ALREADY_DECLARED] = "Already declared";
    need_generate = _need_generate;

}


