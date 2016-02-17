#include <iostream>
#include <bits/stdc++.h>

using namespace std;
enum types {
    KEYWORD,
    INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL,
    OPERATOR, ID, SIGN,
    OTHER


};
unordered_set<string> keywords = {
        "while",
        "if"
};

unordered_set<string> signs = {
        "{",
        "}"
};

unordered_set<string> operators = {
        "+",
        "=",
        "=="
};

struct lexem {
    string s;
    types t;


};
bool isFloatLiteral (string s){
    regex re("(\\d*\\.\\d+|\\d+\\.\\d*)f?|\\d+f");
    return regex_match(s.c_str(), re);
}


vector<lexem> analysis(vector<string> strings) {
    vector<lexem> out;
    for (string s : strings) {
        lexem l;
        l.s = s;
        if (operators.count(s) > 0) l.t = types::OPERATOR;
        else if (signs.count(s) > 0) l.t = types::SIGN;
        else if (keywords.count(s) > 0) l.t = types::KEYWORD;
        else if (isFloatLiteral(s)) l.t = types::FLOAT_LITERAL;
        else l.t = types::OTHER;
        out.push_back(l);
    }
    return out;


}


vector<string> spilt(string s) {

    regex delim("\\s+");
    sregex_token_iterator iter(s.begin(), s.end(), delim, -1);
    std::sregex_token_iterator end;
    vector<string> out;
    for (; iter != end; ++iter) {
        out.push_back((*iter));

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
            transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
            if (s2.size() > 0)
                strings.push_back(s2);
        }
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