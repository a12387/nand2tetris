#include "lexer.hpp"

#include <iterator>
#include <regex>

using namespace std;

Lexer::Lexer(ifstream &in) {
    input = string(istreambuf_iterator<char>(in), istreambuf_iterator<char>());
}
void Lexer::lex(vector<Token> &out) {
    while (!input.empty()) {
        smatch match;
        if (regex_search(input, match, whiteSpace)) {
            input = match.suffix().str();
            continue;
        }
        if (regex_search(input, match, comment)) {
            input = match.suffix().str();
            continue;
        }
        if (regex_search(input, match, blockComment)) {
            input = match.suffix().str();
            continue;
        }
        if (regex_search(input, match, keywords)) {
            out.push_back({.type = Keyword, .lexval = match.str()});
            input = match.suffix().str();
            continue;
        }
        if (regex_search(input, match, symbol)) {
            out.push_back({.type = Symbol, .lexval = match.str()});
            input = match.suffix().str();
            continue;
        }
        if (regex_search(input, match, intConstant)) {
            out.push_back({.type = IntegerConstant, .lexval = match.str()});
            input = match.suffix().str();
            continue;
        }
        if (regex_search(input, match, strConstant)) {
            out.push_back({.type = StringConstant, .lexval = match.str()});
            input = match.suffix().str();
            continue;
        }
        if (regex_search(input, match, identifier)) {
            out.push_back({.type = Identifier, .lexval = match.str()});
            input = match.suffix().str();
            continue;
        }
        throw "Syntax Error!";
    }
}