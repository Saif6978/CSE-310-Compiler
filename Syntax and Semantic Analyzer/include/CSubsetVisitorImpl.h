#ifndef CSUBSET_VISITOR_IMPL_H
#define CSUBSET_VISITOR_IMPL_H

#include "antlr4-runtime.h"
#include "SymbolTable.h"
#include "CSubsetBaseVisitor.h"
#include <fstream>
#include <string>

class CSubsetVisitorImpl : public CSubsetBaseVisitor {
private:
    SymbolTable symbolTable;
    std::string currentType;
    std::string currentFunctionReturnType;
    antlr4::TokenStream *tokens;
    bool isInsideFuncDefCompound = false;

public:
    std::ofstream logout;
    std::ofstream errorout;
    int errorCount;

    void printError(int line, const std::string &message);
    void printError(std::ofstream &out, int line, const std::string &message);

    CSubsetVisitorImpl(antlr4::TokenStream *tokens = nullptr,
                       const std::string &logFileName = "log.txt",
                       const std::string &errorFileName = "error.txt")
        : symbolTable(30, false),
          currentType(""),
          currentFunctionReturnType(""),
          tokens(tokens),
          isInsideFuncDefCompound(false),
          errorCount(0) {
        logout.open(logFileName, std::ios::out | std::ios::trunc);
        errorout.open(errorFileName, std::ios::out | std::ios::trunc);
        if (!logout.is_open()) {
            std::cerr << "Error opening log output file: " << logFileName << std::endl;
        }
        if (!errorout.is_open()) {
            std::cerr << "Error opening error output file: " << errorFileName << std::endl;
        }
    }

    ~CSubsetVisitorImpl() {
        if (logout.is_open()) logout.close();
        if (errorout.is_open()) errorout.close();
    }

    std::any visitStart(CSubsetParser::StartContext *ctx) override;
    std::any visitUnit(CSubsetParser::UnitContext *ctx) override;
    std::any visitProgram(CSubsetParser::ProgramContext *ctx) override;
    std::any visitType_specifier(CSubsetParser::Type_specifierContext *ctx) override;
    std::any visitVar_declaration(CSubsetParser::Var_declarationContext *ctx) override;
    std::any visitDeclaration_list(CSubsetParser::Declaration_listContext *ctx) override;
    std::any visitFunc_definition(CSubsetParser::Func_definitionContext *ctx) override;
    std::any visitFunc_declaration(CSubsetParser::Func_declarationContext *ctx) override;
    std::any visitParameter_list(CSubsetParser::Parameter_listContext *ctx) override;
    std::any visitVariable(CSubsetParser::VariableContext *ctx) override;
    std::any visitFactor(CSubsetParser::FactorContext *ctx) override;
    std::any visitArgument_list(CSubsetParser::Argument_listContext *ctx) override;
    std::any visitArguments(CSubsetParser::ArgumentsContext *ctx) override;
    std::any visitExpression(CSubsetParser::ExpressionContext *ctx) override;
    std::any visitLogic_expression(CSubsetParser::Logic_expressionContext *ctx) override;
    std::any visitRel_expression(CSubsetParser::Rel_expressionContext *ctx) override;
    std::any visitSimple_expression(CSubsetParser::Simple_expressionContext *ctx) override;
    std::any visitTerm(CSubsetParser::TermContext *ctx) override;
    std::any visitUnary_expression(CSubsetParser::Unary_expressionContext *ctx) override;
    std::any visitStatement(CSubsetParser::StatementContext *ctx) override;
    std::any visitStatements(CSubsetParser::StatementsContext *ctx) override;
    std::any visitExpression_statement(CSubsetParser::Expression_statementContext *ctx) override;
    std::any visitCompound_statement(CSubsetParser::Compound_statementContext *ctx) override;
};

#endif // CSUBSET_VISITOR_IMPL_H
