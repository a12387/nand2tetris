#include <fstream>
#include <vector>
#include <iostream>
#include "coder.hpp"
#include "parser.hpp"
#include "symbol_table.hpp"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << format("Usage: {} <input file>\n", argv[0]);
        exit(-1);
    }

    ifstream in(argv[1], ios::in);
    if (!in) {
        cerr << format("Cannot open file: {} !\n", argv[1]);
        exit(-1);
    }

    SymbolTable::init();
    string s;
    int lineno;
    vector<string> insts;

    // First pass: Scan all labels
    lineno = 0;
    while (getline(in, s)) {
        size_t l;
        if (s.find_first_not_of(' ') == string::npos ||
            s.find('/') != string::npos) {
            continue;
        }
        if ((l = s.find('(')) != string::npos) {
            size_t r = s.find(')');
            if (r == string::npos) {
                cerr << "Syntax Error\n";
                exit(-1);
            }
            SymbolTable::insertLabel(s.substr(l + 1, r - l - 1), lineno);
        } else {
            insts.push_back(s);
            lineno++;
        }
    }
    in.close();

    // Second pass: generate code
    string pathOut(argv[1]);
    pathOut = pathOut.substr(0, pathOut.find_last_of('.')) + ".hack";
    Coder coder(pathOut);
    for (int i = 0; i < insts.size(); i++) {
        size_t h;
        if((h = insts[i].find('@') != string::npos)) {
            size_t t = insts[i].find_last_not_of(' ');
            string s = insts[i].substr(h, t - h + 1);
            int addr = atoi(s.c_str());
            if(addr == 0 && s[0] != '0') {
                addr = SymbolTable::find(s);
            }
            coder.codeInstA(addr);
        } else {
            string dest = "", comp = "", jump = "";
            Parser::parse(insts[i], dest, comp, jump);
            coder.codeInstC(dest, comp, jump);
        }
    }
    cout << "Complete!\n";
    return 0;
}