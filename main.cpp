#include <iostream>
#include <bits/stdc++.h>


#include "lexem_types.h"
#include "lexical_analysis.h"
#include "syntax_analysis.h"
#include "Generation.h"

using namespace std;


void run(string dir, string input_file) {
//system(("mkdir " + dir).c_str());
    ifstream ifs(input_file, std::ifstream::in);

    ofstream lexemfs(dir + "/" + input_file + ".lexems");
    ofstream logfs(dir + "/" + input_file + ".log");



    string strings = "";
    while (!ifs.eof()) {
        string s;
        getline(ifs, s);
        strings += s + "\n";
    }
    build();

    vector<lexem> lexems = lexicalAnalysis(strings);
    logfs << "Lexical done" << endl;
    for (lexem l : lexems) {
        lexemfs << l.s << endl;
    }
    SyntaxAnalysis analysis(lexems, true);
    //analysis.program();
    logfs  << "Syntax done";
    Generation gen;

    gen.lexems = lexems;
    gen.build();
    gen.dump_to_file(dir + "/" + input_file + ".poliz");
    cerr << "Rpn builded" << endl;
    gen.run();


}



int main(int argc, char *argv[]) {
    cin.sync_with_stdio(true);
    string s = "input.txt";
    if (argc > 1) {
        s = string(argv[1]);
    }
    run("out", s);

//fprintf(stderr,"OK");
    return 0;
}
