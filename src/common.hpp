#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>



//definição de constantes para a tabela de simbolos-----------------------------
#define FUN 200
#define VET 201
#define VAR 202

//------------------------------------------------------------------------------

void yyerror(char *);

extern std::string erro_txt;
extern int token;
extern bool erro_lexico;

extern int yylex();
extern void abrirArq(std::string file_name);
extern void fecharArq( );
extern std::string GetToken( );
extern bool EOFarq( );

extern char* yytext;
extern int linhas;

extern int yyparse();
extern bool erro_sintatico;

extern std::vector<std::string> erros_sintaticos;

extern std::string text_id;

extern std::string escopo;


class tabela_de_simbolos;
extern tabela_de_simbolos tabela;

class analisador_semantico;
extern analisador_semantico analise_semantica;



#endif
