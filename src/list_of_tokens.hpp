#ifndef LIST_OF_TOKENS_H
#define LIST_OF_TOKENS_H

#include "common.hpp"
#include "parser.tab.h"

using namespace std;

class list_of_tokens
{
  private:
    vector<string> tokens;
    vector<string> errors;

  public:
    list_of_tokens(  );
    bool read(string file_name);
    string toString( );
    void print(  );
    void print( string file_name );
    void printERRORS();
};

#include "list_of_tokens.cpp"

#endif
