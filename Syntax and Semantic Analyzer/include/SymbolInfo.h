#ifndef SYMBOL_INFO_H
#define SYMBOL_INFO_H

#include <iostream>
#include <string>
#include <vector>

class SymbolInfo {
private:
    std::string name;
    std::string type;
    SymbolInfo* next;

    // Offline 3: Semantic analysis metadata
    // Variable information
    bool isArray;
    int arraySize;

    // Function information
    bool isFunction;
    std::string returnType;
    std::vector<std::string> parameterTypes;
    std::vector<std::string> parameterNames;

    // Function declaration/definition flags
    bool isDeclared;
    bool isDefined;

public:
    SymbolInfo(const std::string& name, const std::string& type, SymbolInfo* next = nullptr)
        : name(name),
          type(type),
          next(next),
          isArray(false),
          arraySize(0),
          isFunction(false),
          returnType(""),
          isDeclared(false),
          isDefined(false) {}

    // Iterative destructor to prevent call stack overflow on long bucket chains
    ~SymbolInfo() {
        SymbolInfo* curr = next;
        while (curr != nullptr) {
            SymbolInfo* temp = curr->next;
            curr->next = nullptr;
            delete curr;
            curr = temp;
        }
    }

    // Disable copy semantics to prevent double frees of pointer chains
    SymbolInfo(const SymbolInfo&) = delete;
    SymbolInfo& operator=(const SymbolInfo&) = delete;

    void clearParameters() {
        parameterNames.clear();
        parameterTypes.clear();
    }

    void setName(const std::string& name) {
        this->name = name;
    }

    void setType(const std::string& type) {
        this->type = type;
    }

    void setNext(SymbolInfo* next) {
        this->next = next;
    }

    std::string getName() const {
        return name;
    }

    std::string getType() const {
        return type;
    }

    SymbolInfo* getNext() const {
        return next;
    }

    // Array attributes
    void setArray(bool value) {
        isArray = value;
    }

    bool getIsArray() const {
        return isArray;
    }

    void setArraySize(int size) {
        arraySize = size;
    }

    int getArraySize() const {
        return arraySize;
    }

    // Function attributes
    void setFunction(bool value) {
        isFunction = value;
    }

    bool getIsFunction() const {
        return isFunction;
    }

    void setReturnType(const std::string& type) {
        returnType = type;
    }

    std::string getReturnType() const {
        return returnType;
    }

    void addParameterType(const std::string& type) {
        parameterTypes.push_back(type);
    }

    void addParameterName(const std::string& name) {
        parameterNames.push_back(name);
    }

    std::vector<std::string>& getParameterTypes() {
        return parameterTypes;
    }

    const std::vector<std::string>& getParameterTypes() const {
        return parameterTypes;
    }

    std::vector<std::string>& getParameterNames() {
        return parameterNames;
    }

    const std::vector<std::string>& getParameterNames() const {
        return parameterNames;
    }

    // Declaration / Definition flags
    void setDeclared(bool value) {
        isDeclared = value;
    }

    bool getIsDeclared() const {
        return isDeclared;
    }

    void setDefined(bool value) {
        isDefined = value;
    }

    bool getIsDefined() const {
        return isDefined;
    }

    void show() const {
        std::cout << "\nName = " << name << "\nType = " << type << "\n" << std::endl;
    }
};

#endif // SYMBOL_INFO_H
