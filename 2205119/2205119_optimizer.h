#ifndef OPTIMIZER_2205119_H
#define OPTIMIZER_2205119_H

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
    //bool isRedundantMove(string line);
    bool isEmptyJump(string jumpLine, string labelLine);
    bool parseMov(string line , string &dest , string &src);
    bool optimizeMov(vector<string> &lines);
    bool optimizeArithmetic(vector<string> &lines);
    string stripComment(string lines);
    bool optimizePushPop(vector<string> &lines);
    bool isRegister(string s);
public:
    Optimizer(string input, string output);
    void optimize();
};
#endif