#include "AsmGenerator.h"


using namespace std;
using namespace antlr4;


void AsmGenerator::allocateGlobalVariable(string name){
    writeData(name + " dd 0");
    symbolTable.insert_current_scope( name,"ID" );
    SymbolInfo* symbol = symbolTable.lookup(name);
    if(symbol){
        symbol->setGlobal(true);
    }
}

void AsmGenerator::pushResult()
{
    emit("push eax");
}

void AsmGenerator::popTo(string reg)
{
    emit("pop "+reg);
}

string AsmGenerator::getVariableLocation(string name){
    SymbolInfo *symbol = symbolTable.lookup(name);
    if(symbol && !symbol->getIsGlobal()){
        return "[ebp-" + to_string(symbol->getStackOffset()) + "]";
    }
    return "["+name+"]";
}

AsmGenerator::AsmGenerator(string filename ) : symbolTable(30,false), labelCount(0), localOffset(0), insideFunction(false){
    asmFile.open(filename);
    if(!asmFile.is_open()) {
        cerr<<"Cannot create asm file"<<endl;
        return;
    }
    generateHeader();
}

void AsmGenerator::generatePrintFunction(){
    asmFile << "println:\n";
    asmFile << "push eax\n";
    asmFile << "push ebx\n";
    asmFile << "push ecx\n";
    asmFile << "push edx\n";
    asmFile << "push esi\n";
    asmFile << "push edi\n";
    asmFile << "sub esp,32\n";
    // check negative
    asmFile << "test eax,eax\n";
    asmFile << "jns print_positive\n";
    // print '-'
    asmFile << "push eax\n";
    asmFile << "sub esp,1\n";
    asmFile << "mov byte [esp],'-'\n";
    asmFile << "mov eax,4\n";
    asmFile << "mov ebx,1\n";
    asmFile << "mov ecx,esp\n";
    asmFile << "mov edx,1\n";
    asmFile << "int 0x80\n";
    asmFile << "add esp,1\n";
    asmFile << "pop eax\n";
    asmFile << "neg eax\n";
    asmFile << "print_positive:\n";
    // convert integer to string
    asmFile << "mov ebx,10\n";
    asmFile << "lea esi,[esp+31]\n";
    asmFile << "mov byte [esi],10\n";
    asmFile << "dec esi\n";
    asmFile << "cmp eax,0\n";
    asmFile << "jne convert_loop\n";
    asmFile << "mov byte [esi],'0'\n";
    asmFile << "dec esi\n";
    asmFile << "jmp finish_convert\n";
    asmFile << "convert_loop:\n";
    asmFile << "xor edx,edx\n";
    asmFile << "div ebx\n";
    asmFile << "add dl,'0'\n";
    asmFile << "mov [esi],dl\n";
    asmFile << "dec esi\n";
    asmFile << "test eax,eax\n";
    asmFile << "jnz convert_loop\n";
    asmFile << "finish_convert:\n";
    asmFile << "inc esi\n";
    // write syscall
    asmFile << "lea edx,[esp+32]\n";
    asmFile << "sub edx,esi\n";
    asmFile << "mov eax,4\n";
    asmFile << "mov ebx,1\n";
    asmFile << "mov ecx,esi\n";
    asmFile << "int 0x80\n";
    // restore
    asmFile << "add esp,32\n";
    asmFile << "pop edi\n";
    asmFile << "pop esi\n";
    asmFile << "pop edx\n";
    asmFile << "pop ecx\n";
    asmFile << "pop ebx\n";
    asmFile << "pop eax\n";
    asmFile << "ret\n\n";
}

AsmGenerator::~AsmGenerator() {
    closeFile();
}

void AsmGenerator::generateHeader(){
    asmFile<<"format ELF executable 4\n\n";
    asmFile<<"entry start\n\n";
    asmFile<<"segment readable writeable\n\n";
    asmFile<<"buffer db '0000000000',10\n\n";
}

void AsmGenerator::generateDataSection(){
    asmFile<<"buffer db '0000000000',10\n\n";
}

void AsmGenerator::generateCodeSection(){
    asmFile<<"\nsegment readable executable\n\n";
    generatePrintFunction();
    asmFile<<"start:\n\n";
    asmFile<<"call main\n";
    asmFile<<"mov eax,1\n";
    asmFile<<"xor ebx,ebx\n";
    asmFile<<"int 0x80\n\n";
    for(string line : codeBuffer){
        asmFile<<line<<endl;
    }
}

void AsmGenerator::emit(string code){
    codeBuffer.push_back(code);
}

void AsmGenerator::writeData(string code){
    asmFile<<code<<endl;
}

string AsmGenerator::newLabel(){
    return "L" + to_string(labelCount++);
}

void AsmGenerator::writeCodeSectionToFile(){
    generateCodeSection();
}

void AsmGenerator::closeFile(){
    if(asmFile.is_open()){
        writeCodeSectionToFile();
        asmFile.close();
    }
}

/*
    Visitor Functions
*/

any AsmGenerator::visitStart( CSubsetParser::StartContext *ctx){
    visit(ctx->program());
    return nullptr;
}

any AsmGenerator::visitProgram(CSubsetParser::ProgramContext *ctx){
    if(ctx->program()!=nullptr) visit(ctx->program());
    visit(ctx->unit());
    return nullptr;
}

any AsmGenerator::visitUnit( CSubsetParser::UnitContext *ctx){
    if(ctx->var_declaration()!=nullptr) {
        visit(ctx->var_declaration());
    }
    else if(ctx->func_definition()!=nullptr) {
        visit(ctx->func_definition());
    }
    return nullptr;
}

any AsmGenerator::visitVar_declaration( CSubsetParser::Var_declarationContext *ctx){
    /*
        Example:
        int a;
        will later become:
        a dd 0
    */
    visit(ctx->declaration_list());
    return nullptr;
}

any AsmGenerator::visitDeclaration_list(CSubsetParser::Declaration_listContext *ctx){
    if(ctx->declaration_list()!=nullptr){
        visit(ctx->declaration_list());
    }
    if(!ctx->ID().empty()){
        string name = ctx->ID(0)->getText();
        if(insideFunction){
            localOffset +=4;
            emit("sub esp,4");
            symbolTable.insert_current_scope(name, "ID" );
            SymbolInfo *symbol = symbolTable.lookup(name);
            if(symbol){
                symbol->setStackOffset(localOffset);
                symbol->setGlobal(false);
            }
        }
        else{
            allocateGlobalVariable(name);
        }
    }
    return nullptr;
}

any AsmGenerator::visitFunc_definition( CSubsetParser::Func_definitionContext *ctx){
    insideFunction = true;
    localOffset = 0;
    currentFunctionEnd = newLabel();
    symbolTable.enter_scope();
    string functionName = ctx->ID()->getText();
    emit(functionName+":");
    emit("push ebp");
    emit("mov ebp,esp");
    visit(ctx->compound_statement());
    symbolTable.exit_scope();
    emit(currentFunctionEnd + ":");
    emit("add esp," + to_string(localOffset));
    emit("pop ebp");
    emit("ret");
    insideFunction = false;
    return nullptr;
}

any AsmGenerator::visitCompound_statement( CSubsetParser::Compound_statementContext *ctx){
    visitChildren(ctx);
    return nullptr;
}

any AsmGenerator::visitStatement( CSubsetParser::StatementContext *ctx){
    if(ctx->PRINTLN()!=nullptr){
        string name = ctx->ID()->getText();
        emit("mov eax," + getVariableLocation(name));
        emit("call println");
        return nullptr;
    }
    if(ctx->RETURN()!=nullptr){
        visit(ctx->expression());
        emit("jmp " + currentFunctionEnd);
        return nullptr;
    }
    visitChildren(ctx);
    return nullptr;
}

any AsmGenerator::visitExpression( CSubsetParser::ExpressionContext *ctx){
    /*
        Later:
        a=b+c
        ->
        mov eax,[b]
        add eax,[c]
        mov [a],eax
    */
    if(ctx->ASSIGNOP()!=nullptr){
        string lhs = ctx->variable()->ID()->getText();
        visit(ctx->logic_expression());
        emit("push eax");
        emit("pop eax");
        emit( "mov " + getVariableLocation(lhs) + ",eax" );
        return lhs;
    }
    visitChildren(ctx);
    return nullptr;
}

any AsmGenerator::visitLogic_expression( CSubsetParser::Logic_expressionContext *ctx){
    if(ctx->LOGICOP()!=nullptr){
        string op = ctx->LOGICOP()->getText();
        visit(ctx->rel_expression(0));
        emit("push eax");
        visit(ctx->rel_expression(1));
        emit("pop ebx");
        if(op == "&&"){
            emit("and eax,ebx");
        }
        else if(op == "||"){
            emit("or eax,ebx");
        }
        return string("eax");
    }
    visit(ctx->rel_expression(0));
    return string("eax");
}

any AsmGenerator::visitRel_expression( CSubsetParser::Rel_expressionContext *ctx){
    if(ctx->RELOP()!=nullptr){
        string trueLabel = newLabel();
        string endLabel = newLabel();
        visit(ctx->simple_expression(0));
        emit("push eax");
        visit(ctx->simple_expression(1));
        emit("pop ebx");
        emit("cmp ebx,eax");
        string op = ctx->RELOP()->getText();
        if(op == ">") emit("jg "+ trueLabel);
        else if(op == "<") emit("jl "+trueLabel);
        else if(op == ">=") emit("jge " + trueLabel);
        else if(op == "<=") emit("jle " + trueLabel);
        else if(op == "==") emit("je " + trueLabel);
        else if(op == "!=") emit("jne " + trueLabel);
        emit("mov eax,0");
        emit("jmp " + endLabel);
        emit(trueLabel + ":");
        emit("mov eax,1");
        emit(endLabel + ":");
        return string("eax");
    }
    visit(ctx->simple_expression(0));
    return string("eax");
}

any AsmGenerator::visitSimple_expression( CSubsetParser::Simple_expressionContext *ctx){
    if(ctx->ADDOP()!=nullptr){
        visit(ctx->simple_expression());
        emit("push eax");
        visit(ctx->term());
        emit("pop ebx");
        string op = ctx->ADDOP()->getText();
        if(op == "+"){
            emit("add ebx,eax");
        }
        else if(op == "-"){
            emit("sub ebx,eax");
        }
        emit("mov eax,ebx");
        emit("push eax");
        emit("pop eax");
        return string("eax");
    }
    visit(ctx->term());
    return string("eax");
}

any AsmGenerator::visitTerm( CSubsetParser::TermContext *ctx){
    if(ctx->MULOP()!=nullptr){
        visit(ctx->term());
        emit("push eax");
        visit(ctx->unary_expression());
        emit("pop ebx");
        string op = ctx->MULOP()->getText();
        if(op == "*"){
            emit("imul ebx,eax");
            emit("mov eax,ebx");
        }
        else if(op == "/"){
            emit("mov ecx,eax");
            emit("mov eax,ebx");
            emit("cdq");
            emit("idiv ecx");
        }
        else if(op == "%"){
            emit("mov ecx,eax");
            emit("mov eax,ebx");
            emit("cdq");
            emit("idiv ecx");
            emit("mov eax,edx");
            emit("push eax");
            emit("pop eax");
        }
        return string("eax");
    }
    visit(ctx->unary_expression());
    return string("eax");
}

any AsmGenerator::visitUnary_expression( CSubsetParser::Unary_expressionContext *ctx){
    if(ctx->ADDOP()!=nullptr){
        string op = ctx->ADDOP()->getText();
        visit(ctx->unary_expression());
        if(op == "-"){
            emit("neg eax");
            emit("push eax");
            emit("pop eax");
        }
        return string("eax");
    }
    if(ctx->NOT()!=nullptr){
        visit(ctx->unary_expression());
        emit("cmp eax,0");
        emit("sete al");
        emit("movzx eax,al");
        return string("eax");
    }
    visit(ctx->factor());
    return string("eax");
}

any AsmGenerator::visitFactor( CSubsetParser::FactorContext *ctx){
    /*
        Constants later:
        5
        ->
        mov eax,5
    */
    if(ctx->ID()!=nullptr && ctx->argument_list()!=nullptr){
        string fname = ctx->ID()->getText();
        emit("call " + fname);
        return string("eax");
    }
    if(ctx->expression()!=nullptr){
        visit(ctx->expression());
        return string("eax");
    }
    else if(ctx->INCOP()){
        string name = ctx->variable()->ID()->getText();
        emit("mov eax," + getVariableLocation(name));
        emit("push eax");
        emit("add eax,1");
        emit("mov " + getVariableLocation(name) + ",eax");
        emit("pop eax");
    }
    else if(ctx->DECOP()){
        string name = ctx->variable()->ID()->getText();
    // return old value
        emit("mov eax," + getVariableLocation(name));
    // save old value because postfix returns it
        emit("push eax");
    // decrement variable
        emit("sub eax,1");
        emit("mov " + getVariableLocation(name) + ",eax");
    // restore old value as expression result
        emit("pop eax");
    }
    if(ctx->CONST_INT()!=nullptr){
        string value = ctx->CONST_INT()->getText();
        emit( "mov eax," + value );
        return string("eax");
    }
    if(ctx->CONST_FLOAT()!=nullptr){
        string value = ctx->CONST_FLOAT()->getText();
        emit("mov eax," + value);
        return string("eax");
    }
    if(ctx->variable()!=nullptr){
        string name = ctx->variable()->getText();
        emit("mov eax," + getVariableLocation(name));
        return string("eax");
    }
    visitChildren(ctx);
    return nullptr;
}