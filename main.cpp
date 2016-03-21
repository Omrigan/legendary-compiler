#include <iostream>
#include <bits/stdc++.h>

using namespace std;
enum types {
    KEYWORD, //0
    INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, // 1 2 3
    OPERATOR, ID, SIGN, // 4 5 6
    OTHER // 7


};
set<string> keywords = {
        "while",
        "if"
};

set<string> signs = {
        "{",
        "}",
        "."
};
vector<string> operators_prior = {
        "++",
        "==",
};

set<string> operators = {
        "++",
        "==",
        "+",
        "=",

};

struct lexem {
    string s;
    types t;


};

bool isFloatLiteral(string s) {
    regex re("(\\d*\\.\\d+|\\d+\\.\\d*)f?|\\d+f");
    return regex_match(s.c_str(), re);
}

bool isIntLiteral(string s) {
    regex re("\\d+");
    return regex_match(s.c_str(), re);
}

bool isStringLiteral(string s) {
    regex re("\".*\"");
    return regex_match(s.c_str(), re);
}

bool isId(string s) {
    regex re("[A-Za-z_]+[A-Za-z0-9_-]*");
    return regex_match(s.c_str(), re);
}

bool isShortComment(string s) {
    regex re("//.*");
    return regex_match(s.c_str(), re);
}

bool isLongCommentBegiging(string s) {
    regex re("/\\*.*");
    return regex_match(s.c_str(), re);
}

bool isLongCommentEnding(string s) {
    regex re(".*\\*/");
    return regex_match(s.c_str(), re);
}

bool isLineEnding(string s) {
    return s == "\n";
}

vector<lexem> analysis(vector<string> strings) {
    vector<lexem> out;
    bool short_comment = false;
    bool long_comment = false;
    for (string s : strings) {
        lexem l;
        l.s = s;
        if (isShortComment(s))
            short_comment = true;
        if (isLongCommentBegiging(s))
            long_comment = true;

        if (isLineEnding(s))
            short_comment = false;
        else if (!long_comment && !short_comment) {
            if (operators.count(s) > 0) l.t = types::OPERATOR;
            else if (signs.count(s) > 0) l.t = types::SIGN;
            else if (keywords.count(s) > 0) l.t = types::KEYWORD;
            else if (isFloatLiteral(s)) l.t = types::FLOAT_LITERAL;
            else if (isStringLiteral(s)) l.t = types::STRING_LITERAL;
            else if (isIntLiteral(s)) l.t = types::INT_LITERAL;
            else if (isId(s)) l.t = types::ID;
            else l.t = types::OTHER;
            out.push_back(l);
        }
        if (isLongCommentEnding(s))
            long_comment = false;


    }
    return out;


}


vector<string> spilt(string s) {

    regex delim("\\s+");
    sregex_token_iterator iter(s.begin(), s.end(), delim, -1);
    sregex_token_iterator end;
    vector<string> out;
    for (; iter != end; ++iter) {
        out.push_back((*iter));
    }
    for (string op : operators_prior) {
        vector<string> out2;
        for (string s : out) {
            string delim(op);
            size_t pos = 0;
            std::string token;
            while ((pos = s.find(delim)) != std::string::npos) {
                s.erase(0, pos + delim.length());
                out2.push_back(delim);
            }
            if (s.size() > 0)
                out2.push_back(s);

        }
        out = out2;
    }
    return out;

}

void run() {
    vector<string> strings;
    while (!cin.eof()) {
        string s;
        getline(cin, s);
        vector<string> tmp = spilt(s);
        for (string s2 : tmp) {
            if (s2.size() > 0)
                strings.push_back(s2);
        }
        strings.push_back("\n");
    }
    vector<lexem> lexems = analysis(strings);
    for (lexem l : lexems) {
        cout << l.t << " " << l.s << endl;
    }

}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    run();
    return 0;
}