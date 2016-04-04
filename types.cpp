//
// Created by oleg on 4/4/16.
//

#ifndef COMPILER_ENUMS_H
#define COMPILER_ENUMS_H

#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>

enum types {
    KEYWORD, //0
    INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, BOOL_LITERAL, // 1 2 3 4
    OPERATOR, ID, SIGN, // 5 6 7
    OTHER, // 8
    SPAM,
    NULL_LEXEM
};
enum vertex {
    ZERO,
    COMMENT_SHORT, COMMENT_LONG, COMMENT_END_STAR, COMMENT_END_SLASH,
    STRING, END_STRING,
    INT, FLOAT, FLOAT_END,
    ID_END,
    PLUSMINUS,
    SIGN_END,
    DOT, SLASH,
    OPERATOR_END,
    _ERROR

};
struct lexem {
    std::string s;
    types t;
};
#endif //COMPILER_ENUMS_H
