#ifndef ANALISADOR_SEMANTICO_H
#define ANALISADOR_SEMANTICO_H

#include "common.hpp"
#include "tabela_de_simbolos.hpp"

//erros semanticos a serem tratados:

//variavel não declarada - tabela_de_simbolos
//declaracao de variavel com void - tabela de simbolos
//variavel ou função já declarada - tabela de simbolos
//chamada de função não declarada - tabela de simbolos
//função main não declarada - tabela de simbolos
//variavel ja declarada como função  - tabela de simbolos
//função já declarada como variavel global - tabela de simbolos

//atribuição invalida, tipos diferenstes (retorno de função void e vetor-variavel) - analisador semantico
//numero de parametros corretos na chamada de função - analisador semantico
//parametros do tipo correto( retorno de função void e vetor-variavel) -  analisador semantico

//função void só pode ser "ativada sozinha"

//operações com vetores


class analisador_semantico
{
  private:
    vector<string> erros;

  public:
    analisador_semantico( );

    void verifica_tipos(arvore no, arvore *pai);
    void verifica_argumentos( arvore ativa );

    bool no_errors( );
    void print_errors( );
};

#endif
