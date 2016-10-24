#include "common.hpp"
#include "arvore_sintatica.hpp"
#include "parser.tab.h"

using namespace std;

int main(int argc, char *argv[])
{//função principal, usada para definir como será feita a chamada a partir dos terminal
  if (argc == 2)
  {
    abrirArq(string(argv[1]));
    string t = GetToken( );
    while (GetToken( )!="")
    {  cout<<t;  t = GetToken( );}
    fecharArq( );
  }
  else
  {
    cout<< "usages:" << endl;
    cout<< "lexer input_file_name"<< endl;

    return 0;
  }
  return(0);
}
