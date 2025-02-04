#include "coder.hpp"

#include <bitset>
#include <fstream>
#include <iostream>
using namespace std;
Coder::Coder(const string &path) {
    out.open(path, ios::out | ios::trunc);
    if (!out) {
        cerr << "Cannot open file: " << path << "!\n";
        exit(-1);
    }
}
void Coder::codeInstA(int addr) { out << 0 << bitset<15>(addr) << endl; }
void Coder::codeInstC(const string &dest, const string &comp,
                      const string &jump) {
    out << "111";
    out << compTable[comp];
    if (dest.find('A') != string::npos) {
        out << "1";
    } else {
        out << "0";
    }

    if (dest.find('D') != string::npos) {
        out << "1";
    } else {
        out << "0";
    }

    if (dest.find('M') != string::npos) {
        out << "1";
    } else {
        out << "0";
    }
    out << jumpTable[jump] << endl;
}