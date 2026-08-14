#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "2205119_visitor.h"
#include "antlr4-runtime.h"
#include "CSubsetLexer.h"
#include "CSubsetParser.h"

using namespace antlr4;
using namespace std;
namespace fs = std::filesystem;

ofstream lexLogFile; // used by Lexer.g4's writeIntoLexLogFile()

static bool processInput(
    const string &inputPath,
    const string &logPath,
    const string &errorPath,
    const string &lexLogPath
) {
    ifstream inputFile(inputPath);
    if (!inputFile.is_open()) {
        cerr << "Error opening input file: " << inputPath << endl;
        return false;
    }

    // Lexer.g4 uses this global stream. Opening it here prevents all five
    // runs from being mixed into one root-level lexLogFile.txt.
    if (lexLogFile.is_open()) {
        lexLogFile.close();
    }
    lexLogFile.open(lexLogPath, ios::out | ios::trunc);

    ANTLRInputStream input(inputFile);
    CSubsetLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    CSubsetParser parser(&tokens);

    CSubsetParser::StartContext *tree = parser.start();

    Visitor2205119 visitor(&tokens, logPath, errorPath);
    visitor.visit(tree);

    inputFile.close();
    if (lexLogFile.is_open()) {
        lexLogFile.close();
    }

    return true;
}

int main(int argc, const char *argv[]) {
    if (argc == 2) {
        bool ok = processInput(
            argv[1],
            "log.txt",
            "error.txt",
            "lexLogFile.txt"
        );
        if (ok) {
            cout << "Parsing completed for " << argv[1] << endl;
            return 0;
        }
        return 1;
    }

    if (argc != 6) {
        cerr << "Usage:\n"
             << "  Single input: " << argv[0] << " <input_file>\n"
             << "  Five inputs : " << argv[0]
             << " <input1> <input2> <input3> <input4> <input5>" << endl;
        return 1;
    }

    fs::create_directories("output/log");
    fs::create_directories("output/error");
    fs::create_directories("output/lex");

    bool allSucceeded = true;

    for (int i = 1; i <= 5; ++i) {
        string logPath = "output/log/log" + to_string(i) + ".txt";
        string errorPath = "output/error/error" + to_string(i) + ".txt";
        string lexLogPath = "output/lex/lexLog" + to_string(i) + ".txt";

        cout << "[" << i << "/5] Parsing " << argv[i] << " ... ";

        bool ok = processInput(argv[i], logPath, errorPath, lexLogPath);
        if (ok) {
            cout << "done" << endl;
        } else {
            cout << "failed" << endl;
            allSucceeded = false;
        }
    }

    if (!allSucceeded) {
        cerr << "One or more input files could not be processed." << endl;
        return 1;
    }

    cout << "\nGenerated files:" << endl;
    cout << "  output/log/log1.txt ... log5.txt" << endl;
    cout << "  output/error/error1.txt ... error5.txt" << endl;
    cout << "  output/lex/lexLog1.txt ... lexLog5.txt" << endl;

    return 0;
}
