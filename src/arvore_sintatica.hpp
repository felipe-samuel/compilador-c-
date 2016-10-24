#ifndef ARVORE_SINTATICA_H
#define ARVORE_SINTATICA_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

class arvore
{
  public:
    string value;
    string text;
    int line;
    vector<arvore> filhos;

  public:
    arvore(string value, string text);
    arvore(string text);
    arvore(string value, string text, int line);
    void set_value(string value);
    void inserir_filho( arvore filho);
    void print_item( );
    void print( int i );
    void print( );

    string get_value( );
    string get_text( );
    bool is_terminal( );
};

#endif
