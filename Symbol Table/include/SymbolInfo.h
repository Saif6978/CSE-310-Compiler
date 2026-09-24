#ifndef SYMBOL_INFO_H
#define SYMBOL_INFO_H

#include <iostream>
#include <string>

using namespace std;

class SymbolInfo {
private:
    string name;
    string type;
    SymbolInfo* next;

public:
    SymbolInfo(const string& name, const string& type, SymbolInfo* next = nullptr) {
        this->name = name;
        this->type = type;
        this->next = next;
    }

    ~SymbolInfo() {
        // Iteratively deallocate collision chain to prevent stack overflow on deep chains
        SymbolInfo* curr = next;
        while (curr != nullptr) {
            SymbolInfo* nxt = curr->next;
            curr->next = nullptr;
            delete curr;
            curr = nxt;
        }
    }

    // Disable copy constructor and copy assignment operator to prevent double-free
    SymbolInfo(const SymbolInfo&) = delete;
    SymbolInfo& operator=(const SymbolInfo&) = delete;

    void setName(const string& name) {
        this->name = name;
    }

    void setType(const string& type) {
        this->type = type;
    }

    void setNext(SymbolInfo* next) {
        this->next = next;
    }

    string getName() const {
        return name;
    }

    string getType() const {
        return type;
    }

    SymbolInfo* getNext() const {
        return next;
    }

    void show() const {
        cout << endl;
        cout << "Name = " << name << endl;
        cout << "Type = " << type << endl;
        cout << endl;
    }
};

#endif // SYMBOL_INFO_H
