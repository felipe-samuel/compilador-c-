#include "common.hpp"
#include "arvore_sintatica.hpp"
#include "tabela_de_simbolos.hpp"
#include "analisador_semantico.hpp"

using namespace std;

int main(int argc, char *argv[])
{

	extern arvore *root;
	extern int linhas;
	linhas = 1;
	if(argc!=2)
	{
		cout<<"usage: parser"<<" <input-file-name>"<<endl;
		return 0;
	}

	erro_sintatico = false;

  abrirArq(string(argv[1]));
	cout<<"Análise sintatica iniciada"<<endl<<endl;


  if (yyparse()==0 && erro_sintatico == false)
	{
		cout<<endl<<"Análise sintática completa com sucesso"<<endl<<endl;
	}
	else if(yyparse()==0)
	{
		for (size_t i = 0; i < erros_sintaticos.size(); i++) {
			cout << erros_sintaticos[i] << endl;
		}
		cout<<endl<<"Análise sintática completa com erros"<<endl;
	}
	else
	{
		for (size_t i = 0; i < erros_sintaticos.size(); i++) {
			cout << erros_sintaticos[i] << endl;
		}
		cout<<"Analise sintatica não pode ser completada"<<endl;
	}


	root->print();

	cout<<"\n\n";

	tabela.print();

	if(!tabela.no_errors())
	{
		cout<<endl<<endl;
		tabela.print_errors();
	}

	analise_semantica.print_errors( );

	fecharArq();
  return 0;
}
