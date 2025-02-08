#pragma once
#include <regex>
#include <fstream>
#include <deque>
using namespace std;
enum TokenType {
    Keyword,
    Identifier,
    Symbol,
    StringConstant,
    IntegerConstant,
};
struct Token {
    TokenType type;
    string lexval;
};
class Lexer {
   public:
    Lexer(ifstream &in);
    void lex(deque<Token> &out);

   private:
    string input;
    inline static regex keywords = regex(
        "^(class|constructor|field|static|let|function|method|var|int|char|"
        "boolean|void|true|false|null|this|if|else|while|return|do)");
    inline static regex symbol = regex(R"(^[{}()\[\].,;+\-*/&|<>=~])");
    inline static regex intConstant = regex("^(0|[1-9][0-9]*)");
    inline static regex strConstant = regex(R"(^"([^\\"]*(\\(\"|\\))?)*")");
    inline static regex identifier = regex("^[_a-zA-Z][0-9a-zA-Z_]*");
    inline static regex whiteSpace = regex(R"(^[ \r\t\n]+)");
    inline static regex comment = regex(R"(^//.*)");
    inline static regex blockComment = regex(R"(^/\*([^*]|\*+[^*/])*\*+/)");
};