#include "2205119_optimizer.h"
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

bool Optimizer::isRegister(string s){
    return s=="eax" ||
            s=="ebx" ||
            s=="ecx" ||
            s=="edx" ||
            s=="esp" ||
            s=="ebp";
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

bool Optimizer::optimizePushPop(vector<string> &lines){
    bool changed=false;
    vector<string> result;
    for(int i=0;i<lines.size();i++){
        if(i+1<lines.size() && isPushPopPair(lines[i],lines[i+1])){
            i++;
            changed=true;
            continue;
        }
        result.push_back(lines[i]);
    }
    lines=result;
    return changed;
}

string Optimizer::stripComment(string line){
    size_t pos = line.find(';');
    if(pos != string::npos){
        line  = line.substr(0,pos);
    }
    return trim(line);
}

bool Optimizer::parseMov(string line, string &dest , string &src){
    line  = stripComment(line);
    transform(line.begin() , line.end() , line.begin(),::tolower);
    if(line.find("mov ") != 0)return false;
    size_t comma = line.find(',');
    if(comma == string::npos){
        return false;
    }
    dest = trim(line.substr(4,comma-4));
    src = trim(line.substr(comma+1));
    return true;
}

bool Optimizer::optimizeArithmetic(vector<string> &lines){
    bool changed=false;
    vector<string> result;
    for(int i=0;i<lines.size();i++){
        string line=stripComment(lines[i]);
        string lower=line;
        transform(lower.begin(),lower.end(),lower.begin(),::tolower);
        // ADD EAX,0
        // SUB EAX,0
        // IMUL EAX,1
        if(lower.find("add ")==0 ||lower.find("sub ")==0){
            size_t comma=line.find(',');
            if(comma!=string::npos){
                string value=trim(line.substr(comma+1));
                transform(value.begin(), value.end(), value.begin(), ::tolower);
                if(value=="0"){
                    changed=true;
                    continue;
                }
            }
        }
        if(lower.find("imul ")==0){
            size_t comma=line.find(',');
            if(comma!=string::npos){
                string value=trim(line.substr(comma+1));
                transform(value.begin(), value.end(), value.begin(), ::tolower);
                if(value=="1"){
                    changed=true;
                    continue;
                }
            }
        }
        result.push_back(lines[i]);
    }
    lines=result;
    return changed;
}

bool Optimizer::optimizeMov(vector<string> &lines){
    bool changed=false;
    vector<string> result;
    for(int i=0;i<lines.size();i++){
        string dest1,src1;
        if(parseMov(lines[i],dest1,src1)){
            // MOV EAX,EAX
            if(dest1==src1){
                changed=true;
                continue;
            }
            if(i+1<lines.size()){
                string dest2,src2;
                if(parseMov(lines[i+1],dest2,src2)){
                    // MOV EAX,5
                    // MOV EAX,5
                    if(dest1==dest2 && src1==src2){
                        result.push_back(lines[i]);
                        i++;
                        changed=true;
                        continue;
                    }
                    // MOV EAX,EBX
                    // MOV EBX,EAX
                    if(dest1==src2 && src1==dest2){
                        result.push_back(lines[i]);
                        i++;
                        changed=true;
                        continue;
                    }
                }
            }
        }
        result.push_back(lines[i]);
    }
    lines=result;
    return changed;
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
    bool changed = true;
    int pass= 0;
    while(changed && pass<20){
        changed = false;
        if(optimizeMov(codeLines))changed = true;
        if(optimizeArithmetic(codeLines))changed = true;
        if(optimizePushPop(codeLines))changed = true;
        pass++;
    }
    writeFile();
}