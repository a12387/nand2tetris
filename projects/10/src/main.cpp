#include "lexer.hpp"
#include <iostream>
#include <filesystem>
using namespace std;
using namespace std::filesystem;
int main(int argc, char **argv){
    if(argc != 2) {
        cerr << "Usage: " << argv[0] << " <file | directory>\n";
        exit(-1);
    }

    path p(argv[1]);
    deque<Token> out;
    string outName;
    if(is_regular_file(p)) {
        outName = p.stem().string();
        ifstream in(p, ios::in);
        Lexer lexer(in);
        lexer.lex(out);
        in.close();
    } else if(is_directory(p)) {
        outName = (p/".").parent_path().filename().string();
        for(auto const e: directory_iterator(p)) {
            if(e.path().extension() == ".jack") {
                ifstream in(e.path(), ios::in);
                Lexer lexer(in);
                lexer.lex(out);
                in.close();
            }
        }
    }
    cout << "Tokenized!\n";
    return 0;
}
