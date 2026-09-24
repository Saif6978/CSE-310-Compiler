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

    // Code generation attributes
    bool isGlobal;
    bool isParameter;
    int stackOffset;

    // Semantic and language attributes
    bool isArray;
    int arraySize;
    bool isFunction;
    std::string returnType;
    std::vector<std::string> parameterTypes;
    std::vector<std::string> parameterNames;
    bool isDeclared;
    bool isDefined;

public:
    SymbolInfo(const std::string& name, const std::string& type, SymbolInfo* next = nullptr)
        : name(name),
          type(type),
          next(next),
          isGlobal(false),
          isParameter(false),
          stackOffset(0),
          isArray(false),
          arraySize(0),
          isFunction(false),
          returnType(""),
          isDeclared(false),
          isDefined(false) {}

    // Iterative destructor preventing stack overflow
    ~SymbolInfo() {
        SymbolInfo* curr = next;
        while (curr != nullptr) {
            SymbolInfo* temp = curr->next;
            curr->next = nullptr;
            delete curr;
            curr = temp;
        }
    }

    // Disable copy semantics to prevent double frees
    SymbolInfo(const SymbolInfo&) = delete;
    SymbolInfo& operator=(const SymbolInfo&) = delete;

    void clearParameters() {
        parameterNames.clear();
        parameterTypes.clear();
    }

    void setName(const std::string& name) { this->name = name; }
    void setType(const std::string& type) { this->type = type; }
    void setNext(SymbolInfo* next) { this->next = next; }

    std::string getName() const { return name; }
    std::string getType() const { return type; }
    SymbolInfo* getNext() const { return next; }

    // ICG attributes
    void setGlobal(bool value) { isGlobal = value; }
    bool getIsGlobal() const { return isGlobal; }

    void setParameter(bool value) { isParameter = value; }
    bool getIsParameter() const { return isParameter; }

    void setStackOffset(int offset) { stackOffset = offset; }
    int getStackOffset() const { return stackOffset; }

    // Semantic attributes
    void setArray(bool value) { isArray = value; }
    bool getIsArray() const { return isArray; }

    void setArraySize(int size) { arraySize = size; }
    int getArraySize() const { return arraySize; }

    void setFunction(bool value) { isFunction = value; }
    bool getIsFunction() const { return isFunction; }

    void setReturnType(const std::string& type) { returnType = type; }
    std::string getReturnType() const { return returnType; }

    void addParameterType(const std::string& type) { parameterTypes.push_back(type); }
    void addParameterName(const std::string& name) { parameterNames.push_back(name); }

    std::vector<std::string>& getParameterTypes() { return parameterTypes; }
    const std::vector<std::string>& getParameterTypes() const { return parameterTypes; }

    std::vector<std::string>& getParameterNames() { return parameterNames; }
    const std::vector<std::string>& getParameterNames() const { return parameterNames; }

    void setDeclared(bool value) { isDeclared = value; }
    bool getIsDeclared() const { return isDeclared; }

    void setDefined(bool value) { isDefined = value; }
    bool getIsDefined() const { return isDefined; }
};

#endif // SYMBOL_INFO_H
