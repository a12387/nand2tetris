#pragma once

#include <string>
using namespace std;

class Parser {
   public:
    Parser() = default;
    static void parse(const string &input, string &dest, string &comp, string &jump);
};