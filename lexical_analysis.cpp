//
// Created by oleg on 4/4/16.
//

#include "lexem_types.cpp"
#include "lexical_analysis.h"


using namespace std;
map<vertex, map<char, vertex>> edges;
map<vertex, lexem_types> terminals;


set<string> keywords = {
        "for", "do", "while", "int", "boll", "double", "main", "else", "to", "downto", "cinout"
};

set<string> bools_literal = {
        "true", "false"
};

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
    int curl = 1;
    for (auto c : s) {
        if(c=='\n')
            curl+=1;
        if ( !possible(curv, c)
             and curv!=vertex::ZERO) {
            if(terminals[curv]!=lexem_types::SPAM) {
                lexem l;
                l.s = s2;
                l.t = terminals[curv];
                if (l.t == lexem_types::ID){
                    if (keywords.find(l.s) != keywords.end()){
                        l.t = lexem_types::KEYWORD;
                    }
                    else if (bools_literal.find(l.s) != bools_literal.end()){
                        l.t = lexem_types::BOOL_LITERAL;
                    }
                };
                l.line = curl;
                ans.push_back(l);
            }
            curv = vertex::ZERO;
            s2="";
        }
        if (!possible(curv, c)){
            lexem l;
            l.s = c;
            l.t = lexem_types::OTHER;
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

bool possible(vertex cur, char c){
    if(cur==vertex::COMMENT_SHORT)
        return c!='\n';
    if(cur==vertex::COMMENT_LONG or cur==vertex::STRING
       or cur==vertex::COMMENT_END_STAR ){
        return true;
    }
    return edges[cur].find(c) != edges[cur].end();
}

void build() {

    edges[vertex::ZERO] = {
            {'\"', vertex::STRING},
            {'-',  vertex::PLUSMINUS},
            {'+',  vertex::PLUSMINUS},
            {'=',  vertex::PLUSMINUS},
            {'*',  vertex::PLUSMINUS},
            {'<',  vertex::PLUSMINUS},
            {'>',  vertex::PLUSMINUS},
            {'{',  vertex::SIGN_END},
            {'}',  vertex::SIGN_END},
            {'(',  vertex::SIGN_END},
            {')',  vertex::SIGN_END},
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
            {'<', vertex::OPERATOR_END },
            {'>', vertex::OPERATOR_END }
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
    terminals[vertex::END_STRING] = lexem_types::STRING_LITERAL;
    terminals[vertex::INT] = lexem_types::INT_LITERAL;
    terminals[vertex::FLOAT] = lexem_types::FLOAT_LITERAL;
    terminals[vertex::FLOAT_END] = lexem_types::FLOAT_LITERAL;
    terminals[vertex::SIGN_END] = lexem_types::SIGN;
    terminals[vertex::PLUSMINUS] = lexem_types::OPERATOR;
    terminals[vertex::OPERATOR_END] = lexem_types::OPERATOR;
    terminals[vertex::SLASH] = lexem_types::OPERATOR;
    terminals[vertex::DOT] = lexem_types::SIGN;
    terminals[vertex::ID_END] = lexem_types::ID;
    terminals[vertex::COMMENT_SHORT] = lexem_types::SPAM;
    terminals[vertex::COMMENT_END_SLASH] = lexem_types::SPAM;

}