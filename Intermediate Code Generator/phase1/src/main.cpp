#include <iostream>
#include <fstream>
#include <string>

#include "AsmGenerator.h"
#include "Optimizer.h"
#include "antlr4-runtime.h"
#include "CSubsetLexer.h"
#include "CSubsetParser.h"

using namespace antlr4;
using namespace std;

ofstream lexLogFile;

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file> [asm_output] [optimized_asm_output]" << endl;
        return 1;
    }

    lexLogFile.open("lexLogFile.txt", ios::out | ios::trunc);

    string asmOutput = (argc >= 3) ? argv[2] : "2205119_code.asm";
    string optOutput = (argc >= 4) ? argv[3] : "optimizedcode.asm";

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        cerr << "Error opening input file: " << argv[1] << endl;
        return 1;
    }

    ANTLRInputStream input(inputFile);
    CSubsetLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    CSubsetParser parser(&tokens);

    CSubsetParser::StartContext *tree = parser.start();

    AsmGenerator asmGenerator(asmOutput);
    asmGenerator.visit(tree);
    asmGenerator.closeFile();

    Optimizer optimizer(asmOutput, optOutput);
    optimizer.optimize();

    inputFile.close();
    lexLogFile.close();
    cout << "Phase 1 code generation and optimization completed successfully." << endl;
    return 0;
}
