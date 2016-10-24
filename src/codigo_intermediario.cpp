#include "codigo_intermediario.hpp"

codigo_intermediario::codigo_intermediario( ){ }

codigo_intermediario::codigo_intermediario( arvore no ){this->read(no);}

void codigo_intermediario::push_back(string op, string arg1, string arg2)
{
  this->tres_enderecos[0].push_back(op);
  this->tres_enderecos[1].push_back(arg1);
  this->tres_enderecos[2].push_back(arg2);
}

string codigo_intermediario::interpret_vetor(arvore no)
{
  string arg = "";

  if(no.filhos.size()>0)
  {
      this->interpret_expressao(no.filhos[1]);
      arg =  "["+to_string(tres_enderecos[0].size() - 1)+"]";
  }

  return no.filhos[0].value + arg;
}

void codigo_intermediario::interpret_ativacao(arvore no)
{
    size_t n_argumentos = no.filhos[1].filhos.size();

    vector<string> args;

    for(size_t i = 0 ; i < n_argumentos ; i++)
    {
      interpret_expressao(no.filhos[1].filhos[i]);
      args.push_back("("+to_string(tres_enderecos[0].size() - 1)+")");
    }

    for(size_t i = 0 ; i < args.size() ; i++)
      this->push_back("argumentos",args[i],"");


    this->push_back("ativacao", no.filhos[0].value,to_string(n_argumentos) );
}

void codigo_intermediario::interpret_operacoes(arvore no)
{
  bool arg1=false,arg2=false;

  if( no.text == "+" || no.text == "-" || no.text == "*" || no.text == "/" ||
      no.text == "==" || no.text == ">" || no.text == ">=" || no.text == "<"||
      no.text == "<=")
  {


    if(no.filhos[0].text!="var" && no.filhos[0].text!="vet" && no.filhos[0].text!="INT_NUM")
    {  this->interpret_operacoes(no.filhos[0]); arg1 = true; }

    if(no.filhos[1].text!="var" && no.filhos[1].text!="vet" && no.filhos[1].text!="INT_NUM")
    {  this->interpret_operacoes(no.filhos[1]); arg2 = true; }

    size_t index = tres_enderecos[0].size();
    string argumento1;
    string argumento2;


    if(arg1 == true)
    {
      if(arg2 == true)
        argumento1="("+to_string(index-2)+")";
      else
        argumento1="("+to_string(index-1)+")";
    }
    else
    {
        if(no.filhos[0].text=="INT_NUM")
          argumento1 = no.filhos[0].value;
        else if(no.filhos[0].text=="var")
          argumento1 = no.filhos[0].filhos[0].value;
        else if(no.filhos[0].text=="vet")
        {
          argumento1 = this->interpret_vetor(no.filhos[0]);
        }
    }

    if(arg2 == true)
      argumento2="("+to_string(index-1)+")";
    else
    {
        if(no.filhos[1].text=="INT_NUM")
          argumento2 = no.filhos[1].value;
        else if(no.filhos[1].text=="var")
          argumento2 = no.filhos[1].filhos[0].value;
        else if(no.filhos[1].text=="vet")
        {
          argumento2 =  this->interpret_vetor(no.filhos[1]);
        }
    }

    this->push_back(no.text, argumento1, argumento2);
  }
}

void codigo_intermediario::interpret_atribuicao(arvore no)
{
    size_t index = tres_enderecos[0].size();

    if( no.filhos[1].text == "=" )
    {
      this->interpret_atribuicao(no.filhos[1]);
      if(no.filhos[0].text == "vet")
      {
        this->push_back("atribuicao" , this->interpret_vetor(no.filhos[0])
                        , "("+to_string(index-1)+")" );
      }
      else
        this->push_back("atribuicao" , no.filhos[0].filhos[0].value
                        ,"("+to_string(index-1)+")" );

    }
    else if( no.filhos[1].text == "+" || no.filhos[1].text == "-" ||
        no.filhos[1].text == "*" || no.filhos[1].text == "/")
    {
      this->interpret_operacoes(no.filhos[1]);
      index = tres_enderecos[0].size();

      if(no.filhos[0].text == "vet")
      {
        this->push_back("atribuicao" , this->interpret_vetor(no.filhos[0])
                        , "("+to_string(index-1)+")" );
      }
      else
        this->push_back("atribuicao" , no.filhos[0].filhos[0].value
                        ,"("+to_string(index-1)+")" );
    }
    else if(no.filhos[1].text == "ativacao")
    {
      this->interpret_ativacao(no.filhos[1]);
      index = tres_enderecos[0].size();

      if(no.filhos[0].text == "vet")
      {
        this->push_back("atribuicao" , this->interpret_vetor(no.filhos[0])
                        , "("+to_string(index-1)+")" );
      }
      else
        this->push_back("atribuicao" , no.filhos[0].filhos[0].value
                        ,"("+to_string(index-1)+")" );
    }
    else
    {
      if(no.filhos[1].text=="INT_NUM")
      {
        if(no.filhos[0].text == "vet")
        {
          this->push_back("atribuicao" , this->interpret_vetor(no.filhos[0])
                          , no.filhos[1].value );
        }
        else
          this->push_back("atribuicao" , no.filhos[0].filhos[0].value
                          , no.filhos[1].value );
      }
      else if(no.filhos[1].text == "var")
      {
        if(no.filhos[0].text == "vet")
        {
          this->push_back("atribuicao" , this->interpret_vetor(no.filhos[0]) , no.filhos[1].filhos[0].value);
        }
        else
          this->push_back("atribuicao" , no.filhos[0].filhos[0].value
                          , no.filhos[1].filhos[0].value);
      }
      else if(no.filhos[1].text == "vet")
      {
        int index1 = this->tres_enderecos[0].size();

        if(no.filhos[0].text == "vet")
        {
          this->push_back("atribuicao" , this->interpret_vetor(no.filhos[0])
                          , this->interpret_vetor(no.filhos[1]));
        }
        else
          this->push_back("atribuicao" , no.filhos[0].filhos[0].value
                          , this->interpret_vetor(no.filhos[1]));
      }
    }
}

void codigo_intermediario::interpret_expressao(arvore no)
{
  size_t index = tres_enderecos[0].size();

//reconhecimento de atribuicao--------------------------------------------------
  if( no.text == "=" )
    this->interpret_atribuicao(no);
//------------------------------------------------------------------------------

//reconhecimento de operacoes aritimeticas
  else  if( no.text == "+" || no.text == "-" || no.text == "*" || no.text == "/" ||
          no.text == "==" || no.text == ">" || no.text == ">=" || no.text == "<"||
          no.text == "<=")
    this->interpret_operacoes(no);
//------------------------------------------------------------------------------

//reconhecimento de variaveis vetores, atribuicoes e numeros
  else  if( no.text == "var")
      this->push_back("var",no.filhos[0].value,"");
  else  if( no.text == "vet")
  {
      interpret_vetor(no);
      this->push_back("vetor",no.filhos[0].value+"["+no.filhos[1].value+"]","");
  }
  else  if( no.text == "INT_NUM")
      this->push_back("INT_NUM",no.value,"");
  else if(no.text == "ativacao")
      this->interpret_ativacao(no);
//------------------------------------------------------------------------------
}


void codigo_intermediario::interpret_repeticao(arvore no)
{
  int index_inicio = tres_enderecos[0].size();

  this->interpret_expressao(no.filhos[0]);

  int   index_repeticao = tres_enderecos[0].size();

  this->push_back("selecao","("+to_string( ( index_repeticao - 1) )+")","");

  this->interpret_statement(no.filhos[1]);

  this->push_back("salto", "("+to_string( index_inicio )+")","");

  tres_enderecos[2][index_repeticao] = "("+to_string( tres_enderecos[0].size() )+")";
}


void codigo_intermediario::interpret_selecao(arvore no)
{
  this->interpret_expressao(no.filhos[0]);

  int index_selecao = tres_enderecos[0].size();

  this->push_back("selecao","("+to_string( (index_selecao-1) )+")","");

  this->interpret_statement(no.filhos[1]);

  int index_salto = tres_enderecos[0].size();

  if(no.filhos.size()==3)//"if" com "else"
  {
    tres_enderecos[2][index_selecao] = "("+to_string( index_salto+1 )+")";
      this->push_back("salto","","");
      this->interpret_statement(no.filhos[2]);
      tres_enderecos[1][index_salto] = "("+to_string( tres_enderecos[0].size() )+")" ;
  }
  else
    tres_enderecos[2][index_selecao] = "("+to_string( index_salto )+")";
}

void codigo_intermediario::interpret_retorno(arvore no)
{
  string arg = "";

  if(no.filhos.size()>0)
  {
      this->interpret_expressao(no.filhos[0]);
      arg =  "("+to_string(tres_enderecos[0].size() - 1)+")";
  }

  this->push_back("retorno", arg ,"");
}

void codigo_intermediario::interpret_statement(arvore no)
{
  size_t index = tres_enderecos[0].size();

//reconhecimento de atribuicao--------------------------------------------------
  if( no.text == "=" )
    this->interpret_atribuicao(no);
//------------------------------------------------------------------------------

  else  if( no.text == "+" || no.text == "-" || no.text == "*" || no.text == "/" ||
          no.text == "==" || no.text == ">" || no.text == ">=" || no.text == "<"||
          no.text == "<=")
          this->interpret_operacoes(no);

//recinhecimento de if----------------------------------------------------------
  else if(no.text == "selecao")
    this->interpret_selecao(no);
//------------------------------------------------------------------------------


  else if(no.text == "iteracao")
    this->interpret_repeticao(no);

  else if(no.text == "return")
    this->interpret_retorno(no);

  else if(no.text == "ativacao")
    interpret_ativacao(no);


  else if( no.text == "declaracao-local" )
  {
    for (size_t i = 0; i < no.filhos.size() ; i++)
      if(no.filhos[i].text == "var-declaracaos")
      {
        for(size_t j = 0 ; j < no.filhos[i].filhos.size() ; j++ )
          if(no.filhos[i].filhos[j].text == "var-decl")
            this->push_back("declaracao",no.filhos[i].filhos[j].filhos[0].value,"1");
          else if(no.filhos[i].filhos[j].text == "vet-decl")
            this->push_back("declaracao",no.filhos[i].filhos[j].filhos[0].value,
                                         no.filhos[i].filhos[j].filhos[1].value);
      }
  }
  else if(no.text == "declaracao-composta" || "statement-lista")
    for(size_t i = 0 ; i < no.filhos.size() ; i++)
      this->interpret_statement(no.filhos[i]);

  //else
  //  this->interpret_expressao(no);
}

void codigo_intermediario::read( arvore no )
{
  for(size_t i = 0 ; i < no.filhos.size() ; i++)
    if(no.filhos[i].text == "fun-declaracao")
    {
      this->push_back("label",no.filhos[i].filhos[1].value,"");

      for (int j = no.filhos[i].filhos[2].filhos.size() -1; j>=0 ; j--) {
        this->push_back("param-decl", no.filhos[i].filhos[2].filhos[j].filhos[0].value, to_string(j+3));
      }

      this->interpret_statement(no.filhos[i].filhos[3]);
      this->push_back("label-end",no.filhos[i].filhos[1].value,"");
    }
    else if(no.filhos[i].text == "var-declaracaos")
    {

      for(size_t j = 0 ; j < no.filhos[i].filhos.size() ; j++ )
        if(no.filhos[i].filhos[j].text == "var-decl")
          this->push_back("declaracao",no.filhos[i].filhos[j].filhos[0].value,"1");
        else if(no.filhos[i].filhos[j].text == "vet-decl")
          this->push_back("declaracao",no.filhos[i].filhos[j].filhos[0].value,
                                       no.filhos[i].filhos[j].filhos[1].value);
    }


}

vector<string>* codigo_intermediario::get_code( ){return tres_enderecos;}

void codigo_intermediario::print( )
{
  for(size_t i=0; i<tres_enderecos[0].size(); i++)
      cout<<i<<"\t"<<tres_enderecos[0][i]<<"\t"<<tres_enderecos[1][i]<<"\t"<<tres_enderecos[2][i]<<endl;
}
