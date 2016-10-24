#ifndef CODIGO_MAQUINA_H
#define CODIGO_MAQUINA_H

#include <tuple>

#include "common.hpp"
#include "tabela_de_simbolos.hpp"
#include "analisador_semantico.hpp"
#include "codigo_intermediario.hpp"



#define num  0
#define var  1
#define vet  3

//instruções do processador
#define Add   0
#define AddI  1
#define Sub   2
#define SubI  3
#define Mult  4
#define MultI 5

#define And   6
#define AndI  7
#define Or    8
#define OrI   9
#define Not   10
#define Slt   11
#define SltI  12

#define Sll   13
#define Slr   14

#define Move  15

#define J     16
#define Jr    17
//Jz
#define Beq   19
#define Bnq   20
#define Bob   21
#define Bos   22

#define Load  23
#define Store 24

#define Input  25
#define Output 26

#define SetR   27

#define End    31

//novas instruções
#define LoadR  28
#define StoreR 29
#define movePC 30

#define Div    18


class codigo_maquina
{
  private:
    vector<string> codigo[4];
    vector<string> *cod_intermediario;
    vector<int> pilha_ativacao_linha;
    vector<string> pilha_ativacao_escopo;


    vector<tuple<string,int>> label_lines;

    string escopo;

    size_t ponteiro_memoria_dados;

    void to_binary( );
    void push_back(string opcode, string arg1, string arg2, string arg3);

    int arg_type(string arg);

    void pesquisa_argumento1(int i);

    void interpret_argumento1(int i);
    void interpret_argumento2(int i);
    void interpret_2argumentos(int i);

    void interpret_declaracao(int i);
    void interpret_operacoes(int i);
    void interpret_comparacoes(int i);
    void interpret_atribuicao(int i);
    void interpret_temp(int i);
    void interpret_selecao(int i);
    void interpret_param(int i);
    void interpret_ativacao(int i);
    void interpret_retorno(int i);

  public:
    codigo_maquina( );

    codigo_maquina( codigo_intermediario intermediario );
    void read( codigo_intermediario cod );

    vector<string>* get_code();
    void print( );
};

#endif
