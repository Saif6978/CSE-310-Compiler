#include "2205119_asm_generator.h"


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

void AsmGenerator::allocateGlobalArray(string name,int size){
    // rd = reserve DWORDs in FASM
    writeData(name + " rd " + to_string(size));
    symbolTable.insert_current_scope(name, "ID");
    SymbolInfo *symbol = symbolTable.lookup(name);
    if(symbol){
        symbol->setGlobal(true);
        symbol->setArray(true);
        symbol->setArraySize(size);
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
        if(symbol->getIsParameter()){
            return "[ebp+" + to_string(symbol->getStackOffset())+"]";
        }
        return "[ebp-" + to_string(symbol->getStackOffset()) + "]";
    }
    return "["+name+"]";
}

void AsmGenerator::generateArrayAddress( CSubsetParser::VariableContext *ctx, string reg){
    string name = ctx->ID()->getText();
    SymbolInfo *symbol = symbolTable.lookup(name);
    // evaluate index
    visit(ctx->expression());
    // int = 4 bytes
    emit("imul eax,4");
    if(symbol && symbol->getIsGlobal()){
        emit("lea " + reg +",[" + name + "]");
    }
    else if(symbol){
        emit("lea " + reg +",[ebp-" +to_string(symbol->getStackOffset()) +"]");
    }
    emit("add " + reg + ",eax");
}

AsmGenerator::AsmGenerator(string filename ) : symbolTable(30,false), labelCount(0), localOffset(0), parameterOffset(0),insideFunction(false){
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
    asmFile<<"call func_main\n";
    asmFile<<"mov eax,1\n";
    asmFile<<"xor ebx,ebx\n";
    asmFile<<"int 0x80\n\n";
    for(string line : codeBuffer){
        asmFile<<line<<endl;
    }
}

void AsmGenerator::emit(string code,int line){
    if(line > 0){
        codeBuffer.push_back("; line " + to_string(line));
    }
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
    else if(ctx->func_declaration() != nullptr){
        visit(ctx->func_declaration());
    }
    else if(ctx->func_definition()!=nullptr) {
        visit(ctx->func_definition());
    }
    return nullptr;
}

any AsmGenerator::visitType_specifier(CSubsetParser::Type_specifierContext *ctx){
    if(ctx->INT() != nullptr) ;
    else if(ctx->FLOAT() != nullptr) ;
    else ;
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
        bool isArray = (ctx->LTHIRD()!=nullptr);
        int arraySize = 1;
        if(isArray){
            arraySize = stoi(ctx->CONST_INT()->getText());
        }
        int bytes = arraySize * 4;
        if(insideFunction){
            localOffset += bytes;
            emit("sub esp," + to_string(bytes));
            symbolTable.insert_current_scope(name, "ID" );
            SymbolInfo *symbol = symbolTable.lookup(name);
            if(symbol){
                symbol->setStackOffset(localOffset);
                symbol->setGlobal(false);
                if(isArray){
                    symbol->setArray(true);
                    symbol->setArraySize(arraySize);
                }
            }
        }
        else{
            if(isArray){
                allocateGlobalArray(name , arraySize);
            }
            else{
                allocateGlobalVariable(name);
            }
        }
    }
    return nullptr;
}

any AsmGenerator::visitVariable(CSubsetParser::VariableContext *ctx){
    string name = ctx->ID()->getText();
    if(ctx->expression()==nullptr){
        emit("mov eax," + getVariableLocation(name));
        return string("eax");
    }
    generateArrayAddress(ctx,"edx");
    emit("mov eax,[edx]");
    return string("eax");
}

any AsmGenerator::visitFunc_declaration(CSubsetParser::Func_declarationContext *ctx){
    return nullptr;
}

any AsmGenerator::visitFunc_definition( CSubsetParser::Func_definitionContext *ctx){
    insideFunction = true;
    localOffset = 0;
    parameterOffset=0;
    currentFunctionEnd = newLabel();
    symbolTable.enter_scope();
    string functionName = ctx->ID()->getText();
    emit("func_"+functionName+":");
    emit("push ebp");
    emit("mov ebp,esp");
    if(ctx->parameter_list()!=nullptr){
        parameterOffset=8;
        visit(ctx->parameter_list());
    }
    visit(ctx->compound_statement());
    symbolTable.exit_scope();
    emit(currentFunctionEnd + ":");
    emit("add esp," + to_string(localOffset));
    emit("pop ebp");
    emit("ret");
    insideFunction = false;
    return nullptr;
}

any AsmGenerator::visitParameter_list(CSubsetParser::Parameter_listContext *ctx){
    if(ctx->parameter_list()!=nullptr){
        visit(ctx->parameter_list());
    }
    if(ctx->ID()!=nullptr){
        string name = ctx->ID()->getText();
        symbolTable.insert_current_scope(name,"ID");
        SymbolInfo* symbol = symbolTable.lookup(name);
        if(symbol){
            symbol->setParameter(true);
            symbol->setStackOffset(parameterOffset);
        }
        parameterOffset+=4;
    }
    return nullptr;
}

any AsmGenerator::visitCompound_statement( CSubsetParser::Compound_statementContext *ctx){
    visitChildren(ctx);
    return nullptr;
}

any AsmGenerator::visitStatements(CSubsetParser::StatementsContext *ctx){
    if(ctx->statements()!=nullptr){
        visit(ctx->statements());
    }
    visit(ctx->statement());
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
    if(ctx->IF()!=nullptr && ctx->ELSE()!=nullptr){
        string elseLabel = newLabel();
        string endLabel = newLabel();
        visit(ctx->expression());
        emit("cmp eax,0");
        emit("je "+elseLabel);
        // if body
        visit(ctx->statement(0));
        emit("jmp "+endLabel);
       // else body
        emit(elseLabel+":");
        visit(ctx->statement(1));
        emit(endLabel+":");
        return nullptr;
    }
    if(ctx->IF()!=nullptr){
        string falseLabel = newLabel();
        visit(ctx->expression());
        emit("cmp eax,0");
        emit("je "+falseLabel);
        visit(ctx->statement(0));
        emit(falseLabel+":");
        return nullptr;
    }
    if(ctx->WHILE()!=nullptr){
        string startLabel=newLabel();
        string endLabel=newLabel();
        emit(startLabel+":");
        visit(ctx->expression());
        emit("cmp eax,0");
        emit("je "+endLabel);
        visit(ctx->statement(0));
        emit("jmp "+startLabel);
        emit(endLabel+":");
        return nullptr;
    }
    if(ctx->FOR()!=nullptr){
        string startLabel=newLabel();
        string endLabel=newLabel();
    // initialization
        visit(ctx->expression_statement(0));
        emit(startLabel+":");
    // condition
        visit(ctx->expression_statement(1));
        emit("cmp eax,0");
        emit("je "+endLabel);
    // body
        visit(ctx->statement(0));
    // increment
        visit(ctx->expression());
        emit("jmp "+startLabel);
        emit(endLabel+":");
        return nullptr;
    }
    visitChildren(ctx);
    return nullptr;
}

any AsmGenerator::visitExpression_statement(CSubsetParser::Expression_statementContext *ctx){
    if(ctx->expression()!=nullptr){
        visit(ctx->expression());
    }
    return nullptr;
}

any AsmGenerator::visitExpression( CSubsetParser::ExpressionContext *ctx){
    if(ctx->ASSIGNOP()!=nullptr){
        CSubsetParser::VariableContext *lhs = ctx->variable();
        if(lhs->expression() == nullptr){
            string name = lhs->ID()->getText();
            visit(ctx->logic_expression());
            emit("mov "+ getVariableLocation(name) + ",eax");
            return string("eax");
        }
        visit(ctx->logic_expression());
        emit("push eax");
        generateArrayAddress(lhs, "edx");
        emit("pop eax");
        emit("mov [edx],eax");
        return string("eax");
    }
    visitChildren(ctx);
    return string("eax");
}

any AsmGenerator::visitLogic_expression( CSubsetParser::Logic_expressionContext *ctx){
    if(ctx->LOGICOP()==nullptr){
        visit(ctx->rel_expression(0));
        return string("eax");
    }
    string op = ctx->LOGICOP()->getText();
    string trueLabel = newLabel();
    string falseLabel = newLabel();
    string endLabel = newLabel();
    if(op == "&&"){
        visit(ctx->rel_expression(0));
        emit("cmp eax,0");
        emit("je "+falseLabel);
        visit(ctx->rel_expression(1));
        emit("cmp eax,0");
        emit("je "+falseLabel);
        emit("mov eax,1");
        emit("jmp "+endLabel);
        emit(falseLabel + ":");
        emit("mov eax,0");
    }
    else if(op == "||"){
        visit(ctx->rel_expression(0));
        emit("cmp eax,0");
        emit("jne "+trueLabel);
        visit(ctx->rel_expression(1));
        emit("cmp eax,0");
        emit("jne "+trueLabel);
        emit("mov eax,0");
        emit("jmp "+endLabel);
        emit(trueLabel + ":");
        emit("mov eax,1");
    }
    emit(endLabel + ":");
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
    if(ctx->ID()!=nullptr){
        string fname = ctx->ID()->getText();
        int args=0;
        if(ctx->argument_list()!=nullptr){
            args = any_cast<int>(visit(ctx->argument_list()));
        }
        emit("call func_" + fname);
        if(args){
            emit("add esp,"+to_string(args*4));
        }
        return string("eax");
    }
    if(ctx->expression()!=nullptr){
        visit(ctx->expression());
        return string("eax");
    }
    else if(ctx->INCOP()){
        auto *var = ctx->variable();
        if(var->expression()==nullptr){
            string name = var->ID()->getText();
            emit("mov eax," + getVariableLocation(name));
            emit("push eax");
            emit("add eax,1");
            emit("mov "+ getVariableLocation(name) + ",eax");
            emit("pop eax");
            return string("eax");
        }
        generateArrayAddress(var , "edx");
        emit("mov eax,[edx]");
        emit("push eax");
        emit("add eax,1");
        emit("mov [edx],eax");
        emit("pop eax");
        return string("eax");
    }
    else if(ctx->DECOP()){
        auto *var = ctx->variable();
        // scalar x--
        if(var->expression()==nullptr){
            string name = var->ID()->getText();
            emit( "mov eax," + getVariableLocation(name) );
            emit("push eax");
            emit("sub eax,1");
            emit( "mov " + getVariableLocation(name) + ",eax" );
            emit("pop eax");
            return string("eax");
        }
        // array a[i]--
        generateArrayAddress( var,"edx");
        emit("mov eax,[edx]");
        emit("push eax");
        emit("sub eax,1");
        emit("mov [edx],eax");
        emit("pop eax");
        return string("eax");
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
        return visit(ctx->variable());
    }
    visitChildren(ctx);
    return nullptr;
}
void AsmGenerator::collectArguments( CSubsetParser::ArgumentsContext *ctx , vector<CSubsetParser::Logic_expressionContext*> &args){
    if(ctx->arguments()!=nullptr){
        collectArguments(ctx->arguments() , args);
    }
    if(ctx->logic_expression()!=nullptr){
        args.push_back(ctx->logic_expression());
    }
}

any AsmGenerator::visitArguments(CSubsetParser::ArgumentsContext *ctx){
    vector<CSubsetParser::Logic_expressionContext*> args;
    collectArguments(ctx, args);
    for(int i = (int)args.size()-1; i >= 0; i--){
        visit(args[i]);
        emit("push eax");
    }
    return (int)args.size();
}

any AsmGenerator::visitArgument_list( CSubsetParser::Argument_listContext *ctx ){
    if(ctx->arguments()!=nullptr){
        return visit(ctx->arguments());
    }
    return 0;
}
