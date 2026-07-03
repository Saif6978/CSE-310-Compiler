#ifndef SCOPE_TABLE_H
#define SCOPE_TABLE_H

#include<iostream>
#include<string>
#include<sstream>
#include "2205119_symbol_info.h"

using namespace std;

unsigned int SDBMHash ( string str , unsigned int num_buckets ) {
    unsigned int hash = 0;
    unsigned int len = str . length () ;
    for ( unsigned int i = 0; i < len ; i ++) {
        hash = (( str [ i ]) + ( hash << 6) + ( hash << 16) - hash ) % num_buckets ;
    }
    return hash ;
}

class ScopeTable{
    static int scope_count;
    int number_of_buckets;
    string id;//unique id
    ScopeTable* parent_scope;//lookup
    SymbolInfo** hash_table;//hash->index->symbols linked list
    public:
    ScopeTable(int number_of_buckets,ScopeTable* parent_scope = nullptr){
        this->id = to_string(++scope_count);
        this->parent_scope = parent_scope;
        this->number_of_buckets=number_of_buckets;
        hash_table=new SymbolInfo*[number_of_buckets];
        for(int i=0;i<number_of_buckets;i++){
            hash_table[i]=nullptr;
        }
    }
    ~ScopeTable(){
        for(int i=0;i<number_of_buckets;i++){
            if(hash_table[i] !=nullptr)delete hash_table[i];
        }
        delete[] hash_table;
        cout << "\tScopeTable# " << id << " removed" << endl;
    }

    string getId(){
        return id;
    }

    int bucket_index(string name){
        return SDBMHash(name,number_of_buckets);
    }

    ScopeTable* getParent(){
        return parent_scope;
    }

    SymbolInfo* Lookup(string name,bool verbose=false){
        int bucket=bucket_index(name);
        SymbolInfo* curr = hash_table[bucket];
        int id_inChain=0;
        while(curr !=nullptr){
            if(curr->getName()==name){
                if(verbose) cout<<"\t'"<<name<<"' found in ScopeTable# "<<id <<" at position "<<bucket+1<<", "<<id_inChain+1<<endl;
                return curr;
            }
            curr = curr->getNext();
            id_inChain++;
        }
        return nullptr;
    }



    bool Insert(string name,string type,bool verbose=false){
        SymbolInfo* existing = Lookup(name);
        if(existing==nullptr){
            int bucket = bucket_index(name); 
            SymbolInfo* new_symbol = new SymbolInfo(name , type , nullptr);
            SymbolInfo* curr = hash_table[bucket];
            int chain_id=0;
            if(curr==nullptr){
                hash_table[bucket] = new_symbol;
            }
            else{
                chain_id=1;
                while(curr->getNext()!=nullptr){
                    curr=curr->getNext();
                    chain_id++;
                }
                curr->setNext(new_symbol);
                
            }
            if(verbose)cout<<"\tInserted in ScopeTable# "<<id<<" at position "<<bucket+1<<", "<<chain_id+1<<endl;
            return true;
        }
        else{
            return false;
        }
    }

    bool Remove(string name,bool verbose = false){
        SymbolInfo* existing=Lookup(name);
        if(existing !=nullptr){
            int index=bucket_index(name);
            SymbolInfo* curr = hash_table[index];
            SymbolInfo* temp;
            int chain_id=0;
            if(curr==existing){
                hash_table[index]=curr->getNext();
                temp=curr;
            }
            else{
                chain_id=1;
                while(curr->getNext()!=existing){
                    curr=curr->getNext();
                    chain_id++;
                }
                temp=curr->getNext();
                curr->setNext(curr->getNext()->getNext());
            }
            temp->setNext(nullptr);
            delete temp;
            if(verbose)cout << "\tDeleted '"<<name << "' from ScopeTable# "<<id<<" at position "<< index+1 << ", " << chain_id+1<<endl;
            return true;
        }
        else{
            return false;
        }
    }

    void print(int depth=0){
        string indent(depth+1,'\t');
        cout <<indent<<"ScopeTable# "<<this->id<<endl;
        for(int i=0;i<number_of_buckets;i++){
            cout << indent;
            cout << i+1 << "--> ";
            SymbolInfo* curr = hash_table[i];
            while(curr!=nullptr){
                string name=curr->getName();
                string type=curr->getType();
                istringstream typeStream(type);
                string first;
                typeStream>>first;
                if(first=="FUNCTION"){
                    string returnType;
                    typeStream>>returnType;
                    string params="";
                    string tok;
                    bool firstParam=true;
                    while(typeStream>>tok){
                        if(!firstParam)params+=",";
                        params+=tok;
                        firstParam=false;
                    }
                    cout << "<"+name+",FUNCTION,"+returnType+"<==("+params+")> ";
                }
                else if(first=="STRUCT" || first=="UNION"){
                    string pairs="";
                    bool firstPair=true;
                    string t,n;
                    while(typeStream>>t){
                        typeStream>>n;
                        if(!firstPair){
                            pairs+=",";
                        }
                        pairs+="("+t+","+n+")";
                        firstPair=false;
                    }
                    cout << "<"+name+","+first+",{"+pairs+"}> ";
                }
                else{
                    cout << "<"+name+","+type+"> ";
                }
                curr=curr->getNext();
            }
            cout << endl;
        }
    }

};

#endif