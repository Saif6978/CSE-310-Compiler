#ifndef VISITOR_2205119_H
#define VISITOR_2205119_H

#include "antlr4-runtime.h"
#include "2205119_symbol_table.h"
#include "CSubsetBaseVisitor.h"
#include <fstream>
#include <string>

using namespace std;
using namespace antlr4;

class Visitor2205119 : public CSubsetBaseVisitor {
private:
    SymbolTable symbolTable;
    string currentType;
    string currentFunctionReturnType;
    TokenStream *tokens;
    bool isInsideFuncDefCompound = false;

public:
    ofstream logout;
    ofstream errorout;
    int errorCount;

    void printError(int line, const string &message);
    void printError(ofstream &out, int line, const string &message);

    // Default names preserve the original single-input assignment behaviour.
    // Batch mode can provide separate output names for each input file.
    Visitor2205119(TokenStream *tokens = nullptr, const string &logFileName = "log.txt", const string &errorFileName = "error.txt" )
        :   symbolTable(30, false),
            currentType(""),
            currentFunctionReturnType(""),
            tokens(tokens),
            isInsideFuncDefCompound(false),
            errorCount(0)
    {
        logout.open(logFileName, ios::out | ios::trunc);
        errorout.open(errorFileName, ios::out | ios::trunc);
        if (!logout.is_open()) {
            cerr << "Error opening log output file: " << logFileName << endl;
        }
        if (!errorout.is_open()) {
            cerr << "Error opening error output file: " << errorFileName << endl;
        }
    }

    ~Visitor2205119() {
        if (logout.is_open()) logout.close();
        if (errorout.is_open()) errorout.close();
    }
    SymbolTable& getSymbolTable(){
        return symbolTable;
    }

    any visitStart(CSubsetParser::StartContext *ctx) override;
    any visitUnit(CSubsetParser::UnitContext *ctx) override;
    any visitProgram(CSubsetParser::ProgramContext *ctx) override;
    any visitType_specifier(CSubsetParser::Type_specifierContext *ctx) override;
    any visitVar_declaration(CSubsetParser::Var_declarationContext *ctx) override;
    any visitDeclaration_list(CSubsetParser::Declaration_listContext *ctx) override;
    any visitFunc_definition(CSubsetParser::Func_definitionContext *ctx) override;
    any visitFunc_declaration(CSubsetParser::Func_declarationContext *ctx) override;
    any visitParameter_list(CSubsetParser::Parameter_listContext *ctx) override;
    any visitVariable(CSubsetParser::VariableContext *ctx) override;
    any visitFactor(CSubsetParser::FactorContext *ctx) override;
    any visitArgument_list(CSubsetParser::Argument_listContext *ctx) override;
    any visitArguments(CSubsetParser::ArgumentsContext *ctx) override;
    any visitExpression(CSubsetParser::ExpressionContext *ctx) override;
    any visitLogic_expression(CSubsetParser::Logic_expressionContext *ctx) override;
    any visitRel_expression(CSubsetParser::Rel_expressionContext *ctx) override;
    any visitSimple_expression(CSubsetParser::Simple_expressionContext *ctx) override;
    any visitTerm(CSubsetParser::TermContext *ctx) override;
    any visitUnary_expression(CSubsetParser::Unary_expressionContext *ctx) override;
    any visitStatement(CSubsetParser::StatementContext *ctx) override;
    any visitStatements(CSubsetParser::StatementsContext *ctx) override;
    any visitExpression_statement(CSubsetParser::Expression_statementContext *ctx) override;
    any visitCompound_statement(CSubsetParser::Compound_statementContext *ctx) override;
};

#endif
