#include "analisador_semantico.hpp"

#include <algorithm>
#include <stdexcept>

int ultima_linha;
string ultimo_vetor;

analisador_semantico::analisador_semantico(  ) { }

void analisador_semantico::verifica_argumentos( arvore ativa )
{
  arvore argumentos = ativa.filhos[1];

  //procura a função------------------------------------------------------------
  int i,j;
  tie(i,j) = tabela.pesquisa_fun(ativa.filhos[0].value);
  //----------------------------------------------------------------------------

  //verifica numero de argumentos-----------------------------------------------
  if(i==-1 && j==-1)
    return; //erro, função não encontrada na tabela de simbolos

  if(argumentos.filhos.size() != tabela.parametros[i][j].size())
  {
    erros.push_back("erro semantico: função " + ativa.filhos[0].value +
                    " chamada com número errado de parametros; linha "+
                    to_string(ativa.filhos[0].line));
    return;
  }
  //----------------------------------------------------------------------------


  //verifica tipo dos argumentos (variavel - vetor)-----------------------------
  int k,l;
  for(int f=0 ; f<argumentos.filhos.size() ; f++)
  {
    if(argumentos.filhos[f].text == "var" || argumentos.filhos[f].text == "vet")
      tie(k,l) = tabela.pesquisa_var(argumentos.filhos[f].filhos[0].value, escopo);

    if(k==-1 && l==-1)
      return; //erro, variavel não encontrada na tabela de simbolos

    string tipo;

    if(argumentos.filhos[f].text == "var")
    {
      if(tabela.type_id[k][l]==VAR)
        tipo = "var";
      else if(tabela.type_id[k][l]==VET)
        tipo = "vet";
      else
        tipo = "fun";
    }
    else
      tipo = "var";

    if(tabela.parametros[i][j][f] != tipo)
      erros.push_back("erro semantico: função " + ativa.filhos[0].value +
                      " chamada com parametro "+ to_string(f) +" do tipo errado, esperado: " +
                      tabela.parametros[i][j][f] +  " recebido: " +
                      tipo + "; linha "+
                      to_string(ativa.filhos[0].line));

  }
  //----------------------------------------------------------------------------

}

void analisador_semantico::verifica_tipos(arvore no , arvore *pai)
{
  //chamada de função com retorno void em contexto não permitido--------------
  if(pai!=NULL && pai->text!="expressao")
    if(no.text == "ativacao")
    {
      int i,j;
      tie(i,j) = tabela.pesquisa_fun(no.filhos[0].value);

      if(i==-1 && j==-1)
        return; //erro, função não encontrada na tabela de simbolos

      if(tabela.type_data[i][j] == "void")
        erros.push_back("erro semantico: chamada da função "+ no.filhos[0].value
        +" de retorno void em contexto não permitido; linha "+
        to_string(no.filhos[0].line));
    }
  //----------------------------------------------------------------------------

  //utilização de veotor em contexto não permitido------------------------------
  else if(pai!=NULL && pai->text!="argumentos")
  {
    if(no.text == "var")
    {
      int i,j;
      tie(i,j) = tabela.pesquisa_var(no.filhos[0].value, escopo);

      if(i==-1 && j==-1)
        return; //erro, variavel não encontrada na tabela de simbolos

      if(tabela.type_id[i][j] == VET)
        erros.push_back("erro semantico: utilização do vetor "+ no.filhos[0].value
        +" em contexto não permitido; linha "+
        to_string(no.filhos[0].line));
    }
  }
  //----------------------------------------------------------------------------

  //recursão para analisar a expressão inteira----------------------------------
  for( int i=0 ; i<no.filhos.size() ; i++ )
    verifica_tipos(no.filhos[i] , &no);
  //----------------------------------------------------------------------------

}

bool analisador_semantico::no_errors( )
{
  if(this->erros.size()==0)
    return true;
  else
    return false;
}


void analisador_semantico::print_errors( )
{
  for(int i=0 ; i<this->erros.size() ; i++)
    cout<<erros[i]<<endl;
}
