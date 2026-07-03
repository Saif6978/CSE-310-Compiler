#include<iostream>
#include<string>
#include<sstream>
#include "2205119_symbol_table.h"
using namespace std;

int ScopeTable::scope_count = 0;

int main(int argc,char* argv[]){
    if(argc<3){
        return 0;
    }
    if(freopen(argv[1],"r",stdin)==nullptr){
        cout<<"Cannot open inputfile"<<endl;
        return 0;
    }
    if(freopen(argv[2],"w",stdout)==nullptr){
        cout<< "Cannot open outputfile\n";
        return 0;
    }
    int num_buckets;
    cin>>num_buckets;
    cin.ignore();
    SymbolTable* table =new SymbolTable(num_buckets,true);
    string line;
    int cmd_count=0;
    while(getline(cin,line)){
        if(line.empty())continue;
        istringstream iss(line);
        char command;
        iss>>command;
        if(command=='I'){
            string name,word,type;
            iss>>name;
            string headerText="I";
            if(!name.empty())headerText+=" "+name;
            while(iss>>word){
                if(!type.empty())type+=" ";
                type+=word;
                headerText+=" "+word;
            }
            cmd_count++;
            cout << "Cmd "<<cmd_count<<": "<< headerText<<endl;
            table->insert_current_scope(name,type,true);

        }
        else if(command=='L'){
            string tok,name,headerText="L";
            int n=0;
            while(iss>>tok){
                n++;
                if(n==1)name=tok;
                headerText+=" "+tok;
            }
            cmd_count++;
            cout << "Cmd "<<cmd_count<<": "<<headerText<<endl;
            if(n!=1){
                cout << "\tNumber of parameters mismatch for the command L"<<endl;
            }
            else{
                table->lookup(name,true);
            }

        }
        else if(command =='D'){
            string tok,name,headerText="D";
            int n=0;
            while(iss>>tok){
                n++;
                if(n==1){
                    name=tok;
                }
                headerText+=" "+tok;
            }
            cmd_count++;
            cout << "Cmd "<<cmd_count<<": "<<headerText<<endl;
            if(n!=1){
                cout << "\tNumber of parameters mismatch for the command D"<< endl;
            }
            else{
                table->remove_current_scope(name,true);
            }
            

        }
        else if(command=='P'){
            char sub='\0';
            iss>>sub;
            if(sub=='A'){
                cmd_count++;
                cout << "Cmd "<<cmd_count<<": P A"<<endl;
                table->print_all_tables();
            
            }
            else if(sub=='C'){
                cmd_count++;
                cout << "Cmd "<<cmd_count<<": P C"<<endl;
                table->print_this_table();
            }

        }
        else if(command=='S'){
            cmd_count++;
            cout << "Cmd "<<cmd_count<<": S"<<endl;
            table->enter_scope(true);
        }
        else if(command=='E'){
            if(table->getCurrent()->getParent()!=nullptr){
                cmd_count++;
                cout << "Cmd "<<cmd_count<<": E"<<endl;
                table->exit_scope();
            }
        }
        else if(command=='Q'){
            cmd_count++;
            cout << "Cmd "<<cmd_count<<": Q"<<endl;
            break;
        }
    }
    delete table;
    fclose(stdin);
    fclose(stdout);
    return 0;
}