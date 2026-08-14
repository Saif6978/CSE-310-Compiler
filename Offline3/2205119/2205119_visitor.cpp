#include "2205119_visitor.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <any>
#include <iomanip>

using namespace std;
using namespace antlr4;

void Visitor2205119::printError(int line, const string &message) {
    string formatted = "Error at line " + to_string(line) + ": " + message;
    errorout << formatted << endl << endl;
    logout << formatted << endl << endl;
    errorCount++;
}

static string formatFloat(const string &text) {
    try {
        double value = stod(text);
        ostringstream out;
        out << fixed << setprecision(2) << value;
        return out.str();
    } catch (...) {
        return text;
    }
}

void Visitor2205119::printError(ofstream &out, int line, const string &message) {
    out << "Error at line " << line << ": " << message << endl << endl;
    errorCount++;
}

static void printLog(ofstream &out, int line, const string &rule, const string &text) {
    out << "Line " << line << ": " << rule << endl << endl;
    out << text << endl << endl;
}

static string anyToString(const any &value) {
    if (!value.has_value()) return "error";
    try {
        return any_cast<string>(value);
    } catch (...) {
        return "error";
    }
}

struct MalformedAddAssignParts {
    CSubsetParser::ExpressionContext *expression = nullptr;
    CSubsetParser::Simple_expressionContext *badSimple = nullptr;
};

static bool getMalformedAddAssign(CSubsetParser::StatementContext *statement, MalformedAddAssignParts &parts) {
    if(statement == nullptr || statement->FOR() != nullptr){
        return false;
    }
    if(statement->expression_statement().size() != 1){
        return false;
    }
    auto *es = statement->expression_statement(0);
    if(es == nullptr || es->expression() == nullptr || es->SEMICOLON() != nullptr){
        return false;
    }
    auto *expr = es->expression();
    if (expr->ASSIGNOP() == nullptr || expr->variable() == nullptr || expr->logic_expression() == nullptr) {
        return false;
    }
    auto *logic = expr->logic_expression();
    if (logic->LOGICOP() != nullptr || logic->rel_expression().empty()) {
        return false;
    }
    auto *rel = logic->rel_expression(0);
    if (rel == nullptr || rel->RELOP() != nullptr || rel->simple_expression().empty()) {
        return false;
    }
    auto *simple = rel->simple_expression(0);
    if (simple == nullptr || simple->ADDOP() == nullptr || simple->ASSIGNOP() == nullptr ||
        simple->simple_expression() == nullptr || simple->term() == nullptr) {
        return false;
    }
    parts.expression = expr;
    parts.badSimple = simple;
    return true;
}

static string ensureOneTrailingNewline(string text) {
    while (!text.empty() && text.back() == '\n') {
        text.pop_back();
    }
    text.push_back('\n');
    return text;
}

static bool isArrayType(const string &type) {
    return type.find(" array[") != string::npos;
}

static string getArrayBaseType(const string &type) {
    size_t pos = type.find(" array[");
    if (pos == string::npos) return type;
    return type.substr(0, pos);
}

static string getArraySize(const string &type) {
    size_t left = type.find(" array[");
    if (left == string::npos) return "";
    left += 7;
    size_t right = type.find("]", left);
    if (right == string::npos) return "";
    return type.substr(left, right - left);
}

static string arithmeticType(const string &left, const string &right) {
    if (left == "error" || right == "error") return "error";
    if (isArrayType(left) || isArrayType(right)) return "error";
    if (left == "float" || right == "float") return "float";
    if (left == "int" && right == "int") return "int";
    return "error";
}

static vector<string> getParameterTypes(CSubsetParser::Parameter_listContext *ctx) {
    vector<string> result;
    if (ctx == nullptr) return result;
    if (ctx->parameter_list() != nullptr) {
        vector<string> previous = getParameterTypes(ctx->parameter_list());
        result.insert(result.end(), previous.begin(), previous.end());
    }
    if (ctx->type_specifier() != nullptr) {
        result.push_back(ctx->type_specifier()->getText());
    }
    return result;
}

static vector<string> getParameterNames(CSubsetParser::Parameter_listContext *ctx) {
    vector<string> result;
    if (ctx == nullptr) return result;
    if (ctx->parameter_list() != nullptr) {
        vector<string> previous = getParameterNames(ctx->parameter_list());
        result.insert(result.end(), previous.begin(), previous.end());
    }
    if (ctx->ID() != nullptr) result.push_back(ctx->ID()->getText());
    else result.push_back("");
    return result;
}

static vector<string> getArgumentTypes(CSubsetParser::ArgumentsContext *ctx, Visitor2205119 *visitor) {
    vector<string> result;
    if (ctx == nullptr) return result;
    if (ctx->arguments() != nullptr) {
        vector<string> previous = getArgumentTypes(ctx->arguments(), visitor);
        result.insert(result.end(), previous.begin(), previous.end());
    }
    if (ctx->logic_expression() != nullptr) {
        string type = anyToString(visitor->visit(ctx->logic_expression()));
        result.push_back(type);
    }
    return result;
}

static bool isStandaloneFunctionCall(CSubsetParser::FactorContext *factor) {
    if (factor == nullptr) return false;
    auto *unary = dynamic_cast<CSubsetParser::Unary_expressionContext *>(factor->parent);
    if (unary == nullptr) return false;
    auto *term = dynamic_cast<CSubsetParser::TermContext *>(unary->parent);
    if (term == nullptr || term->MULOP() != nullptr) return false;
    auto *simple = dynamic_cast<CSubsetParser::Simple_expressionContext *>(term->parent);
    if (simple == nullptr || simple->ADDOP() != nullptr) return false;
    auto *rel = dynamic_cast<CSubsetParser::Rel_expressionContext *>(simple->parent);
    if (rel == nullptr || rel->RELOP() != nullptr) return false;
    auto *logic = dynamic_cast<CSubsetParser::Logic_expressionContext *>(rel->parent);
    if (logic == nullptr || logic->LOGICOP() != nullptr) return false;
    auto *expression = dynamic_cast<CSubsetParser::ExpressionContext *>(logic->parent);
    if (expression == nullptr) return false;
    if (expression->ASSIGNOP() != nullptr) return false;
    if (expression->getText() != factor->getText()) return false;
    auto *expressionStatement = dynamic_cast<CSubsetParser::Expression_statementContext *>(expression->parent);
    return expressionStatement != nullptr;
}

any Visitor2205119::visitStart(CSubsetParser::StartContext *ctx) {
    visit(ctx->program());
    logout << "Line " << ctx->getStart()->getLine() << ": start : program" << endl << endl;
    symbolTable.print_all_tables(logout);
    int lineCount = ctx->getStop()->getLine();
    logout << "Total lines: " << lineCount << endl;
    logout << "Total errors: " << errorCount << endl << endl;
    return ctx->getText();
}

any Visitor2205119::visitProgram(CSubsetParser::ProgramContext *ctx) {
    string text;
    string rule;
    if (ctx->program() == nullptr) {
        text = anyToString(visit(ctx->unit()));
        rule = "program : unit";
    }
    else {
        string previous = anyToString(visit(ctx->program()));
        string current = anyToString(visit(ctx->unit()));
        text = previous + current;
        rule = "program : program unit";
    }
    printLog(logout, ctx->unit()->getStart()->getLine(), rule, text);
    return text;
}

any Visitor2205119::visitUnit(CSubsetParser::UnitContext *ctx) {
    string text;
    string rule;
    if (ctx->var_declaration() != nullptr) {
        text = anyToString(visit(ctx->var_declaration()));
        rule = "unit : var_declaration";
    } else if (ctx->func_declaration() != nullptr) {
        text = anyToString(visit(ctx->func_declaration()));
        rule = "unit : func_declaration";
    } else if (ctx->func_definition() != nullptr) {
        text = anyToString(visit(ctx->func_definition()));
        rule = "unit : func_definition";
    }
    text += "\n";
    printLog(logout, ctx->getStart()->getLine(), rule, text);
    return text;
}

any Visitor2205119::visitType_specifier(CSubsetParser::Type_specifierContext *ctx) {
    string rule;
    if (ctx->INT() != nullptr) rule = "type_specifier : INT";
    else if (ctx->FLOAT() != nullptr) rule = "type_specifier : FLOAT";
    else rule = "type_specifier : VOID";
    string text = ctx->getText();
    printLog(logout, ctx->getStart()->getLine(), rule, text);
    return text;
}

any Visitor2205119::visitVar_declaration(CSubsetParser::Var_declarationContext *ctx) {
    currentType = ctx->type_specifier()->getText();
    string type = anyToString(visit(ctx->type_specifier()));
    string declarationList = anyToString(visit(ctx->declaration_list()));
    if (type == "void") {
        printError(ctx->getStart()->getLine(), "Variable type cannot be void");
    }
    string text = type + " " + declarationList + ";";
    printLog(logout, ctx->getStart()->getLine(), "var_declaration : type_specifier declaration_list SEMICOLON", text);
    return text;
}

any Visitor2205119::visitDeclaration_list(CSubsetParser::Declaration_listContext *ctx) {
    string rule;
    string text;
    string name = "";
    int line = ctx->getStart()->getLine();
    if (ctx->ADDOP() != nullptr) {
        line = ctx->ADDOP()->getSymbol()->getLine();
        if (ctx->declaration_list() == nullptr) {
            string validName = ctx->ID(0)->getText();
            printLog(logout, ctx->ID(0)->getSymbol()->getLine(), "declaration_list : ID", validName);
            if (currentType != "void") {
                if (!symbolTable.insert_current_scope(validName, currentType)) {
                    printError(line, "Multiple declaration of " + validName);
                }
            }
            string invalidName = ctx->ID(1)->getText();
            printError(line, "syntax error, unexpected token(s) '- " + invalidName + "' in declaration list");
            return validName;
        }
        string previous = anyToString(visit(ctx->declaration_list()));
        string invalidName = ctx->ID(0)->getText();
        printError(line, "syntax error, unexpected token(s) '- " + invalidName + "' in declaration list");
        return previous;
    }
    if (!ctx->ID().empty()) {
        name = ctx->ID(0)->getText();
        line = ctx->ID(0)->getSymbol()->getLine();
    }
    if (ctx->declaration_list() == nullptr) {
        if (ctx->LTHIRD() != nullptr) {
            string size = ctx->CONST_INT()->getText();
            text = name + "[" + size + "]";
            rule = "declaration_list : ID LTHIRD CONST_INT RTHIRD";
        } else {
            text = name;
            rule = "declaration_list : ID";
        }
    } else {
        string previous = anyToString(visit(ctx->declaration_list()));
        if (ctx->LTHIRD() != nullptr) {
            string size = ctx->CONST_INT()->getText();
            text = previous + "," + name + "[" + size + "]";
            rule = "declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD";
        } else {
            text = previous + "," + name;
            rule = "declaration_list : declaration_list COMMA ID";
        }
    }
    if (currentType != "void") {
        string symbolType = currentType;
        if (ctx->LTHIRD() != nullptr) {
            string size = ctx->CONST_INT()->getText();
            symbolType += " array[" + size + "]";
        }
        if (!symbolTable.insert_current_scope(name, symbolType)) {
            printError(line, "Multiple declaration of " + name);
        } else {
            SymbolInfo *symbol = symbolTable.getCurrent()->Lookup(name);
            if (symbol != nullptr && ctx->LTHIRD() != nullptr) {
                symbol->setArray(true);
                symbol->setArraySize(stoi(ctx->CONST_INT()->getText()));
            }
        }
    }
    printLog(logout, ctx->getStart()->getLine(), rule, text);
    return text;
}

any Visitor2205119::visitFunc_declaration(CSubsetParser::Func_declarationContext *ctx) {
    visit(ctx->type_specifier());
    string functionName = ctx->ID()->getText();
    string returnType = ctx->type_specifier()->getText();
    vector<string> parameterTypes;
    vector<string> parameterNames;
    if (ctx->parameter_list() != nullptr) {
        parameterTypes = getParameterTypes(ctx->parameter_list());
        parameterNames = getParameterNames(ctx->parameter_list());
    }
    int line = ctx->ID()->getSymbol()->getLine();
    SymbolInfo *existing = symbolTable.getCurrent()->Lookup(functionName);
    if (existing != nullptr) {
        if (!existing->getIsFunction()) {
            printError(line, "Multiple declaration of " + functionName);
        } else {
            if (existing->getReturnType() != returnType) {
                printError(line, "Return type mismatch with function declaration in function " + functionName);
            }
            if (existing->getParameterTypes().size() != parameterTypes.size()) {
                printError(line, "Total number of arguments mismatch with declaration in function " + functionName);
            } else {
                for (size_t i = 0; i < parameterTypes.size(); i++) {
                    if (existing->getParameterTypes()[i] != parameterTypes[i]) {
                        printError(line, to_string(i + 1) + "th argument mismatch with declaration in function " + functionName);
                        break;
                    }
                }
            }
        }
    } else {
        bool inserted = symbolTable.insert_current_scope(functionName, "FUNCTION");
        if (inserted) {
            SymbolInfo *functionSymbol = symbolTable.getCurrent()->Lookup(functionName);
            functionSymbol->setFunction(true);
            functionSymbol->setReturnType(returnType);
            functionSymbol->setDeclared(true);
            functionSymbol->setDefined(false);
            for (const string &type : parameterTypes) {
                functionSymbol->addParameterType(type);
            }
            for (const string &name : parameterNames) {
                functionSymbol->addParameterName(name);
            }
        }
    }
    string rule;
    string params = "";
    if (ctx->parameter_list() != nullptr) {
        params = anyToString(visit(ctx->parameter_list()));
        rule = "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON";
    } else {
        rule = "func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON";
    }
    string text = returnType + " " + functionName + "(" + params + ");";
    printLog(logout, line, rule, text);
    logout << endl;
    symbolTable.enter_scope();
    symbolTable.exit_scope();
    return text;
}

any Visitor2205119::visitFunc_definition(CSubsetParser::Func_definitionContext *ctx) {
    vector<string> delayedFunctionErrors;
    visit(ctx->type_specifier());
    string functionName = ctx->ID()->getText();
    int line = ctx->ID()->getSymbol()->getLine();
    string returnType = ctx->type_specifier()->getText();
    vector<string> parameterTypes;
    vector<string> parameterNames;
    if (ctx->parameter_list() != nullptr) {
        parameterTypes = getParameterTypes(ctx->parameter_list());
        parameterNames = getParameterNames(ctx->parameter_list());
    }
    SymbolInfo *functionSymbol = symbolTable.getCurrent()->Lookup(functionName);
    if (functionSymbol != nullptr) {
        if (!functionSymbol->getIsFunction()) {
            delayedFunctionErrors.push_back("Multiple declaration of " + functionName);
        } else if (functionSymbol->getIsDefined()) {
            delayedFunctionErrors.push_back("Multiple declaration of " + functionName);
        } else {
            if (functionSymbol->getReturnType() != returnType) {
                delayedFunctionErrors.push_back("Return type mismatch with function declaration in function " + functionName);
            }
            if (functionSymbol->getParameterTypes().size() != parameterTypes.size()) {
                delayedFunctionErrors.push_back("Total number of arguments mismatch with declaration in function " + functionName);
            } else {
                for (size_t i = 0; i < parameterTypes.size(); i++) {
                    if (functionSymbol->getParameterTypes()[i] != parameterTypes[i]) {
                        delayedFunctionErrors.push_back( to_string(i + 1) + "th argument mismatch with declaration in function " + functionName);
                        break;
                    }
                }
            }
            functionSymbol->setDefined(true);
        }
    } else {
        bool inserted = symbolTable.insert_current_scope(functionName, "FUNCTION");
        if (inserted) {
            functionSymbol = symbolTable.getCurrent()->Lookup(functionName);
            functionSymbol->setFunction(true);
            functionSymbol->setReturnType(returnType);
            functionSymbol->setDeclared(false);
            functionSymbol->setDefined(true);
            for (const string &type : parameterTypes) {
                functionSymbol->addParameterType(type);
            }
            for (const string &name : parameterNames) {
                functionSymbol->addParameterName(name);
            }
        }
    }
    string previousReturnType = currentFunctionReturnType;
    currentFunctionReturnType = returnType;
    symbolTable.enter_scope();
    string params = "";
    if (ctx->parameter_list() != nullptr) {
        params = anyToString(visit(ctx->parameter_list()));
        if (ctx->parameter_list() != nullptr && ctx->parameter_list()->ADDOP() != nullptr) {
            printError(ctx->parameter_list()->ADDOP()->getSymbol()->getLine(), "syntax error, unexpected token(s) '-' before ')'");
            printError(ctx->parameter_list()->ADDOP()->getSymbol()->getLine(), "1th parameter's name not given in function definition of " + functionName);
        }
    }
    for (const string &message : delayedFunctionErrors) {
        printError(line, message);
    }
    isInsideFuncDefCompound = true;
    string body = anyToString(visit(ctx->compound_statement()));
    isInsideFuncDefCompound = false;
    symbolTable.print_all_tables(logout);
    symbolTable.exit_scope();
    currentFunctionReturnType = previousReturnType;
    string rule;
    if (ctx->parameter_list() != nullptr) {
        rule = "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement";
    } else {
        rule = "func_definition : type_specifier ID LPAREN RPAREN compound_statement";
    }
    string text = returnType + " " + functionName + "(" + params + ")" + body;
    printLog(logout, line, rule, text);
    return text;
}

any Visitor2205119::visitParameter_list(CSubsetParser::Parameter_listContext *ctx) {
    string text;
    string rule;
    if (ctx->parameter_list() == nullptr && ctx->ADDOP() != nullptr) {
        string type = anyToString(visit(ctx->type_specifier()));
        string text = type;
        string rule = "parameter_list : type_specifier";
        printLog(logout, ctx->getStart()->getLine(), rule, text);
        return text;
    }
    if (ctx->parameter_list() != nullptr && ctx->ADDOP() != nullptr) {
        string previous = anyToString(visit(ctx->parameter_list()));
        string type = ctx->type_specifier()->getText();
        vector<string> previousTypes = getParameterTypes(ctx->parameter_list());
        int parameterNumber = static_cast<int>(previousTypes.size()) + 1;
        auto *funcDef = dynamic_cast<CSubsetParser::Func_definitionContext *>(ctx->parent);
        string functionName = "unknown";
        if (funcDef != nullptr && funcDef->ID() != nullptr) {
            functionName = funcDef->ID()->getText();
        }
        printError(ctx->ADDOP()->getSymbol()->getLine(), to_string(parameterNumber) + "th parameter's name not given in function definition of " + functionName);
        text = previous + "," + type + ctx->ADDOP()->getText();
        rule = "parameter_list : parameter_list COMMA type_specifier ADDOP";
        printLog(logout, ctx->getStart()->getLine(), rule, text);
        return text;
    }
    if (ctx->parameter_list() != nullptr) {
        string previous = anyToString(visit(ctx->parameter_list()));
        visit(ctx->type_specifier());
        string current = ctx->type_specifier()->getText();
        if (ctx->ID() != nullptr) {
            current += " " + ctx->ID()->getText();
            string name = ctx->ID()->getText();
            if (symbolTable.getCurrent()->getId() != "1") {
                if (!symbolTable.insert_current_scope(name, current.substr(0, current.find(' ')))) {
                    printError(ctx->ID()->getSymbol()->getLine(), "Multiple declaration of " + name + " in parameter");
                }
            }
        }
        text = previous + "," + current;
        if (ctx->ID() != nullptr) {
            rule = "parameter_list : parameter_list COMMA type_specifier ID";
        } else {
            rule = "parameter_list : parameter_list COMMA type_specifier";
        }
    } else {
        visit(ctx->type_specifier());
        string type = ctx->type_specifier()->getText();
        if (ctx->ID() != nullptr) {
            string name = ctx->ID()->getText();
            text = type + " " + name;
            rule = "parameter_list : type_specifier ID";
            if (symbolTable.getCurrent()->getId() != "1") {
                if (!symbolTable.insert_current_scope(name, type)) {
                    printError(ctx->ID()->getSymbol()->getLine(), "Multiple declaration of " + name + " in parameter");
                }
            }
        } else {
            text = type;
            rule = "parameter_list : type_specifier";
        }
    }
    printLog(logout, ctx->getStart()->getLine(), rule, text);
    return text;
}

any Visitor2205119::visitVariable(CSubsetParser::VariableContext *ctx) {
    string name = ctx->ID()->getText();
    int line = ctx->getStart()->getLine();
    SymbolInfo *symbol = symbolTable.lookup(name);
    if (ctx->expression() != nullptr) {
        string indexType = anyToString(visit(ctx->expression()));
        bool valid = true;
        if (symbol == nullptr) {
            printError(line, "Undeclared variable " + name);
            valid = false;
        }
        else {
            string type = symbol->getType();
            if (!symbol->getIsArray() && !isArrayType(type)) {
                printError(line, name + " not an array");
                valid = false;
            }
            else if (indexType != "int") {
                printError(line, "Expression inside third brackets not an integer");
                valid = false;
            }
        }
        string text = name + "[" + ctx->expression()->getText() + "]";
        printLog(logout, line, "variable : ID LTHIRD expression RTHIRD", text);
        if (!valid || symbol == nullptr) {
            return string("error");
        }
        return getArrayBaseType(symbol->getType());
    }
    if (symbol == nullptr) {
        printError(line, "Undeclared variable " + name);
        printLog(logout, line, "variable : ID", name);
        return string("error");
    }
    printLog(logout, line, "variable : ID", name);
    return symbol->getType();
}

any Visitor2205119::visitFactor(CSubsetParser::FactorContext *ctx) {
    int line = ctx->getStart()->getLine();
    if (ctx->CONST_INT() != nullptr) {
        string text = ctx->CONST_INT()->getText();
        printLog(logout, line, "factor : CONST_INT", text);
        return string("int");
    }
    if (ctx->CONST_FLOAT() != nullptr) {
        string text = formatFloat(ctx->CONST_FLOAT()->getText());
        Token *token = ctx->CONST_FLOAT()->getSymbol();
        CommonToken *commonToken = dynamic_cast<CommonToken *>(token);
        if (commonToken != nullptr) {
            commonToken->setText(text);
        }
        printLog(logout, line, "factor : CONST_FLOAT", text);
        return string("float");
    }
    if (ctx->variable() != nullptr) {
        string type = anyToString(visit(ctx->variable()));
        string text = ctx->variable()->getText();
        if (ctx->INCOP() != nullptr) {
            text += ctx->INCOP()->getText();
            printLog(logout, line, "factor : variable INCOP", text);
            return type;
        }
        if (ctx->DECOP() != nullptr) {
            text += ctx->DECOP()->getText();
            printLog(logout, line, "factor : variable DECOP", text);
            return type;
        }
        printLog(logout, line, "factor : variable", text);
        return type;
    }
    if (ctx->LPAREN() != nullptr && ctx->expression() != nullptr && ctx->ID() == nullptr) {
        string type = anyToString(visit(ctx->expression()));
        string text = "(" + ctx->expression()->getText() + ")";
        printLog(logout, line, "factor : LPAREN expression RPAREN", text);
        return type;
    }
    if (ctx->ID() != nullptr) {
        string functionName = ctx->ID()->getText();
        SymbolInfo *symbol = symbolTable.lookup(functionName);
        vector<string> argumentTypes;
        if (ctx->argument_list() != nullptr) {
            any result = visit(ctx->argument_list());
            if (result.has_value()) {
                try {
                    argumentTypes = any_cast<vector<string>>(result);
                } catch (...) {
                    argumentTypes.clear();
                }
            }
        }
        string argsText = ctx->argument_list() ? ctx->argument_list()->getText() : "";
        string text = functionName + "(" + argsText + ")";
        if (symbol == nullptr) {
            printError(line, "Undeclared function " + functionName);
            printLog(logout, line, "factor : ID LPAREN argument_list RPAREN", text);
            return string("error");
        }
        if (!symbol->getIsFunction()) {
            printError(line, functionName + " is not a function");
            printLog(logout, line, "factor : ID LPAREN argument_list RPAREN", text);
            return string("error");
        }
        vector<string> parameterTypes = symbol->getParameterTypes();
        if (argumentTypes.size() != parameterTypes.size()) {
            printError(line, "Total number of arguments mismatch in function " + functionName);
        }
        else {
            for (size_t i = 0; i < argumentTypes.size(); i++) {
                string actual = argumentTypes[i];
                string expected = parameterTypes[i];
                if (actual == "error")
                    continue;
                if (isArrayType(actual)) {
                    continue;
                }
                if (expected == "int" && actual == "float") {
                    printError(line, to_string(i + 1) + "th argument mismatch in function " + functionName);
                    break;
                }
                if (expected == "float" && actual == "int") {
                    continue;
                }
                if (expected != actual) {
                    printError(line, to_string(i + 1) + "th argument mismatch in function " + functionName);
                    break;
                }
            }
        }
        string returnType = symbol->getReturnType();
        printLog(logout, line, "factor : ID LPAREN argument_list RPAREN", text);
        return returnType;
    }
    return string("error");
}

any Visitor2205119::visitArgument_list(CSubsetParser::Argument_listContext *ctx) {
    if (ctx->arguments() == nullptr) {
        printLog(logout, ctx->getStart()->getLine(), "argument_list : ", "");
        return vector<string>();
    }
    vector<string> args;
    any result = visit(ctx->arguments());
    if (result.has_value()) {
        try {
            args = any_cast<vector<string>>(result);
        } catch (...) {
            args.clear();
        }
    }
    printLog(logout, ctx->getStart()->getLine(), "argument_list : arguments", ctx->getText());
    return args;
}

any Visitor2205119::visitArguments(CSubsetParser::ArgumentsContext *ctx) {
    vector<string> args;
    if (ctx->arguments() != nullptr) {
        any previousResult = visit(ctx->arguments());
        if (previousResult.has_value()) {
            try {
                args = any_cast<vector<string>>(previousResult);
            } catch (...) {
                args.clear();
            }
        }
    }
    if (ctx->logic_expression() != nullptr) {
        string argumentText = ctx->logic_expression()->getText();
        SymbolInfo *argumentSymbol = symbolTable.lookup(argumentText);
        if (argumentSymbol != nullptr && argumentSymbol->getIsArray()) {
            printError(ctx->logic_expression()->getStart()->getLine(), "Type mismatch, " + argumentText + " is an array");
        }
        string type = anyToString(visit(ctx->logic_expression()));
        args.push_back(type);
    }
    string rule;
    if (ctx->arguments() != nullptr) {
        rule = "arguments : arguments COMMA logic_expression";
    } else {
        rule = "arguments : logic_expression";
    }
    printLog(logout, ctx->getStart()->getLine(), rule, ctx->getText());
    return args;
}

any Visitor2205119::visitExpression(CSubsetParser::ExpressionContext *ctx) {
    int line = ctx->getStart()->getLine();
    if (ctx->ASSIGNOP() != nullptr) {
        bool wholeArrayAssignment = false;
        if (ctx->variable() != nullptr && ctx->variable()->expression() == nullptr) {
            string lhsName = ctx->variable()->ID()->getText();
            SymbolInfo *lhsSymbol = symbolTable.lookup(lhsName);
            if (lhsSymbol != nullptr && lhsSymbol->getIsArray()) {
                printError(line, "Type mismatch, " + lhsName + " is an array");
                wholeArrayAssignment = true;
            }
        }
        string lhs = anyToString(visit(ctx->variable()));
        string rhs = anyToString(visit(ctx->logic_expression()));
        string text = ctx->getText();
        string resultType = lhs;
        if (lhs == "error" || rhs == "error") {
            resultType = "error";
        } else if (rhs == "void") {
            printError(line, "Void function used in expression");
            resultType = "error";
        } else if (isArrayType(lhs)) {
            if (!wholeArrayAssignment) {
                string variableName = ctx->variable()->getText();
                printError(line, "Type mismatch, " + variableName + " is an array");
            }
            resultType = "error";
        }
        else if (lhs == "int" && rhs == "float") {
            printError(line, "Type Mismatch");
            resultType = "error";
        }
        printLog(logout, line, "expression : variable ASSIGNOP logic_expression", text);
        return resultType;
    }
    string type = anyToString(visit(ctx->logic_expression()));
    printLog(logout, line, "expression : logic expression", ctx->getText());
    return type;
}

any Visitor2205119::visitLogic_expression(CSubsetParser::Logic_expressionContext *ctx) {
    int line = ctx->getStart()->getLine();
    if (ctx->LOGICOP() != nullptr) {
        string left = anyToString(visit(ctx->rel_expression(0)));
        string right = anyToString(visit(ctx->rel_expression(1)));
        printLog(logout, line, "logic_expression : rel_expression LOGICOP rel_expression", ctx->getText());
        if (left == "error" || right == "error") {
            return string("error");
        }
        return string("int");
    }
    string type = anyToString(visit(ctx->rel_expression(0)));
    printLog(logout, line, "logic_expression : rel_expression", ctx->getText());
    return type;
}

any Visitor2205119::visitRel_expression(CSubsetParser::Rel_expressionContext *ctx) {
    int line = ctx->getStart()->getLine();
    if (ctx->RELOP() != nullptr) {
        string left = anyToString(visit(ctx->simple_expression(0)));
        string right = anyToString(visit(ctx->simple_expression(1)));
        printLog(logout, line, "rel_expression : simple_expression RELOP simple_expression", ctx->getText());
        if (left == "error" || right == "error") {
            return string("error");
        }
        return string("int");
    }
    string type = anyToString(visit(ctx->simple_expression(0)));
    printLog(logout, line, "rel_expression : simple_expression", ctx->getText());
    return type;
}

any Visitor2205119::visitSimple_expression(CSubsetParser::Simple_expressionContext *ctx) {
    int line = ctx->getStart()->getLine();
    if (ctx->ADDOP() != nullptr && ctx->ASSIGNOP() != nullptr) {
        printError(line, "syntax error, invalid operand '=' after '" + ctx->ADDOP()->getText() + "'");
    }
    if (ctx->ADDOP() != nullptr) {
        string left = anyToString(visit(ctx->simple_expression()));
        string right = anyToString(visit(ctx->term()));
        if (left == "void" || right == "void") {
            printError(line, "Void function used in expression");
        }
        string type = arithmeticType(left, right);
        printLog(logout, line, "simple_expression : simple_expression ADDOP term", ctx->getText());
        return type;
    }
    string type = anyToString(visit(ctx->term()));
    printLog(logout, line, "simple_expression : term", ctx->getText());
    return type;
}

any Visitor2205119::visitTerm(CSubsetParser::TermContext *ctx) {
    int line = ctx->getStart()->getLine();
    if (ctx->MULOP() != nullptr) {
        string left = anyToString(visit(ctx->term()));
        string right = anyToString(visit(ctx->unary_expression()));
        if (left == "void" || right == "void") {
            printError(line, "Void function used in expression");
        }
        string op = ctx->MULOP()->getText();
        if (op == "%") {
            if (left != "int" || right != "int") {
                printError(line, "Non-Integer operand on modulus operator");
                printLog(logout, line, "term : term MULOP unary_expression", ctx->getText());
                return string("error");
            }
            string rightText = ctx->unary_expression()->getText();
            if (rightText == "0" || rightText == "+0" || rightText == "-0") {
                printError(line, "Modulus by Zero");
                printLog(logout, line, "term : term MULOP unary_expression", ctx->getText());
                return string("error");
            }
            printLog(logout, line, "term : term MULOP unary_expression", ctx->getText());
            return string("int");
        }
        string type = arithmeticType(left, right);
        printLog(logout, line, "term : term MULOP unary_expression", ctx->getText());
        return type;
    }
    string type = anyToString(visit(ctx->unary_expression()));
    printLog(logout, line, "term : unary_expression", ctx->getText());
    return type;
}

any Visitor2205119::visitUnary_expression(CSubsetParser::Unary_expressionContext *ctx) {
    int line = ctx->getStart()->getLine();
    if (ctx->factor() != nullptr) {
        string type = anyToString(visit(ctx->factor()));
        printLog(logout, line, "unary_expression : factor", ctx->getText());
        return type;
    }
    if (ctx->ADDOP() != nullptr) {
        string type = anyToString(visit(ctx->unary_expression()));
        printLog(logout, line, "unary_expression : ADDOP unary_expression", ctx->getText());
        return type;
    }
    if (ctx->NOT() != nullptr) {
        string operand = anyToString(visit(ctx->unary_expression()));
        if (operand == "error")
            return string("error");
        printLog(logout, line, "unary_expression : NOT unary expression", ctx->getText());
        return string("int");
    }
    return string("error");
}

any Visitor2205119::visitStatements(CSubsetParser::StatementsContext *ctx) {
    string previous = "";
    bool hasPrevious = ctx->statements() != nullptr;
    if (hasPrevious) {
        previous = anyToString(visit(ctx->statements()));
    }
    CSubsetParser::StatementContext *statement = ctx->statement();
    int line = statement->getStart()->getLine();
    MalformedAddAssignParts malformed;
    if (getMalformedAddAssign(statement, malformed)) {
        auto *bad = malformed.badSimple;
        auto *expr = malformed.expression;
        string lhsText = expr->variable()->getText();
        visit(expr->variable());
        visit(bad->simple_expression());
        string leftText = bad->simple_expression()->getText();
        printError(line, "syntax error, invalid operand '=' after '" + bad->ADDOP()->getText() + "'");
        printLog(logout, line, "rel_expression : simple_expression", leftText);
        printLog(logout, line, "logic_expression : rel_expression", leftText);
        string firstExpr = lhsText + "=" + leftText;
        printLog(logout, line, "expression : variable ASSIGNOP logic_expression", firstExpr);
        printError(line, "syntax error, missing ';' after expression '" + firstExpr + "'");
        printLog(logout, line, "expression_statement : expression (missing SEMICOLON)", firstExpr);
        string firstStatement = ensureOneTrailingNewline(firstExpr);
        printLog(logout, line, "statement : expression_statement", firstStatement);
        string firstAccum = previous + firstStatement;
        printLog(logout, line, hasPrevious ? "statements : statements statement" : "statements : statement", firstAccum);
        visit(bad->term());
        string secondExpr = bad->term()->getText();
        printLog(logout, line, "simple_expression : term", secondExpr);
        printLog(logout, line, "rel_expression : simple_expression", secondExpr);
        printLog(logout, line, "logic_expression : rel_expression", secondExpr);
        printLog(logout, line, "expression : logic expression", secondExpr);
        printError(line, "syntax error, missing ';' after expression '" + secondExpr + "'");
        printLog(logout, line, "expression_statement : expression (missing SEMICOLON)", secondExpr);
        string secondStatement = ensureOneTrailingNewline(secondExpr);
        printLog(logout, line, "statement : expression_statement", secondStatement);
        string text = firstAccum + secondStatement;
        printLog(logout, line, "statements : statements statement", text);
        return text;
    }
    string current = anyToString(visit(statement));
    string text = hasPrevious ? previous + current : current;
    string rule = hasPrevious ? "statements : statements statement" : "statements : statement";
    printLog(logout, line, rule, text);
    return text;
}

any Visitor2205119::visitStatement(CSubsetParser::StatementContext *ctx) {
    int line = ctx->getStart()->getLine();
    if (ctx->var_declaration() != nullptr) {
        string text = ensureOneTrailingNewline(anyToString(visit(ctx->var_declaration())));
        printLog(logout, line, "statement : var_declaration", text);
        return text;
    }
    if (ctx->RETURN() != nullptr) {
        string returnType = anyToString(visit(ctx->expression()));
        if (returnType != "error") {
            if (currentFunctionReturnType == "int" && returnType == "float") {
                printError(line, "Type Mismatch");
            }
            else if (currentFunctionReturnType != "" && currentFunctionReturnType != "void" && currentFunctionReturnType != returnType) {
                printError(line, "Type Mismatch");
            }
        }
        string text = ensureOneTrailingNewline("return " + ctx->expression()->getText() + ";");
        printLog(logout, line, "statement : RETURN expression SEMICOLON", text);
        return text;
    }
    if (ctx->expression_statement().size() > 0 && ctx->FOR() == nullptr) {
        string text = ensureOneTrailingNewline(anyToString(visit(ctx->expression_statement(0))));
        printLog(logout, line, "statement : expression_statement", text);
        return text;
    }
    if (ctx->compound_statement() != nullptr) {
        string text = anyToString(visit(ctx->compound_statement()));
        printLog(logout, line, "statement : compound_statement", text);
        return text;
    }
    if (ctx->FOR() != nullptr) {
        string init = anyToString(visit(ctx->expression_statement(0)));
        string cond = anyToString(visit(ctx->expression_statement(1)));
        visit(ctx->expression());
        string body = anyToString(visit(ctx->statement(0)));
        string text = "for(" + init + cond + ctx->expression()->getText() + ")" + body;
        text = ensureOneTrailingNewline(text);
        printLog(logout, line, "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement", text);
        return text;
    }
    if (ctx->IF() != nullptr) {
        string conditionType = anyToString(visit(ctx->expression()));
        if (conditionType == "float") {
            printError(line, "If condition cannot be float");
        }
        string first = anyToString(visit(ctx->statement(0)));
        string text = "if (" + ctx->expression()->getText() + ")" + first;
        string rule;
        if (ctx->statement().size() >= 2) {
            string second = anyToString(visit(ctx->statement(1)));
            text += "else\n" + second;
            rule = "statement : IF LPAREN expression RPAREN statement ELSE statement";
        }
        else {
            rule = "statement : IF LPAREN expression RPAREN statement";
        }
        text = ensureOneTrailingNewline(text);
        printLog(logout, line, rule, text);
        return text;
    }
    if (ctx->WHILE() != nullptr) {
        string conditionType = anyToString(visit(ctx->expression()));
        if (conditionType == "float") {
            printError(line, "While condition cannot be float");
        }
        string body = anyToString(visit(ctx->statement(0)));
        string text = "while (" + ctx->expression()->getText() + ")" + body;
        text = ensureOneTrailingNewline(text);
        printLog(logout, line, "statement : WHILE LPAREN expression RPAREN statement", text);
        return text;
    }
    if (ctx->PRINTLN() != nullptr) {
        string name = ctx->ID()->getText();
        SymbolInfo *symbol = symbolTable.lookup(name);
        if (symbol == nullptr) {
            printError(line, "Undeclared variable " + name);
        }
        string text = ensureOneTrailingNewline("printf(" + name + ");");
        printLog(logout, line, "statement : PRINTLN LPAREN ID RPAREN SEMICOLON", text);
        return text;
    }
    return visitChildren(ctx);
}

any Visitor2205119::visitExpression_statement(CSubsetParser::Expression_statementContext *ctx) {
    int line = ctx->getStart()->getLine();
    if (ctx->expression() == nullptr) {
        printLog(logout, line, "expression_statement : SEMICOLON", ";");
        return string(";");
    }
    visit(ctx->expression());
    string text = ctx->expression()->getText();
    if (ctx->SEMICOLON() != nullptr) {
        text += ";";
        printLog(logout, line, "expression_statement : expression SEMICOLON", text);
        return text;
    }
    printError(line, "syntax error, missing ';' after expression '" + text + "'");
    printLog(logout, line, "expression_statement : expression (missing SEMICOLON)", text);
    return text;
}

any Visitor2205119::visitCompound_statement(CSubsetParser::Compound_statementContext *ctx) {
    bool enteredHere = false;
    if (isInsideFuncDefCompound) {
        isInsideFuncDefCompound = false;
    }
    else {
        symbolTable.enter_scope();
        enteredHere = true;
    }
    string statementsText = "";
    if (ctx->statements() != nullptr) {
        statementsText = anyToString(visit(ctx->statements()));
    }
    string rule;
    string text;
    if (ctx->statements() != nullptr) {
        rule = "compound_statement : LCURL statements RCURL";
        text = "{\n" + statementsText + "}\n";
    }
    else {
        rule = "compound_statement : LCURL RCURL";
        text = "{}\n";
    }
    printLog(logout, ctx->getStart()->getLine(), rule, text);
    if (enteredHere) {
        symbolTable.print_all_tables(logout);
        symbolTable.exit_scope();
    }
    return text;
}