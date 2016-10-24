%{
#include "common.hpp"
#include "arvore_sintatica.hpp"
#include "tabela_de_simbolos.hpp"
#include "analisador_semantico.hpp"

arvore* root;
arvore* temp;
int last_var_type;

vector<int> save_line;
vector<string> save_text;
vector<string> fun_params;
vector<bool> expressao_decl;

string var_data_type;
string escopo = "global";


tabela_de_simbolos tabela;
analisador_semantico analise_semantica;

stringstream buf;

using namespace std;

%}

%union {
  arvore* node;
  double  value;
	char*  text;
}

%start programa

%nonassoc IFX

%token <text> ATRIBUICAO
%token <node> INT_NUM
%token <node> FLOAT_NUM
%token <node> ID

//Palavra Reservadas
%token <text>IF
%nonassoc <text>ELSE
%token <text>WHILE
%token <text>RETURN
%token <text>INT
%token <text>FLOAT
%token <text>VOID

//Simbolos Especiais
%token <text>SOMA
%token <text>SUB
%token <text>MULT
%token <text>DIV
%token <text>IGUAL
%token <text>MAIOR
%token <text>MENOR
%token <text>MAIOR_IGUAL
%token <text>MENOR_IGUAL
%token <text>DIFERENTE
%token <text>PARENTESES_ABRE
%token <text>PARENTESES_FECHA
%token <text>COLCHETES_ABRE
%token <text>COLCHETES_FECHA
%token <text>CHAVES_ABRE
%token <text>CHAVES_FECHA
%token <text>PONTO_VIRGULA

%token <text>VIRGULA

%token FIM
%token <text>NEW_LINE
%token <text>ERROR_START
%token <text>ERROR
%token <text>ERROR_END
%token <text>ERROR_END_NEW_LINE

%token MULT_LINE_COMMENT_BEGIN
%token MULT_LINE_COMMENT_END

%type <node> programa;
%type <node> declaracao;
%type <node> var-declaracao;
%type <node> fun-declaracao;

%type <node> tipo-especificador;

%type <node> params;
%type <node> param;

%type <text> relacional;

%type <node> mult-var-decl;
%type <node> var-decl;
%type <node> param-lista;
%type <node> composto-decl;
%type <node> local-declaracao;
%type <node> statement;
%type <node> statement-lista;
%type <node> expressao-decl;
%type <node> selecao-decl;
%type <node> iteracao-decl;
%type <node> retorno-decl;
%type <node> var;
%type <node> simples-expressao;
%type <node> soma-expressao;
%type <node> soma;
%type <node> termo;
%type <node> mult;
%type <node> fator;
%type <node> ativacao;
%type <node> arg-lista;
%type <node> expressao;

%type <node> args;

%%


programa: programa declaracao {$$ = $1; $$->inserir_filho(*$2); root = $$; } |
                  /*entrada vazia*/ {$$ = new arvore("","programa"); root = $$;};

declaracao: var-declaracao
          |fun-declaracao
          |error {$$ = new arvore("", "erro"); cout<<"erro\n\n";} ;

var-declaracao: tipo-especificador var-decl mult-var-decl PONTO_VIRGULA
                {
                    $$ = $3;
                    $$->inserir_filho( *$2 ); $$->inserir_filho(*$1);
                    buf.str(""); buf<<linhas;

                    for(int f = 0 ; f<$$->filhos.size()-1 ; f++)
                      if($$->filhos[f].text == "var-decl")
                        tabela.insere( $$->filhos[f].filhos[0].value, VAR, $1->value , escopo , buf.str(), 1 );
                      else if($$->filhos[f].text == "vet-decl")
                        tabela.insere( $$->filhos[f].filhos[0].value, VET, $1->value , escopo , buf.str(), stoi($$->filhos[f].filhos[1].value) );
                };

mult-var-decl: VIRGULA var-decl mult-var-decl { $$ = $3; $$->inserir_filho( *$2 ); }
              | /*entrada vazia*/ {$$ = new arvore("", "var-declaracaos");} ;

var-decl: ID {  $$ = new arvore( "" , "var-decl"); $$->inserir_filho( arvore(text_id, "ID", linhas) );}
          | ID
            COLCHETES_ABRE
            INT_NUM {save_text.push_back(yytext);}
            COLCHETES_FECHA
            { $$ = new arvore( "" , "vet-decl"); $$->inserir_filho( arvore(text_id, "ID", linhas)  );
              $$->inserir_filho( arvore(save_text.back(), "INT_NUM", linhas)  );
              save_text.pop_back();
            };

tipo-especificador: INT {$$ = new arvore("int" , "tipo", linhas);}
                    | FLOAT {$$ = new arvore("float" , "tipo", linhas);}
                    | VOID {$$ = new arvore("void" , "tipo", linhas);} ;

fun-declaracao: tipo-especificador ID
                {
                  escopo = text_id; save_text.push_back(text_id); save_line.push_back(linhas);
                }
                PARENTESES_ABRE params PARENTESES_FECHA
                {
                  buf.str(""); buf<<save_line.back();
                  tabela.insere(save_text.back() , FUN, $1->value , "global" , buf.str(), 0 , fun_params );

                }
                composto-decl
                {
                  escopo = "global";
                  $$ = new arvore( "" , "fun-declaracao"); $$->inserir_filho(*$1);
                  $$->inserir_filho( arvore(save_text.back(), "ID", save_line.back()) );
                  $$->inserir_filho(*$5); $$->inserir_filho(*$8);
                  save_text.pop_back(); save_line.pop_back(); fun_params.clear();}
							| tipo-especificador error composto-decl
                { $$ = new arvore("" , "fun-declaracao"); $$->inserir_filho(*$1);
                  $$->inserir_filho(arvore( "" , "erro" )); $$->inserir_filho(*$3);};

params: param param-lista { $$ = $2; $$->inserir_filho(*$1);}
      | VOID {$$ = new arvore("","lista_parametros"); }
      | /*entrada vazia*/ {$$ = new arvore("","lista_parametros");};

param-lista: VIRGULA param param-lista { $$ = $3; $$->inserir_filho(*$2); }
           | /* entrada vazia */ {$$ = new arvore("","lista_parametros");} ;

param: tipo-especificador ID {
                              $$ = new arvore("", "param-var");
                              $$->inserir_filho( arvore(text_id, "ID", linhas) );
                              $$->inserir_filho( *$1 );
                              fun_params.push_back("var");
                              buf.str(""); buf<<linhas;
                              tabela.insere( text_id , VAR, $1->value , escopo , to_string(linhas), 1 );
                             }

     | tipo-especificador ID
       COLCHETES_ABRE COLCHETES_FECHA
        {$$ = new arvore("", "param-vet"); $$->inserir_filho(*$1);
         $$->inserir_filho( arvore(text_id, "ID", linhas) ); fun_params.push_back("vet");
         tabela.insere( text_id , VET, $1->value , escopo , to_string(linhas), 1 );
        } ;

composto-decl: CHAVES_ABRE local-declaracao statement-lista CHAVES_FECHA
               {$$ = new arvore("","declaracao-composta");
                if(! $2->is_terminal() )
                  $$->inserir_filho(*$2);
                if(! $3->is_terminal() )
                  $$->inserir_filho(*$3);};

local-declaracao: local-declaracao var-declaracao {$$ = $1; $$->inserir_filho(*$2);}
                | local-declaracao error var-declaracao {$$ = $1; $$->inserir_filho(*$3);}
                |/* entrada vazia */ {$$ = new arvore("","declaracao-local");};

statement-lista: statement-lista statement {$$ = $1; $$->inserir_filho(*$2);}
               | statement-lista error statement {$$ = $1; $$->inserir_filho(*$3);}
               | /* entrada vazia */ {$$ = new arvore("","statement-lista");} ;

statement: expressao-decl | composto-decl | selecao-decl | iteracao-decl | retorno-decl ;

expressao-decl: expressao PONTO_VIRGULA {
                                          $$  = new arvore("", "expressao");
                                          $$->inserir_filho(*$1);
                                          analise_semantica.verifica_tipos(*$1, $$);
                                        }
              | PONTO_VIRGULA {$$  = new arvore("", "expressao"); } ;

selecao-decl: IF PARENTESES_ABRE expressao PARENTESES_FECHA statement %prec IFX
                {$$ = new arvore("","selecao"); $$->inserir_filho(*$3); $$->inserir_filho(*$5);
                 analise_semantica.verifica_tipos(*$3,new arvore("",""));}
            | IF PARENTESES_ABRE expressao PARENTESES_FECHA statement ELSE statement
                {$$ = new arvore("","selecao"); $$->inserir_filho(*$3);
                 $$->inserir_filho(*$5); $$->inserir_filho(*$7);
                 analise_semantica.verifica_tipos(*$3,new arvore("",""));};

iteracao-decl: WHILE PARENTESES_ABRE expressao PARENTESES_FECHA statement
                {$$ = new arvore("","iteracao"); $$->inserir_filho(*$3); $$->inserir_filho(*$5);
                 analise_semantica.verifica_tipos(*$3,new arvore("",""));};

retorno-decl: RETURN PONTO_VIRGULA {$$ = new arvore("VOID","return",linhas);}
            | RETURN expressao PONTO_VIRGULA
              {
                $$ = new arvore("","return"); $$->inserir_filho(*$2);
                analise_semantica.verifica_tipos(*$2,new arvore("",""));
              };

expressao: var ATRIBUICAO expressao
           {
             $$ = new arvore("","="); $$->inserir_filho(*$1);  $$->inserir_filho(*$3);
           }
         | simples-expressao
          {
            $$ = $1;
          };

var: ID {$$ = new arvore("","var"); $$->inserir_filho(arvore(text_id, "ID", linhas));
         buf.str(""); buf<<linhas;
         tabela.insere(text_id , VAR,"", escopo , buf.str(), 1 );
        }
   | ID {save_text.push_back(text_id); save_line.push_back(linhas);}
     COLCHETES_ABRE expressao COLCHETES_FECHA
        {$$ = new arvore("","vet");
        $$->inserir_filho(arvore(save_text.back(), "ID", save_line.back()));
        $$->inserir_filho(*$4);

        buf.str(""); buf<<save_line.back();
        tabela.insere(save_text.back() , VET,"", escopo , buf.str(), 1 );

        save_text.pop_back(); save_line.pop_back();

        analise_semantica.verifica_tipos(*$4,new arvore("",""));

      };

simples-expressao: soma-expressao relacional soma-expressao { $$ = new arvore("", $2);
                   $$->inserir_filho(*$1); $$->inserir_filho(*$3);}
                 | soma-expressao {$$ = $1;} ;

relacional: MAIOR_IGUAL {$$ = ">=";} | MAIOR {$$ = ">";} | MENOR {$$ = "<";} |
            MENOR_IGUAL {$$ = "<=";} | IGUAL {$$ = "==";} | DIFERENTE {$$ = "!=";} ;

soma-expressao: soma-expressao soma termo
                { $$ = $2; $$->inserir_filho(*$1); $$->inserir_filho(*$3);}
              | SOMA termo { $$ = new arvore("","+"); $$->inserir_filho(*$2); };
              | SUB  termo { $$ = new arvore("","-"); $$->inserir_filho(*$2); };
              | termo {$$ = $1; };

soma: SOMA {$$ = new arvore("","+");} | SUB {$$ = new arvore("","-");} ;

termo: termo mult fator { $$ = $2; $$->inserir_filho(*$1); $$->inserir_filho(*$3); }
     | fator {$$ = $1; } ;

mult: MULT {$$ = new arvore("","*");} | DIV {$$ = new arvore("","/");};

fator: PARENTESES_ABRE expressao PARENTESES_FECHA {$$ = $2;}
     | var {$$ = $1;}
     | ativacao {$$ = $1;}
     | INT_NUM {$$ = new arvore(yytext, "INT_NUM", linhas);};

ativacao: ID {save_text.push_back(text_id); save_line.push_back(linhas);}
          PARENTESES_ABRE args PARENTESES_FECHA

          {
          $$ = new arvore("","ativacao");
          $$->inserir_filho( arvore(save_text.back(), "ID", save_line.back()) );
          $$->inserir_filho( *$4 );

          buf.str(""); buf<<save_line.back();
          tabela.insere(save_text.back() , FUN ,"", "" , buf.str(), 0 );

          save_text.pop_back(); save_line.pop_back();

          analise_semantica.verifica_argumentos(*$$);
          };

args: arg-lista {$$ = $1;}
    | /* entrada vazia */ { $$ = new arvore("","argumentos"); } ;

arg-lista: arg-lista VIRGULA expressao {$$ = $1; $$->inserir_filho(*$3);}
         | expressao {$$ = new arvore("","argumentos"); $$->inserir_filho(*$1);} ;

%%

using namespace std;

bool erro_sintatico;

vector<string> erros_sintaticos;

void yyerror(char * msg)
{
	erro_sintatico = true;

	if(erro_lexico == true)
	{
		erro_lexico = false;
		erros_sintaticos.push_back("erro lexico: " + erro_txt + " linha: " + to_string(linhas));
	}
	else
		erros_sintaticos.push_back("erro sintatico: " + string(yytext) + " linha: " + to_string(linhas));
}
