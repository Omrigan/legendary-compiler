#include <iostream>
#include <bits/stdc++.h>

using namespace std;
enum types {
    KEYWORD, //0
    INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, // 1 2 3
    OPERATOR, ID, SIGN, // 4 5 6
    OTHER, // 7
    SPAM
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
    string s;
    types t;


};
map<vertex, map<char, vertex>> edges;
map<vertex, types> terminals;


void build() {
    edges[vertex::ZERO] = {
            {'\"', vertex::STRING},
            {'-',  vertex::PLUSMINUS},
            {'+',  vertex::PLUSMINUS},
            {'=',  vertex::PLUSMINUS},
            {'*',  vertex::PLUSMINUS},
            {'{',  vertex::SIGN_END},
            {'}',  vertex::SIGN_END},
            {'.',  vertex::DOT},
            {',',  vertex::SIGN_END},
            {'[',  vertex::SIGN_END},
            {']',  vertex::SIGN_END},
            {'*',  vertex::OPERATOR_END},
            {'/',  vertex::SLASH},
            {'^',  vertex::OPERATOR_END},
            {' ',  vertex::ZERO},
            {'\n', vertex::ZERO},
    };
    edges[vertex::STRING] = {
            {'"', vertex::END_STRING}
    };
    edges[vertex::INT] = {
            {'.', vertex::FLOAT},
            {'f', vertex::FLOAT_END}
    };
    edges[vertex::PLUSMINUS] = {
            {'-', vertex::OPERATOR_END},
            {'+', vertex::OPERATOR_END},
            {'=', vertex::OPERATOR_END},
            {'*', vertex::OPERATOR_END},
    };
    edges[vertex::FLOAT] = {
            {'f', vertex::FLOAT_END}
    };
    edges[vertex::SLASH] = {
            {'/', vertex::COMMENT_SHORT },
            {'*', vertex::COMMENT_LONG }
    };
    for (char i = '0'; i <= '9'; ++i) {
        edges[vertex::ZERO][i] = vertex::INT;
        edges[vertex::DOT][i] = vertex::FLOAT;
        edges[vertex::INT][i] = vertex::INT;
        edges[vertex::FLOAT][i] = vertex::FLOAT;
        edges[vertex::ID_END][i] = vertex::ID_END;
    }
    set<char> names = {'_', '$'};

    for (char i = 'a'; i <= 'z'; ++i) {
        names.insert(i);
    }
    for (char i = 'A'; i <= 'Z'; ++i) {
        names.insert(i);
    }
    for(char c : names) {
        edges[vertex::ZERO][c] = vertex::ID_END;
        edges[vertex::ID_END][c] = vertex::ID_END;
    }
    terminals[vertex::END_STRING] = types::STRING_LITERAL;
    terminals[vertex::INT] = types::INT_LITERAL;
    terminals[vertex::FLOAT] = types::FLOAT_LITERAL;
    terminals[vertex::FLOAT_END] = types::FLOAT_LITERAL;
    terminals[vertex::SIGN_END] = types::SIGN;
    terminals[vertex::PLUSMINUS] = types::OPERATOR;
    terminals[vertex::OPERATOR_END] = types::OPERATOR;
    terminals[vertex::SLASH] = types::OPERATOR;
    terminals[vertex::DOT] = types::SIGN;
    terminals[vertex::ID_END] = types::ID;
    terminals[vertex::COMMENT_SHORT] = types::SPAM;
    terminals[vertex::COMMENT_END_SLASH] = types::SPAM;

}
bool possible(vertex cur, char c){
    if(cur==vertex::COMMENT_SHORT)
        return c!='\n';
    if(cur==vertex::COMMENT_LONG or cur==vertex::STRING
       or cur==vertex::COMMENT_END_STAR ){
        return true;
    }
    return edges[cur].find(c) != edges[cur].end();
}


vertex go_ahead(vertex cur, char c){
    if(cur==vertex::COMMENT_SHORT){
        return vertex::COMMENT_SHORT;
    }
    if(cur==vertex::COMMENT_LONG)
    {
        if(c=='*')
            return vertex::COMMENT_END_STAR;
        return vertex::COMMENT_LONG;
    }
    if(cur==vertex::COMMENT_END_STAR)
    {
        if(c=='/')
            return vertex::COMMENT_END_SLASH;
        return vertex::COMMENT_LONG;
    }
    if(cur==vertex::STRING){
        if(c=='"')
            return vertex::END_STRING;
        return vertex::STRING;
    }
    return edges[cur][c];
}



vector<lexem> analysis(string s) {
    vector<lexem> ans;
    vertex curv = vertex::ZERO;
    string s2 = "";
    for (auto c : s) {
        if ( !possible(curv, c)
            and curv!=vertex::ZERO) {
            if(terminals[curv]!=types::SPAM) {
                lexem l;
                l.s = s2;
                l.t = terminals[curv];
                ans.push_back(l);
            }
            curv = vertex::ZERO;
            s2="";
        }
        if (!possible(curv, c)){
            assert(curv==vertex::ZERO);
            lexem l;
            l.s = c;
            l.t = types::OTHER;
            ans.push_back(l);
            s2="";
        }else {
            curv = go_ahead(curv, c);
            if(curv!=vertex::ZERO)
                s2 += c;

        }

    }
    return ans;


}


void run() {
    string strings = "";
    while (!cin.eof()) {
        string s;
        getline(cin, s);
        strings += s+"\n";
    }
    build();
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