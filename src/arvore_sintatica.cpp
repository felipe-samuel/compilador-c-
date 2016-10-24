#include "arvore_sintatica.hpp"

using namespace std;

arvore::arvore( string value , string text ){ this->value = value; this->text = text; this->line = -1;}

arvore::arvore( string text ){ this->text = text; this->line = -1;}


arvore::arvore( string value , string text, int line ){ this->value = value; this->text = text; this->line = line;}

void arvore::set_value( string value ){ this->value = value; }

void arvore::inserir_filho( arvore filho ){ this->filhos.push_back(filho); }

void arvore::print_item( )
{

  if(this->filhos.size( ) == 0  and this->value != "")
    cout<<this->text<<" : "<<this->value<<": linha "<<this->line<<endl;
  else
    cout<<this->text<<endl;
}


void arvore::print( int identacao )
{
  for(int j=0; j<identacao; j++)
    cout<<"   ";

  this->print_item( );

  identacao++;

  for(int i=0; i<this->filhos.size( ); i++)
    this->filhos[i].print( identacao );
}


void arvore::print( )
{
  this->print_item( );

  for(int i=0; i<this->filhos.size( ); i++)
    this->filhos[i].print( 1 );
}

string arvore::get_value( ){ return this->value; }

string arvore::get_text( ){ return this->text; }

bool arvore::is_terminal( )
{
  if(this->filhos.size( )==0)
    return true;
  else
    return false;
}
