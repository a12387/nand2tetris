#include "parser.hpp"

#include <string>

using namespace std;
void Parser::parse(const string &input, string &dest, string &comp,
                   string &jump) {
    string tmp = "";
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] == ' ') continue;  // Ignore spaces
        if (input[i] == '=') {
            dest = tmp;
            tmp = "";  // Reset tmp for next part
        } else if (input[i] == ';') {
            comp = tmp;
            jump = input.substr(i + 1);
            tmp = "";
            break;
        } else {
            tmp += input[i];
        }
    }
    if(tmp != "") {
        comp = tmp;
    }
}