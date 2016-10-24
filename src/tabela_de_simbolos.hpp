#ifndef TABELA_DE_SIMBOLOS_H
#define TABELA_DE_SIMBOLOS_H

#include "common.hpp"
#include "arvore_sintatica.hpp"
#include "parser.tab.h"
#include <tuple>

using namespace std;

class tabela_de_simbolos
{
  public:
    static int hashing(const char * key);

  public:
    vector<string> names_id[211];
    vector<int> type_id[211];
    vector<string> type_data[211];
    vector<string> escopo[211];
    vector<string> lines[211];
    vector<vector<string>> parametros[211];
    vector<size_t> vetor_tamanho[211];
    vector<int> posicao_memoria[211];

    vector<string> erros;

  public:
    tabela_de_simbolos( );
    bool insere(string id, int typeID, string typeDATA, string scop, string line,size_t vetor_tamanho, vector<string> param = vector<string>());

    tuple<int,int> pesquisa_var(string id, string scop);
    tuple<int,int> pesquisa_fun(string id);


    void read( string file_name );
    string toString();
    void print( );
    void print(string file_name );

    bool no_errors( );
    void print_errors( );
    vector<string> get_errors( );
};

#endif
