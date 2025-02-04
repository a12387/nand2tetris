#include "symbol_table.hpp"

#include <iostream>
#include <string>

using namespace std;
void SymbolTable::init() {
    for (int i = 0; i < 16; i++) {
        string r = "R" + to_string(i);
        table[r] = i;
    }
    table["SCREEN"] = 0x4000;
    table["KBD"] = 0x6000;
    table["SP"] = 0;
    table["LCL"] = 1;
    table["ARG"] = 2;
    table["THIS"] = 3;
    table["THAT"] = 4;
}
void SymbolTable::insertLabel(const string &key, int line) {
    auto iter = table.find(key);
    if (iter != table.end()) {
        cerr << "Multi-definition of symbol: " << key << endl;
        exit(-1);
    }
    table[key] = line;
    return;
}
void SymbolTable::insertVar(const string &key) {
    // Guaranteed key not in table
    table[key] = ramCounter;
}
int SymbolTable::find(const string &key) {
    auto iter = table.find(key);
    if (iter == table.end()) {
        insertVar(key);
        return ramCounter++;
    } else {
        return iter->second;
    }
}