#ifndef SYMBOL_INFO_H
#define SYMBOL_INFO_H

#include<iostream>
#include<string>
#include<vector>
using namespace std;

class SymbolInfo{
    string name;
    string type;
    SymbolInfo* next;
    // Code generation information

    bool isGlobal;
    bool isParameter;

    int stackOffset;

    // Offline 3 information

    // Variable information
    bool isArray;
    int arraySize;

    // Function information
    bool isFunction;
    string returnType;
    vector<string> parameterTypes;
    vector<string> parameterNames;

    // Function declaration
    bool isDeclared;
    bool isDefined;


    public:
    SymbolInfo(string name,string type,SymbolInfo* next=nullptr){
        this->name=name;
        this->type=type;
        this->next=next;

        isArray=false;
        arraySize=0;

        isFunction=false;
        returnType="";
        isParameter = false;

        isDeclared=false;
        isDefined=false;
        isGlobal=false;
        stackOffset=0;
    }
    ~SymbolInfo(){
        if(next!=nullptr)delete next;
    }
    void clearParameters(){
        parameterNames.clear();
        parameterTypes.clear();
    }
    void setName(string name){
        this->name=name;
    }
    void setType(string type){
        this->type=type;
    }
    void setNext(SymbolInfo* next){
        this->next=next;
    }
    string getName(){
        return name;
    }
    string getType(){
        return type;
    }
    SymbolInfo* getNext(){
        return next;
    }
    // Array info
    void setArray(bool value){
        isArray=value;
    }
    bool getIsArray(){
        return isArray;
    }
    void setArraySize(int size) {
        arraySize = size;
    }
    int getArraySize() {
        return arraySize;
    }
    // Function info
    void setFunction(bool value) {
        isFunction = value;
    }
    bool getIsFunction() {
        return isFunction;
    }
    void setReturnType(string type) {
        returnType = type;
    }
    string getReturnType() {
        return returnType;
    }
    void setParameter(bool x){
        isParameter=x;
    }
    bool getIsParameter(){
        return isParameter;
    }
    void addParameterType(string type) {
        parameterTypes.push_back(type);
    }
    void addParameterName(string name) {
        parameterNames.push_back(name);
    }
    vector<string>& getParameterTypes() {
        return parameterTypes;
    }
    vector<string>& getParameterNames() {
        return parameterNames;
    }
    // Declaration
    void setDeclared(bool value) {
        isDeclared = value;
    }
    bool getIsDeclared() {
        return isDeclared;
    }
    void setDefined(bool value) {
        isDefined = value;
    }
    bool getIsDefined() {
        return isDefined;
    }
    void setGlobal(bool value){
        isGlobal=value;
    }
    bool getIsGlobal(){
        return isGlobal;
    }
    void setStackOffset(int offset){
        stackOffset=offset;
    }
    int getStackOffset(){
        return stackOffset;
    }
    void show(){//debugg
        cout<<endl;
		cout<<"Name = "<<name<<endl;
		cout<<"Type = "<<type<<endl;
		cout<<endl;
    }
};

#endif