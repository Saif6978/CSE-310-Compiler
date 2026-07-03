#ifndef SYMBOL_INFO_H
#define SYMBOL_INFO_H

#include<iostream>
#include<string>

using namespace std;

class SymbolInfo{
    string name;
    string type;
    SymbolInfo* next;
    public:
    SymbolInfo(string name,string type,SymbolInfo* next=nullptr){
        this->name=name;
        this->type=type;
        this->next=next;
    }
    ~SymbolInfo(){
        if(next!=nullptr)delete next;
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
    void show(){//debugg
        cout<<endl;
		cout<<"Name = "<<name<<endl;
		cout<<"Type = "<<type<<endl;
		cout<<endl;
    }
};

#endif