#include <iostream>
#include <string>
#include <sstream>
#include "SymbolTable.h"

using namespace std;

int main(int argc, char* argv[]) {
    // If input file argument is provided, redirect stdin
    if (argc >= 2) {
        if (freopen(argv[1], "r", stdin) == nullptr) {
            cerr << "Error: Cannot open input file: " << argv[1] << endl;
            return 1;
        }
    }
    // If output file argument is provided, redirect stdout
    if (argc >= 3) {
        if (freopen(argv[2], "w", stdout) == nullptr) {
            cerr << "Error: Cannot open output file: " << argv[2] << endl;
            return 1;
        }
    }

    int num_buckets;
    if (!(cin >> num_buckets) || num_buckets <= 0) {
        return 0;
    }

    SymbolTable* table = new SymbolTable(num_buckets, true);
    string line;
    int cmd_count = 0;

    while (getline(cin, line)) {
        // Strip trailing carriage return if input has Windows CRLF
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        istringstream iss(line);
        char command = '\0';
        if (!(iss >> command)) {
            continue; // Skip blank or whitespace-only lines safely
        }

        if (command == 'I') {
            string name, word, type;
            iss >> name;
            string headerText = "I";
            if (!name.empty()) headerText += " " + name;
            while (iss >> word) {
                if (!type.empty()) type += " ";
                type += word;
                headerText += " " + word;
            }
            cmd_count++;
            cout << "Cmd " << cmd_count << ": " << headerText << endl;
            if (name.empty() || type.empty()) {
                cout << "\tNumber of parameters mismatch for the command I" << endl;
            } else {
                table->insert_current_scope(name, type, true);
            }
        } else if (command == 'L') {
            string tok, name, headerText = "L";
            int n = 0;
            while (iss >> tok) {
                n++;
                if (n == 1) name = tok;
                headerText += " " + tok;
            }
            cmd_count++;
            cout << "Cmd " << cmd_count << ": " << headerText << endl;
            if (n != 1) {
                cout << "\tNumber of parameters mismatch for the command L" << endl;
            } else {
                table->lookup(name, true);
            }
        } else if (command == 'D') {
            string tok, name, headerText = "D";
            int n = 0;
            while (iss >> tok) {
                n++;
                if (n == 1) name = tok;
                headerText += " " + tok;
            }
            cmd_count++;
            cout << "Cmd " << cmd_count << ": " << headerText << endl;
            if (n != 1) {
                cout << "\tNumber of parameters mismatch for the command D" << endl;
            } else {
                table->remove_current_scope(name, true);
            }
        } else if (command == 'P') {
            char sub = '\0';
            iss >> sub;
            if (sub == 'A') {
                cmd_count++;
                cout << "Cmd " << cmd_count << ": P A" << endl;
                table->print_all_tables();
            } else if (sub == 'C') {
                cmd_count++;
                cout << "Cmd " << cmd_count << ": P C" << endl;
                table->print_this_table();
            }
        } else if (command == 'S') {
            cmd_count++;
            cout << "Cmd " << cmd_count << ": S" << endl;
            table->enter_scope(true);
        } else if (command == 'E') {
            if (table->getCurrent() != nullptr && table->getCurrent()->getParent() != nullptr) {
                cmd_count++;
                cout << "Cmd " << cmd_count << ": E" << endl;
                table->exit_scope();
            }
        } else if (command == 'Q') {
            cmd_count++;
            cout << "Cmd " << cmd_count << ": Q" << endl;
            break;
        }
    }

    delete table;
    return 0;
}
