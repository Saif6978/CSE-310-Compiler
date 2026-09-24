#ifndef SCOPE_TABLE_H
#define SCOPE_TABLE_H

#include <iostream>
#include <string>
#include <sstream>
#include "2205119_symbol_info.h"

using namespace std;

// Inline SDBM hash function as specified in the assignment
inline unsigned int SDBMHash(const string& str, unsigned int num_buckets) {
    unsigned int hash = 0;
    for (char c : str) {
        hash = (c + (hash << 6) + (hash << 16) - hash);
    }
    return hash % num_buckets;
}

class ScopeTable {
private:
    int number_of_buckets;
    int child_count;
    string id;
    ScopeTable* parent_scope;
    SymbolInfo** hash_table;

public:
    ScopeTable(int number_of_buckets, ScopeTable* parent_scope = nullptr) {
        this->parent_scope = parent_scope;
        this->number_of_buckets = number_of_buckets;
        this->child_count = 0;
        if (parent_scope == nullptr) {
            this->id = "1";
        } else {
            parent_scope->child_count++;
            this->id = parent_scope->id + "." + to_string(parent_scope->child_count);
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
        cout << "\tScopeTable# " << id << " removed" << endl;
    }

    // Disable copy constructor and copy assignment operator
    ScopeTable(const ScopeTable&) = delete;
    ScopeTable& operator=(const ScopeTable&) = delete;

    string getId() const {
        return id;
    }

    int bucket_index(const string& name) const {
        return SDBMHash(name, number_of_buckets);
    }

    ScopeTable* getParent() const {
        return parent_scope;
    }

    SymbolInfo* Lookup(const string& name, bool verbose = false) {
        int bucket = bucket_index(name);
        SymbolInfo* curr = hash_table[bucket];
        int id_inChain = 0;
        while (curr != nullptr) {
            if (curr->getName() == name) {
                if (verbose) {
                    cout << "\t'" << name << "' found in ScopeTable# " << id << " at position " << bucket + 1 << ", " << id_inChain + 1 << endl;
                }
                return curr;
            }
            curr = curr->getNext();
            id_inChain++;
        }
        return nullptr;
    }

    SymbolInfo* LookUp(const string& name, bool verbose = false) {
        return Lookup(name, verbose);
    }

    bool Insert(const string& name, const string& type, bool verbose = false) {
        int bucket = bucket_index(name);
        SymbolInfo* curr = hash_table[bucket];
        int chain_id = 0;

        while (curr != nullptr) {
            if (curr->getName() == name) {
                if (verbose) {
                    cout << "< " << name << " : " << curr->getType() << " > already exists in ScopeTable# "
                         << id << " at position " << bucket << ", " << chain_id << endl;
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

    bool Insert(SymbolInfo* symbol, bool verbose = false) {
        if (!symbol) return false;
        return Insert(symbol->getName(), symbol->getType(), verbose);
    }

    bool Remove(const string& name, bool verbose = false) {
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
                cout << "\tDeleted '" << name << "' from ScopeTable# " << id << " at position " << index + 1 << ", " << chain_id + 1 << endl;
            }
            return true;
        } else {
            return false;
        }
    }

    bool Delete(const string& name, bool verbose = false) {
        return Remove(name, verbose);
    }

    void print(int depth = 0) const {
        (void)depth;
        cout << "ScopeTable # " << this->id << endl;
        for (int i = 0; i < number_of_buckets; i++) {
            if (hash_table[i] == nullptr) continue;

            cout << i << " --> ";
            SymbolInfo* curr = hash_table[i];
            while (curr != nullptr) {
                string name = curr->getName();
                string type = curr->getType();
                istringstream typeStream(type);
                string first;
                typeStream >> first;
                if (first == "FUNCTION") {
                    string returnType;
                    typeStream >> returnType;
                    string params = "";
                    string tok;
                    bool firstParam = true;
                    while (typeStream >> tok) {
                        if (!firstParam) params += ",";
                        params += tok;
                        firstParam = false;
                    }
                    cout << "< " + name + ": FUNCTION, " + returnType + " <==(" + params + ") > ";
                } else if (first == "STRUCT" || first == "UNION") {
                    string pairs = "";
                    bool firstPair = true;
                    string t, n;
                    while (typeStream >> t) {
                        typeStream >> n;
                        if (!firstPair) {
                            pairs += ",";
                        }
                        pairs += "(" + t + "," + n + ")";
                        firstPair = false;
                    }
                    cout << "< " + name + " : " + first + " : {" + pairs + "} >";
                } else {
                    cout << "< " + name + " : " + type + " >";
                }
                curr = curr->getNext();
            }
            cout << endl;
        }
    }

    void Print(int depth = 0) const {
        print(depth);
    }
};

#endif // SCOPE_TABLE_H
