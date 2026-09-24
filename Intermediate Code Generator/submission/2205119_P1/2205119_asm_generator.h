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
    string getVariableAddress(string name);
    void allocateGlobalVariable(string name,string value);
    void allocateLocalVariable(string name);
    ofstream asmFile;
    vector<string> codeBuffer;
    SymbolTable symbolTable;
    int labelCount;
    string currentFunctionEnd;
    int localOffset;
    bool insideFunction;
    // assembly helper functions
    void generateHeader();
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
    void emit(string code);
    void writeData(string code);
    string newLabel();
    // Parse tree visitors
    any visitStart(CSubsetParser::StartContext *ctx) override;
    any visitProgram(CSubsetParser::ProgramContext *ctx) override;
    any visitUnit(CSubsetParser::UnitContext *ctx) override;
    any visitVar_declaration( CSubsetParser::Var_declarationContext *ctx ) override;
    any visitDeclaration_list( CSubsetParser::Declaration_listContext *ctx ) override;
    any visitFunc_definition( CSubsetParser::Func_definitionContext *ctx ) override;
    any visitCompound_statement( CSubsetParser::Compound_statementContext *ctx ) override;
    any visitStatement( CSubsetParser::StatementContext *ctx ) override;
    any visitExpression(CSubsetParser::ExpressionContext *ctx ) override;
    any visitLogic_expression( CSubsetParser::Logic_expressionContext *ctx ) override;
    any visitRel_expression( CSubsetParser::Rel_expressionContext *ctx ) override;
    any visitSimple_expression( CSubsetParser::Simple_expressionContext *ctx ) override;
    any visitTerm( CSubsetParser::TermContext *ctx ) override;
    any visitUnary_expression( CSubsetParser::Unary_expressionContext *ctx ) override;
    any visitFactor( CSubsetParser::FactorContext *ctx ) override;
    any visitChain_print(CSubsetParser::Chain_printContext *ctx) override;
    any visitExpression_list(CSubsetParser::Expression_listContext *ctx);
    void closeFile();
};


#endif