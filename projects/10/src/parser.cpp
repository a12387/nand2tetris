#include "parser.hpp"

#include "lexer.hpp"


using namespace std;

Parser::Parser(deque<Token> &in, ofstream &out) : tokens(in), out(out) {}
void Parser::error() { throw "Error Parsing"; }
void Parser::print(string s, int pos, bool recur) {
    if (pos == 1) {
        out << string(indent, ' ') << format("<{}> ", s);
        if (recur) {
            out << '\n';
            indent += 2;
        }
    } else if (pos == -1) {
        if (recur) {
            out << '\n';
            indent -= 2;
        }
        out << string(indent, ' ') << format("</{}>\n", s);
    } else if (pos == 0) {
        out << s << ' ';
    }
}
bool Parser::hasNextToken() { return !tokens.empty(); }
bool Parser::compare(string tok) { return tokens[0].lexval == tok; }
bool Parser::compare(TokenType type) { return tokens[0].type == type; }
void Parser::eat() {
    string s;
    switch (tokens[0].type) {
    case Keyword:
        s = "keyword";
        break;
    case Identifier:
        s = "identifier";
        break;
    case Symbol:
        s = "symbol";
        break;
    case IntegerConstant:
        s = "integerConstant";
        break;
    case StringConstant:
        s = "stringConstant";
        break;
    }
    print(s, 1, false);
    print(tokens[0].lexval, 0, false);
    print(s, -1, false);
    tokens.pop_front();
}
void Parser::parse() {
    while (hasNextToken()) {
        print("class", 1, true);
        parseClass();
        print("class", -1, true);
    }
}
void Parser::parseClass() {
    if(compare(Keyword) && compare("class")) {
        eat();
    } else {
        error();
    }

    if (compare(Identifier)) {
        eat();
    } else {
        error();
    }

    if (compare("{")) {
        eat();
    } else {
        error();
    }

    while(hasNextToken() && compare(Keyword)) {
        if(compare("field") || compare("static")) {
            print("classVarDec", 1, true);
            parseClassVarDec();
            print("classVarDec", -1, true);
        } else if(compare("constructor") || compare("method") || compare("function")) {
            print("subroutineDec", 1, true);
            parseSubroutineDec();
            print("subroutineDec", -1, true); 
        } else {
            break;
        }
    }

    if(compare("}")) {
        eat();
    } else {
        error();
    }
}

void Parser::parseClassVarDec() {
    if(compare(Keyword)) {
        if(compare("field")) {
            eat();
        } else if (compare("static")) {
            eat();
        } else {
            error();
        }
    } else {
        error();
    }

    if(compare(Identifier)) {
        eat();
    } else if (compare(Keyword)) {
        eat();
    } else {
        error();
    }

    while(true) {
        if(compare(Identifier)) {
            eat();
        } else {
            error();
        }

        if(compare(",")) {
            eat();
        } else if(compare(";")) {
            eat();
            break;
        } else {
            error();
        }
    }
}
void Parser::parseSubroutineDec() {
    if(compare(Keyword)) {

    }
}