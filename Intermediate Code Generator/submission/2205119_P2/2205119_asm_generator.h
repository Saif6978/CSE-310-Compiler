#ifndef ASM_GENERATOR_2205119_H
#define ASM_GENERATOR_2205119_H

#include "antlr4-runtime.h"
#include "CSubsetBaseVisitor.h"
#include "CSubsetParser.h"

#include "2205119_symbol_table.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace antlr4;


class AsmGenerator : public CSubsetBaseVisitor {

private:
    int stackOffset;
    ofstream asmFile;
    int parameterOffset;
    vector<string> codeBuffer;
    SymbolTable symbolTable;
    int labelCount;
    string currentFunctionEnd;
    int localOffset;
    bool insideFunction;
    vector<string> localAllocations;
    // assembly helper functions
    void allocateGlobalVariable(string name);
    void allocateGlobalArray(string name, int size);
    void generateArrayAddress(CSubsetParser::VariableContext *ctx , string reg);
    void generateHeader();
    void generateDataSection();
    void generateCodeSection();
    void generatePrintFunction();
    string getVariableLocation(string name);
    void pushResult();
    void popTo(string reg);
    void writeCodeSectionToFile();
public:
    AsmGenerator(string filename);
    ~AsmGenerator();
    // basic assembly writing
    void emit(string code , int line = 0);
    void writeData(string code);
    void collectArguments(CSubsetParser::ArgumentsContext *ctx , vector<CSubsetParser::Logic_expressionContext*> &args );
    string newLabel();
    // Parse tree visitors
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
    void closeFile();
};


#endif