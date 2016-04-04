#include <iostream>
#include <bits/stdc++.h>

using namespace std;
enum types {
    KEYWORD, //0
    INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, BOOL_LITERAL, // 1 2 3 4
    OPERATOR, ID, SIGN, // 5 6 7
    OTHER, // 8
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

set<string> keywords = {
        "for", "do", "while", "int", "boll", "double", "main", "else", "to", "downto", "cinout"
};

set<string> bools_literal = {
        "true", "false"
};

lexem c;

vector <lexem> lexems;

void atom();
void expression();
void operators();
void specatom();

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
            {'|',  vertex::OPERATOR_END},
            {'&',  vertex::OPERATOR_END},
            {'%',  vertex::OPERATOR_END},
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



vector<lexem> lexicalAnalysis(string s) {
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
                if (l.t == ID){
                    if (keywords.find(l.s) != keywords.end()){
                        l.t = KEYWORD;
                    }
                    else if (bools_literal.find(l.s) != bools_literal.end()){
                        l.t = BOOL_LITERAL;
                    }
                };
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

int lexem_number = 0;

void read(){
    c = lexems[lexem_number];
    lexem_number++;
}

void run() {
    string strings = "";
    while (!cin.eof()) {
        string s;
        getline(cin, s);
        strings += s+"\n";
    }
    build();
    lexems = lexicalAnalysis(strings);


    for (lexem l : lexems) {
        cout << l.t << " " << l.s << endl;
    }

}

void error(){
    cout << "Syntax error";
}

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
        read();
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
        if (c.t == ID || c.s == "(" || c.t == FLOAT_LITERAL || c.t == INT_LITERAL || BOOL_LITERAL || c.s == "!" ){
            expression();
            if (c.s != ";"){
                error();
            }
            else{
                read();
                if (c.t == ID || c.s == "(" || c.t == FLOAT_LITERAL || c.t == INT_LITERAL || BOOL_LITERAL || c.s == "!"){
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
        sostoperators();
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
    else if (c.t == ID || c.s == "(" || c.t == FLOAT_LITERAL || c.t == INT_LITERAL || BOOL_LITERAL || c.s == "!"){
        expression();
    }
    else{
        error();
    }
}

void program(){
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
                operators();
            }
        }
    }
}



int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    run();
    return 0;
}
