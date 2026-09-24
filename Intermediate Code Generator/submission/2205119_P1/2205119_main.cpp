#include <iostream>
#include <fstream>
#include <string>

#include "2205119_visitor.h"
#include "2205119_asm_generator.h"
#include "antlr4-runtime.h"
#include "CSubsetLexer.h"
#include "CSubsetParser.h"
#include "2205119_optimizer.h"

using namespace antlr4;
using namespace std;

ofstream lexLogFile;

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        cerr << "Error opening input file: " << argv[1] << endl;
        return 1;
    }

    lexLogFile.open("lexLogFile.txt", ios::out | ios::trunc);
    if (!lexLogFile.is_open()) {
        cerr << "Error opening lexLogFile.txt" << endl;
        return 1;
    }

    ANTLRInputStream input(inputFile);
    CSubsetLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    CSubsetParser parser(&tokens);

    CSubsetParser::StartContext *tree = parser.start();

    //Visitor2205119 visitor(&tokens);
    //visitor.visit(tree);
    AsmGenerator asmGenerator("2205119_code.asm" );
    asmGenerator.visit(tree);
    asmGenerator.closeFile();
    Optimizer optimizer("2205119_code.asm" , "optimizedcode.asm");
    optimizer.optimize();

    inputFile.close();
    lexLogFile.close();

    cout << "Parsing completed." << endl;
    return 0;
}

