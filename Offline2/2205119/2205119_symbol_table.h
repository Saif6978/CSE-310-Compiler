#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "2205119_scope_table.h"

class SymbolTable{
    int number_of_buckets;
    ScopeTable* current;
    public:
    SymbolTable(int n,bool verbose=false){
        this->number_of_buckets=n;
        current=nullptr;
        enter_scope(verbose);
    }
    ~SymbolTable(){
        while(current!=nullptr){
            ScopeTable* temp=current;
            current=current->getParent();
            delete temp;
        }
    }
    ScopeTable* getCurrent(){
        return current;
    }
    void enter_scope(bool verbose=false){
        ScopeTable* new_scope=new ScopeTable(number_of_buckets,current);
        current=new_scope;
        if(verbose)cout<<"\tScopeTable# "<<current->getId()<<" created"<<endl;

    }
    void exit_scope(){
        if(current->getParent()==nullptr){
            return;
        }
        ScopeTable* temp=current;
        current=current->getParent();
        delete temp;
    }
    bool insert_current_scope(string name ,string type,bool verbose=false){
        bool flag=current->Insert(name,type,verbose);
        return flag;
    }

    bool remove_current_scope(string name,bool verbose=false){
        bool flag = current->Remove(name,verbose);
        if(flag==false && verbose){
            cout<<"\tNot found in the current ScopeTable"<<endl;
        }
        return flag;
    }
    SymbolInfo* lookup(string name,bool verbose =false){
        ScopeTable* curr = current;
        while(curr!=nullptr){
            SymbolInfo* existing=curr->Lookup(name,verbose);
            if(existing!=nullptr){
                return existing;
            }
            curr=curr->getParent();
        }
        if(verbose)cout<<"\t'" << name<<"' not found in any of the ScopeTables"<<endl;
        return nullptr;
    }

    void print_this_table(){
        current->print(0);
    }
    void print_all_tables(){
        ScopeTable* curr=current;
        int depth=0;
        while(curr!=nullptr){
            curr->print(depth);
            depth++;
            curr=curr->getParent();
        }
    }
};

#endif