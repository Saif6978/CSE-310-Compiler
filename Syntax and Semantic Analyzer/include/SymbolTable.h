#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ScopeTable.h"

class SymbolTable {
private:
    int number_of_buckets;
    ScopeTable* current;

public:
    SymbolTable(int n, bool verbose = false)
        : number_of_buckets(n),
          current(nullptr) {
        enter_scope(verbose);
    }

    ~SymbolTable() {
        while (current != nullptr) {
            ScopeTable* temp = current;
            current = current->getParent();
            delete temp;
        }
    }

    // Disable copy semantics
    SymbolTable(const SymbolTable&) = delete;
    SymbolTable& operator=(const SymbolTable&) = delete;

    ScopeTable* getCurrent() const {
        return current;
    }

    void enter_scope(bool verbose = false) {
        ScopeTable* new_scope = new ScopeTable(number_of_buckets, current);
        current = new_scope;
        if (verbose) {
            std::cout << "\tScopeTable# " << current->getId() << " created" << std::endl;
        }
    }

    void exit_scope() {
        if (current == nullptr || current->getParent() == nullptr) {
            return;
        }
        ScopeTable* temp = current;
        current = current->getParent();
        delete temp;
    }

    bool insert_current_scope(const std::string& name, const std::string& type, bool verbose = false) {
        if (current == nullptr) return false;
        return current->Insert(name, type, verbose);
    }

    bool remove_current_scope(const std::string& name, bool verbose = false) {
        if (current == nullptr) return false;
        bool flag = current->Remove(name, verbose);
        if (!flag && verbose) {
            std::cout << "\tNot found in the current ScopeTable" << std::endl;
        }
        return flag;
    }

    SymbolInfo* lookup(const std::string& name, bool verbose = false) {
        ScopeTable* curr = current;
        while (curr != nullptr) {
            SymbolInfo* existing = curr->Lookup(name, verbose);
            if (existing != nullptr) {
                return existing;
            }
            curr = curr->getParent();
        }
        if (verbose) {
            std::cout << "\t'" << name << "' not found in any of the ScopeTables" << std::endl;
        }
        return nullptr;
    }

    void print_this_table(std::ostream &out = std::cout) const {
        if (current != nullptr) {
            current->print(out);
        }
    }

    void print_all_tables(std::ostream &out = std::cout) const {
        ScopeTable* curr = current;
        while (curr != nullptr) {
            out << std::endl << std::endl;
            curr->print(out);
            out << std::endl;
            curr = curr->getParent();
        }
        out << std::endl;
    }
};

#endif // SYMBOL_TABLE_H
