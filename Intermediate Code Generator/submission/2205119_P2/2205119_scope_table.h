#ifndef SCOPE_TABLE_H
#define SCOPE_TABLE_H

#include<iostream>
#include<string>
#include<sstream>
#include "2205119_symbol_info.h"

using namespace std;

inline unsigned int SDBMHash ( string str , unsigned int num_buckets ) {
    unsigned int hash = 0;
    for ( char c:str) {
        hash += c;
    }
    return hash % num_buckets  ;
}

class ScopeTable{
    int number_of_buckets;
    int child_count=0;
    string id;//unique id
    ScopeTable* parent_scope;//lookup
    SymbolInfo** hash_table;//hash->index->symbols linked list
    public:
    ScopeTable(int number_of_buckets,ScopeTable* parent_scope = nullptr){
        this->parent_scope = parent_scope;
        this->number_of_buckets=number_of_buckets;
        if(parent_scope==nullptr){
            this->id ="1";
        }
        else{
            parent_scope->child_count++;
            this->id = parent_scope->id + "." +to_string(parent_scope->child_count);
        }
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

    bool Insert(string name, string type, bool verbose=false){
        int bucket = bucket_index(name);
        SymbolInfo* curr = hash_table[bucket];
        int chain_id = 0;
        while(curr != nullptr){
            if(curr->getName() == name){
                if(verbose){
                    cout << "< " << name << " : " << curr->getType() << " > already exists in ScopeTable# "
                        << id << " at position " << bucket << ", " << chain_id << endl;//changed from chain_id
                }
                return false;
            }
            curr = curr->getNext();
            chain_id++;
        }
        SymbolInfo* new_symbol = new SymbolInfo(name, type, nullptr);
        if(hash_table[bucket] == nullptr){
            hash_table[bucket] = new_symbol;
        } else {
            SymbolInfo* tail = hash_table[bucket];
            while(tail->getNext() != nullptr){
                tail = tail->getNext();
            }
            tail->setNext(new_symbol);
        }
        return true;
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

    void print(ostream &out = cout){
        out << "ScopeTable # " << this->id << endl;
        for(int i=0;i<number_of_buckets ; i++){
            if(hash_table[i] == nullptr)continue;
            out << " " << i << " --> " ;
            SymbolInfo* curr = hash_table[i];
            while(curr != nullptr){
                out << "< " << curr->getName() << " , ID >";
                if(!curr->getIsFunction()){
                    out << " ";
                }
                curr = curr->getNext();
            }
            out << endl;
        }
    }
};

#endif