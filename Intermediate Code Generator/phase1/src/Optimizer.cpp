#include "Optimizer.h"
#include <algorithm>
#include <cctype>

Optimizer::Optimizer(string input, string output){
    inputFile = input;
    outputFile = output;
}

string trim(string s){
    size_t first=s.find_first_not_of(" \t");
    if(first==string::npos)
        return "";
    size_t last=s.find_last_not_of(" \t");
    return s.substr(first,last-first+1);
}

bool isEmptyLine(string line){
    return trim(line)=="";
}

void Optimizer::readFile(){
    ifstream file(inputFile);
    if(!file.is_open()){
        cerr<<"Cannot open input asm file"<<endl;
        return;
    }
    string line;
    while(getline(file,line)){
        codeLines.push_back(line);
    }
    file.close();
}

void Optimizer::writeFile(){
    ofstream file(outputFile);
    if(!file.is_open()){
        cerr<<"Cannot create optimized asm file"<<endl;
        return;
    }
    for(string line : codeLines){
        file<<line<<endl;
    }
    file.close();
}

bool Optimizer::isPushPopPair(string a,string b){
    transform(a.begin(), a.end(), a.begin(), ::tolower);
    transform(b.begin(), b.end(), b.begin(), ::tolower);
    if(a.find("push ")==0 && b.find("pop ")==0){
        string reg1=a.substr(5);
        string reg2=b.substr(4);
        if(reg1==reg2) return true;
    }
    return false;
}

bool Optimizer::isRedundantMove(string line){
    string temp=trim(line);
    transform(temp.begin(),temp.end(),temp.begin(),::tolower);
    if(temp=="mov eax, eax" || temp=="mov eax,eax") return true;
    return false;
}

bool Optimizer::isEmptyJump(string jumpLine,string labelLine)
{
    jumpLine = trim(jumpLine);
    labelLine = trim(labelLine);
    transform(jumpLine.begin(), jumpLine.end(), jumpLine.begin(), ::tolower);
    transform(labelLine.begin(), labelLine.end(), labelLine.begin(), ::tolower);
    if(jumpLine.find("jmp ")==0){
        string label = jumpLine.substr(4);
        if(label + ":" == labelLine)
            return true;
    }
    return false;
}

void Optimizer::optimize(){
    readFile();
    vector<string> optimized;
    for(size_t i = 0; i < codeLines.size(); i++){
        // optimization 1
        if(i+1 < codeLines.size() && isPushPopPair(codeLines[i],codeLines[i+1])){
            i++;
            continue;
        }
        // optimization 2
        if(isRedundantMove(codeLines[i])){
            continue;
        }
        // optimization 3
        if(i+1 < codeLines.size() && isEmptyJump(codeLines[i],codeLines[i+1])){
            continue;
        }
        if(isEmptyLine(codeLines[i])){
            if(!optimized.empty() && isEmptyLine(optimized.back())) continue;
        }
        optimized.push_back(codeLines[i]);
    }
    codeLines = optimized;
    writeFile();
}