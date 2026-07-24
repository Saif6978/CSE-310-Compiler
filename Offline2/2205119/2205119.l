%option noyywrap

%{
    #include <iostream>
    #include <cstdio>
    #include <fstream>
    #include <string>
    #include "2205119_symbol_table.h"
    using namespace std;
    extern FILE *yyin;
    /*global variables here */
    SymbolTable table(7);
    int line_count = 1;
    int error_count = 0;

    ofstream logout;
    ofstream tokenout;

    void writeToken(string token, string lexeme)
    {
        tokenout << "<" << token;

        if(!lexeme.empty()){
            tokenout << ", " << lexeme;
        }
        tokenout << ">" << endl;
    }

    void writeLog(string token, string lexeme){
        logout << "Line no " << line_count << ": Token <" << token << "> Lexeme " << lexeme << " found" << endl;
        logout << endl;
    }
    void insertAndLogSymbol(string name, string type){
        streambuf* old_buf = cout.rdbuf(logout.rdbuf());

        bool inserted = table.insert_current_scope(name, type, true);
        if(inserted)
        {
            table.print_all_tables();
        }
        else{

        }
        cout << endl;
        cout.rdbuf(old_buf);
    }

%}




DIGIT    [0-9]
DIGITS   {DIGIT}+
EXP      [Ee][+-]?{DIGITS}
FLOAT    ({DIGITS}"."{DIGITS}|"."{DIGITS}|{DIGITS}{EXP}|{DIGITS}"."{DIGITS}{EXP}) 
COMMENT   "/*"([^*]|\n|\*+[^*/])*"*/"
UNCLOSED_COMMENT  "/*"([^*]|\n|\*+[^*/])*\*?
CHAR_CONST '([^'\n\\]|\\[ntr\\'"afbv0])'
INVALID_CHAR_CONST  '([^'\n\\]|\\.){2,}'
UNCLOSED_CHAR  '([^'\n\\]|\\.)*
STRING_LITERAL  \"([^"\n\\]|\\.|\\\r?\n)*\" 
UNCLOSED_STRING  \"([^"\n\\]|\\.|\\\r?\n)* 
LINECOMMENT  "//"([^\n]*"\\"\r?\n)*[^\n]*

%%

[ \r\t]+     {  /* do nothing */  }

\n       {  line_count++;  }

"if"        { writeToken("IF","");        writeLog("IF","if"); }
"else"      { writeToken("ELSE","");      writeLog("ELSE","else"); }
"for"       { writeToken("FOR","");       writeLog("FOR","for"); }
"while"     { writeToken("WHILE","");     writeLog("WHILE","while"); }
"do"        { writeToken("DO","");        writeLog("DO","do"); }
"break"     { writeToken("BREAK","");     writeLog("BREAK","break"); }

"int"       { writeToken("INT","");       writeLog("INT","int"); }
"char"      { writeToken("CHAR","");      writeLog("CHAR","char"); }
"float"     { writeToken("FLOAT","");     writeLog("FLOAT","float"); }
"double"    { writeToken("DOUBLE","");    writeLog("DOUBLE","double"); }
"void"      { writeToken("VOID","");      writeLog("VOID","void"); }

"return"    { writeToken("RETURN","");    writeLog("RETURN","return"); }
"switch"    { writeToken("SWITCH","");    writeLog("SWITCH","switch"); }
"case"      { writeToken("CASE","");      writeLog("CASE","case"); }
"default"   { writeToken("DEFAULT","");   writeLog("DEFAULT","default"); }
"continue"  { writeToken("CONTINUE","");  writeLog("CONTINUE","continue"); }

"goto"      { writeToken("GOTO","");      writeLog("GOTO","goto"); }
"long"      { writeToken("LONG","");      writeLog("LONG","long"); }
"short"     { writeToken("SHORT","");     writeLog("SHORT","short"); }
"static"    { writeToken("STATIC","");    writeLog("STATIC","static"); }
"unsigned"  { writeToken("UNSIGNED","");  writeLog("UNSIGNED","unsigned"); }

[a-zA-Z_][a-zA-Z0-9_]* {
    writeToken("ID",yytext);
    writeLog("ID",yytext);

    insertAndLogSymbol(yytext, "ID");
}

{FLOAT}  {
    writeToken("CONST_FLOAT",yytext);
    writeLog("CONST_FLOAT",yytext);

    insertAndLogSymbol(yytext, "CONST_FLOAT");
}

{DIGITS}  {
    writeToken("CONST_INT", yytext);
    writeLog("CONST_INT",yytext);

    insertAndLogSymbol(yytext, "CONST_INT");
}

{DIGITS}("."{DIGITS}){2,}  {
    error_count++;
    logout << "Error at line no " << line_count << ": Too many decimal points " << yytext << endl;
    logout << endl;
}

{DIGITS}{EXP}"."{DIGITS}  {
    error_count++;
    logout << "Error at line no " << line_count << ": Ill formed number " << yytext << endl;
    logout << endl;
}

{DIGITS}[a-zA-Z_][a-zA-Z0-9_]*  {
    error_count++;
    logout << "Error at line no " << line_count << ": Invalid prefix on ID or invalid suffix on Number " << yytext << endl;
    logout << endl;
}
{CHAR_CONST} {
    char value;
    string raw(yytext, 1, yyleng-2);

    if(raw[0] == '\\')
    {
        switch(raw[1])
        {
            case 'n':  value = '\n'; break;
            case 't':  value = '\t'; break;
            case 'r':  value = '\r'; break;
            case '\\': value = '\\'; break;
            case '\'': value = '\''; break;
            case '\"': value = '\"'; break;
            case 'a':  value = '\a'; break;
            case 'f':  value = '\f';  break;
            case 'b':  value = '\b'; break;
            case 'v':  value = '\v'; break;
            case '0':  value = '\0'; break;
            default:   value = raw[1];  break;
        }
    }
    else
    {
        value = raw[0];
    }

    string converted(1, value);

    
    writeToken("CONST_CHAR", converted);

    logout << "Line no " << line_count << ": Token <CONST_CHAR> Lexeme " << yytext
           << " found --> <CONST_CHAR, " << converted << ">" << endl;
    logout << endl;

    insertAndLogSymbol(yytext, "CONST_CHAR");
}

''  {
    error_count++;
    logout << "Error at line no " << line_count << ": Empty character constant error " << yytext << endl;
    logout << endl;
}

{INVALID_CHAR_CONST} {
    error_count++;
    logout << "Error at line no " << line_count << ": Multi character constant error " << yytext << endl;
    logout << endl;
}

{UNCLOSED_CHAR} {
    error_count++;
    logout << "Error at line no " << line_count << ": Unterminated character " << yytext << endl;
    logout << endl;
}


{LINECOMMENT}  {
    for(int i=0; yytext[i]!='\0'; i++)
    {
        if(yytext[i]=='\n')
            line_count++;
    }

    logout << "Line no " << line_count << ": Token <COMMENT> Lexeme " << yytext << " found" << endl;
    logout << endl;
}

{COMMENT} {
    for(int i=0; yytext[i]!='\0'; i++)
    {
        if(yytext[i]=='\n')
            line_count++;
    }

    logout << "Line no " << line_count << ": Token <COMMENT> Lexeme " << yytext << " found" << endl; 
    logout << endl;
}

{STRING_LITERAL} {
    string raw(yytext, 1, yyleng-2);   
    string lexeme;
    for(size_t i=0; i<raw.size(); i++)
    {
        if(raw[i]=='\\' && i+1<raw.size() && (raw[i+1]=='\n' || raw[i+1]=='\r'))
        {
            if(raw[i+1]=='\r') i++;   
            i++;                      
            line_count++;
        }
        else if(raw[i]=='\\' && i+1<raw.size())
        {
            char next = raw[i+1];
            switch(next)
            {
                case 'n': lexeme += '\n';  break;
                case 't': lexeme += '\t';  break;
                case '\\': lexeme += '\\'; break;
                case '\"': lexeme += '\"'; break;
                case 'a': lexeme += '\a';  break;
                case 'f': lexeme += '\f';  break;
                case 'r': lexeme += '\r';  break;
                case 'b': lexeme += '\b';  break;
                case 'v': lexeme += '\v';  break;
                case '0': lexeme += '\0';  break;
                default:  lexeme += next;  break;
            }
            i++;
        }
        else
        {
            lexeme += raw[i];
        }
    }

    writeToken("STRING", lexeme);
    logout << "Line no " << line_count << ": Token <STRING> Lexeme " << yytext << " found --> <STRING, " << lexeme << ">" << endl;
    logout << endl;
}

{UNCLOSED_STRING} {
    int start_line = line_count;
    for(int i=0; yytext[i]!='\0'; i++)
    {
        if(yytext[i]=='\n')
            line_count++;
    }
    error_count++;
    logout << "Error at line no " << start_line << ": Unterminated string " << yytext << endl;
    logout << endl;
}

{UNCLOSED_COMMENT}  {
    int start_line = line_count;
    for(int i=0; yytext[i]!='\0'; i++)
    {
        if(yytext[i]=='\n')
            line_count++;
    }
    error_count++;
    logout << "Error at line no " << start_line << ": Unterminated comment " << yytext << endl;
    logout << endl;
}

"++"|"--"  {
    writeToken("INCOP",yytext);
    writeLog("INCOP",yytext);
}

"="  {
    writeToken("ASSIGNOP" , yytext);
    writeLog("ASSIGNOP" , yytext);
}

"<="|">="|"=="|"!="|"<"|">"  {
    writeToken("RELOP", yytext);
    writeLog("RELOP", yytext);
}

"+"|"-"  {
    writeToken("ADDOP",yytext);
    writeLog("ADDOP",yytext);
}

"*"|"/"|"%"  {
    writeToken("MULOP",yytext);
    writeLog("MULOP",yytext);
}

"(" {
    writeToken("LPAREN", yytext);
    writeLog("LPAREN", yytext);
}

")" {
    writeToken("RPAREN", yytext);
    writeLog("RPAREN", yytext);
}

"{" {
    writeToken("LCURL", yytext);
    writeLog("LCURL", yytext);

    table.enter_scope();
}

"}" {
    writeToken("RCURL", yytext);
    writeLog("RCURL", yytext);

    table.exit_scope();
}

"[" {
    writeToken("LTHIRD", yytext);
    writeLog("LTHIRD", yytext);
}

"]" {
    writeToken("RTHIRD", yytext);
    writeLog("RTHIRD", yytext);
}

"," {
    writeToken("COMMA", yytext);
    writeLog("COMMA", yytext);
}

";" {
    writeToken("SEMICOLON", yytext);
    writeLog("SEMICOLON", yytext);
}

":" {
    writeToken("COLON", yytext);
    writeLog("COLON", yytext);
}

"&&"|"||" {
    writeToken("LOGICOP",yytext);
    writeLog("LOGICOP",yytext);
}

"!" {
    writeToken("NOT",yytext);
    writeLog("NOT",yytext);
}

. {
    error_count++;
    logout << "Error at line no " << line_count << ": Unrecognized character " << yytext << endl;
    logout << endl;
}

%%

int main(int argc, char *argv[]){
    if(argc < 2){
        cout << "Usage: ./lexer input.c\n";
        return 1;
    }
    yyin = fopen(argv[1],"r");

    if(yyin==nullptr){
        cout << "Cannot open input file\n";
        return 1;
    }
    logout.open("2205119_log.txt");
    tokenout.open("2205119_token.txt");

    yylex();
    {
        streambuf* old_buf = cout.rdbuf(logout.rdbuf());
        table.print_all_tables();
        cout.rdbuf(old_buf);
    }

    logout << "\nTotal lines: " << line_count << endl;
    logout << "Total errors: " << error_count << endl;
    
    logout.close();
    tokenout.close();
    
    fclose(yyin);

    return 0;
}



