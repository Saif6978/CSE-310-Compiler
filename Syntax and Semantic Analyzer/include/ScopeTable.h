#ifndef SCOPE_TABLE_H
#define SCOPE_TABLE_H

#include <iostream>
#include <string>
#include <sstream>
#include "SymbolInfo.h"

inline unsigned int SDBMHash(const std::string& str, unsigned int num_buckets) {
    unsigned int hash = 0;
    for (char c : str) {
        hash += static_cast<unsigned char>(c);
    }
    return hash % num_buckets;
}

class ScopeTable {
private:
    int number_of_buckets;
    int child_count = 0;
    std::string id;
    ScopeTable* parent_scope;
    SymbolInfo** hash_table;

public:
    ScopeTable(int number_of_buckets, ScopeTable* parent_scope = nullptr)
        : number_of_buckets(number_of_buckets),
          parent_scope(parent_scope) {
        if (parent_scope == nullptr) {
            id = "1";
        } else {
            parent_scope->child_count++;
            id = parent_scope->id + "." + std::to_string(parent_scope->child_count);
        }
        hash_table = new SymbolInfo*[number_of_buckets];
        for (int i = 0; i < number_of_buckets; i++) {
            hash_table[i] = nullptr;
        }
    }

    ~ScopeTable() {
        for (int i = 0; i < number_of_buckets; i++) {
            if (hash_table[i] != nullptr) {
                delete hash_table[i];
            }
        }
        delete[] hash_table;
        std::cout << "\tScopeTable# " << id << " removed" << std::endl;
    }

    // Disable copy semantics
    ScopeTable(const ScopeTable&) = delete;
    ScopeTable& operator=(const ScopeTable&) = delete;

    std::string getId() const {
        return id;
    }

    int bucket_index(const std::string& name) const {
        return static_cast<int>(SDBMHash(name, static_cast<unsigned int>(number_of_buckets)));
    }

    ScopeTable* getParent() const {
        return parent_scope;
    }

    SymbolInfo* Lookup(const std::string& name, bool verbose = false) {
        int bucket = bucket_index(name);
        SymbolInfo* curr = hash_table[bucket];
        int id_inChain = 0;
        while (curr != nullptr) {
            if (curr->getName() == name) {
                if (verbose) {
                    std::cout << "\t'" << name << "' found in ScopeTable# " << id
                              << " at position " << bucket + 1 << ", " << id_inChain + 1 << std::endl;
                }
                return curr;
            }
            curr = curr->getNext();
            id_inChain++;
        }
        return nullptr;
    }

    bool Insert(const std::string& name, const std::string& type, bool verbose = false) {
        int bucket = bucket_index(name);
        SymbolInfo* curr = hash_table[bucket];
        int chain_id = 0;
        while (curr != nullptr) {
            if (curr->getName() == name) {
                if (verbose) {
                    std::cout << "< " << name << " : " << curr->getType() << " > already exists in ScopeTable# "
                              << id << " at position " << bucket << ", " << chain_id << std::endl;
                }
                return false;
            }
            curr = curr->getNext();
            chain_id++;
        }

        SymbolInfo* new_symbol = new SymbolInfo(name, type, nullptr);
        if (hash_table[bucket] == nullptr) {
            hash_table[bucket] = new_symbol;
        } else {
            SymbolInfo* tail = hash_table[bucket];
            while (tail->getNext() != nullptr) {
                tail = tail->getNext();
            }
            tail->setNext(new_symbol);
        }
        return true;
    }

    bool Remove(const std::string& name, bool verbose = false) {
        SymbolInfo* existing = Lookup(name);
        if (existing != nullptr) {
            int index = bucket_index(name);
            SymbolInfo* curr = hash_table[index];
            SymbolInfo* temp = nullptr;
            int chain_id = 0;

            if (curr == existing) {
                hash_table[index] = curr->getNext();
                temp = curr;
            } else {
                chain_id = 1;
                while (curr->getNext() != existing) {
                    curr = curr->getNext();
                    chain_id++;
                }
                temp = curr->getNext();
                curr->setNext(curr->getNext()->getNext());
            }

            temp->setNext(nullptr);
            delete temp;

            if (verbose) {
                std::cout << "\tDeleted '" << name << "' from ScopeTable# " << id
                          << " at position " << index + 1 << ", " << chain_id + 1 << std::endl;
            }
            return true;
        }
        return false;
    }

    void print(std::ostream &out = std::cout) const {
        out << "ScopeTable # " << this->id << std::endl;
        for (int i = 0; i < number_of_buckets; i++) {
            if (hash_table[i] == nullptr) continue;
            out << " " << i << " --> ";
            SymbolInfo* curr = hash_table[i];
            while (curr != nullptr) {
                out << "< " << curr->getName() << " , ID >";
                if (!curr->getIsFunction()) {
                    out << " ";
                }
                curr = curr->getNext();
            }
            out << std::endl;
        }
    }
};

#endif // SCOPE_TABLE_H
