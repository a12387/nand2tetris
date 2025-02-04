#pragma once
#include <string>
#include <unordered_map>
using namespace std;

class SymbolTable {
   public:
    static void init();
    static void insertLabel(const string &key, int line);
    static int find(const string &key);

   private:
    static void insertVar(const string &key);
    inline static unordered_map<string, int> table = {};
    inline static int ramCounter = 16;
};