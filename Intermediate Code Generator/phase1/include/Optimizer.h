#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Optimizer{
private:
    string inputFile;
    string outputFile;
    vector<string> codeLines;
    void readFile();
    void writeFile();
    bool isPushPopPair(string a, string b);
    bool isRedundantMove(string line);
    bool isEmptyJump(string jumpLine, string labelLine);
public:
    Optimizer(string input, string output);
    void optimize();
};
#endif