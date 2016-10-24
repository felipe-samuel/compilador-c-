#include "common.hpp"
#include "codigo_intermediario.hpp"
#include "codigo_maquina.hpp"
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

	bool no_error = false;

  if (yyparse()==0 && erro_sintatico == false)
		no_error = true;

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

	if(!tabela.no_errors())
	{
		cout<<endl<<endl;
		tabela.print_errors();

		no_error = false;
	}


	if(!analise_semantica.no_errors())
	{
		analise_semantica.print_errors( );
		no_error = false;
	}

	codigo_intermediario cod_i(*root);

	codigo_maquina cod_m(cod_i);

	if(no_error)
	{
		cout<<"Analise completa sem erros"<<endl;
		cod_m.print();
	}

	fecharArq();
  return 0;
}
