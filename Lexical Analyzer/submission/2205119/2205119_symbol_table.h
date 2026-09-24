#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "2205119_scope_table.h"

class SymbolTable {
private:
    int number_of_buckets;
    ScopeTable* current;

public:
    SymbolTable(int n, bool verbose = false) {
        this->number_of_buckets = n;
        this->current = nullptr;
        enter_scope(verbose);
    }

    ~SymbolTable() {
        while (current != nullptr) {
            ScopeTable* temp = current;
            current = current->getParent();
            delete temp;
        }
    }

    // Disable copy constructor and copy assignment operator
    SymbolTable(const SymbolTable&) = delete;
    SymbolTable& operator=(const SymbolTable&) = delete;

    ScopeTable* getCurrent() const {
        return current;
    }

    void enter_scope(bool verbose = false) {
        ScopeTable* new_scope = new ScopeTable(number_of_buckets, current);
        current = new_scope;
        if (verbose) {
            cout << "\tScopeTable# " << current->getId() << " created" << endl;
        }
    }

    void EnterScope(bool verbose = false) {
        enter_scope(verbose);
    }

    bool exit_scope() {
        if (current == nullptr || current->getParent() == nullptr) {
            return false;
        }
        ScopeTable* temp = current;
        current = current->getParent();
        delete temp;
        return true;
    }

    bool ExitScope() {
        return exit_scope();
    }

    bool insert_current_scope(const string& name, const string& type, bool verbose = false) {
        if (current == nullptr) return false;
        return current->Insert(name, type, verbose);
    }

    bool Insert(const string& name, const string& type, bool verbose = false) {
        return insert_current_scope(name, type, verbose);
    }

    bool Insert(SymbolInfo* symbol, bool verbose = false) {
        if (!symbol) return false;
        return insert_current_scope(symbol->getName(), symbol->getType(), verbose);
    }

    bool remove_current_scope(const string& name, bool verbose = false) {
        if (current == nullptr) return false;
        bool flag = current->Remove(name, verbose);
        if (!flag && verbose) {
            cout << "\tNot found in the current ScopeTable" << endl;
        }
        return flag;
    }

    bool Remove(const string& name, bool verbose = false) {
        return remove_current_scope(name, verbose);
    }

    bool Delete(const string& name, bool verbose = false) {
        return remove_current_scope(name, verbose);
    }

    SymbolInfo* lookup(const string& name, bool verbose = false) {
        ScopeTable* curr = current;
        while (curr != nullptr) {
            SymbolInfo* existing = curr->Lookup(name, verbose);
            if (existing != nullptr) {
                return existing;
            }
            curr = curr->getParent();
        }
        if (verbose) {
            cout << "\t'" << name << "' not found in any of the ScopeTables" << endl;
        }
        return nullptr;
    }

    SymbolInfo* Lookup(const string& name, bool verbose = false) {
        return lookup(name, verbose);
    }

    SymbolInfo* LookUp(const string& name, bool verbose = false) {
        return lookup(name, verbose);
    }

    void print_this_table() {
        if (current != nullptr) {
            current->print(0);
        }
    }

    void PrintCurrentScopeTable() {
        print_this_table();
    }

    void print_all_tables() {
        ScopeTable* curr = current;
        int depth = 0;
        while (curr != nullptr) {
            curr->print(depth);
            depth++;
            curr = curr->getParent();
        }
    }

    void PrintAllScopeTable() {
        print_all_tables();
    }
};

#endif // SYMBOL_TABLE_H
