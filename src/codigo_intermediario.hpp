#ifndef CODIGO_INTERMEDIARIO_H
#define CODIGO_INTERMEDIARIO_H

#include <tuple>

#include "common.hpp"
#include "tabela_de_simbolos.hpp"
#include "arvore_sintatica.hpp"

/*
variaveis de escopo local terão seu nome concatenado com o escopos: "escopo"+"variavel"
*/

class codigo_intermediario
{
  private:
    vector<string> tres_enderecos[3];//0-operação 1-arg1 2-arg2

    void push_back(string op, string arg1, string arg2);

    string interpret_vetor(arvore no);
    void interpret_ativacao(arvore no);
    void interpret_operacoes(arvore no);
    void interpret_atribuicao(arvore no);
    void interpret_expressao(arvore no);
    void interpret_repeticao(arvore no);
    void interpret_selecao(arvore no);
    void interpret_retorno(arvore no);
    void interpret_statement(arvore no);


  public:
    codigo_intermediario( );
    codigo_intermediario(arvore no);
    void read(arvore no);
    vector<string>* get_code();
    void print( );
};

#endif
