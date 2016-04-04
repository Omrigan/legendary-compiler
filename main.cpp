#include <iostream>
#include <bits/stdc++.h>


#include "types.cpp"
#include "lexical_analysis.h"
#include "syntax_analysis.h"

using namespace std;






void run() {
    string strings = "";
    while (!cin.eof()) {
        string s;
        getline(cin, s);
        strings += s+"\n";
    }
    build();
    vector<lexem> lexems = lexicalAnalysis(strings);
    for (lexem l : lexems) {
        cout << l.t << " " << l.s << endl;
    }


    read();
    program(lexems);
    cout<< "Syntax done";



}









int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    run();
    return 0;
}
