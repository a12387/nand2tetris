#pragma once
#include "lexer.hpp"
using namespace std;

class Parser {
   public:
    Parser(deque<Token> &in, ofstream &out);
    void parse();
   private:
    int indent = 0;
    deque<Token> &tokens;
    ofstream &out;
    void error();
    void print(string s, int pos, bool recur);
    bool hasNextToken();
    bool compare(string tok);
    bool compare(TokenType type);
    void eat();
    void parseClass();
    void parseClassVarDec();
    void parseSubroutineDec();
};