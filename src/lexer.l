%{//Primero Campo: Inclusão de Bibliotecas e definição de constantes-----------------------------------------------------

#include "common.hpp"
#include "arvore_sintatica.hpp"
#include "parser.tab.h"

char buffer[30];
extern YYSTYPE yylval;


using namespace std;

string text_id;

%}

%x MULT_LINE_COMMENT_STATE
%x ERROR_DETECTED

DIGITO [0-9]
LETRA [a-zA-Z]

%%


<INITIAL>"//".*"\n"                                linhas++;
<INITIAL>"/*"                                      {BEGIN(MULT_LINE_COMMENT_STATE);}
<INITIAL>\n                                        linhas++;
<INITIAL>\r\n                                        linhas++;
<INITIAL>\t|" "
<INITIAL>if		  		                            	return IF;
<INITIAL>else  	   		                            return ELSE;
<INITIAL>while					                          return WHILE;
<INITIAL>return				                            return RETURN;
<INITIAL>int						                          return INT;
<INITIAL>void					                            return VOID;
<INITIAL>"+"						                          return SOMA;
<INITIAL>"-"						                          return SUB;
<INITIAL>"*"						                          return MULT;
<INITIAL>"/"						                          return DIV;
<INITIAL>"!""="                                   return DIFERENTE;
<INITIAL>"=""="				                            return IGUAL;
<INITIAL>">""="				                            return MAIOR_IGUAL;
<INITIAL>"<""="				                            return MENOR_IGUAL;
<INITIAL>"="						                          return ATRIBUICAO;
<INITIAL>">"						                          return MAIOR;
<INITIAL>"<"						                          return MENOR;
<INITIAL>"("						                          return PARENTESES_ABRE;
<INITIAL>")"						                          return PARENTESES_FECHA;
<INITIAL>"["						                          return COLCHETES_ABRE;
<INITIAL>"]"						                          return COLCHETES_FECHA;
<INITIAL>"{"					                            return CHAVES_ABRE;
<INITIAL>"}"						                          return CHAVES_FECHA;
<INITIAL>;						                            return PONTO_VIRGULA;
<INITIAL>,                                        return VIRGULA;


<INITIAL>{LETRA}({LETRA}|{DIGITO})*			          { text_id = yytext; return ID;}
<INITIAL>{DIGITO}+					                      { return INT_NUM;}

<INITIAL>.                                        {BEGIN(ERROR_DETECTED); erro_txt = string(yytext);}



<ERROR_DETECTED>"\n"|"\r\n"                          {BEGIN(INITIAL); erro_lexico = true; return ERROR;}
<ERROR_DETECTED>(" "|"\t")                    {BEGIN(INITIAL); erro_lexico = true; return ERROR;}
<ERROR_DETECTED>(.)                           {erro_txt.append(yytext); }



<MULT_LINE_COMMENT_STATE>"*/"                 {BEGIN(INITIAL);}
<MULT_LINE_COMMENT_STATE>(.) ;
<MULT_LINE_COMMENT_STATE>(\n)|("\r\n")                 linhas++;
<MULT_LINE_COMMENT_STATE><<EOF>> { perror("Expecting \"*/.\" before end of file \n"); }



%%//Terceiro campo: Rotinas do Usuario-----------------------------------------------------------------------------------

int linhas;

int token;

std::string erro_txt;

FILE *f_in;

void abrirArq(string file_name)
{
  if(f_in = fopen(file_name.c_str(),"r"))
  {
      yyin = f_in;
  }
  else
    perror("lexer");
}

bool erro_lexico=false;

void fecharArq( ){fclose(f_in);}

bool EOFarq( ){return feof(f_in);}

string GetToken( )
{
  string temp;
  string return_token;

  int token=yylex();

  if(token!=0)
  {
    switch(token)
    {
      //Outros
      case INT_NUM: return_token = (string("INT_NUM\t") + string(yytext) + string("\n")); break;
      case FLOAT_NUM: return_token = (string("FLOAT_NUM\t") + string(yytext) + string("\n")); break;
      case ID: return_token = (string("ID\t") + string(yytext) + string("\t") + to_string(linhas) + string("\n")); break;

      //Palavra Reservadas
      case IF: return_token = (string("IF\n")); break;
      case ELSE: return_token = (string("ELSE\n")); break;
      case WHILE: return_token = (string("WHILE\n")); break;
      case RETURN: return_token = (string("RETURN\n")); break;
      case INT: return_token = (string("INT\n")); break;
      case FLOAT: return_token = (string("FLOAT\n")); break;
      case VOID: return_token = (string("VOID\n")); break;

      //Simbolos Especiais
      case SOMA: return_token = (string("SOMA\n")); break;
      case SUB: return_token = (string("SUB\n")); break;
      case MULT: return_token = (string("MULT\n")); break;
      case DIV: return_token = (string("DIV\n")); break;
      case ATRIBUICAO: return_token = (string("ATRIBUICAO\n")); break;
      case IGUAL: return_token = (string("IGUAL\n")); break;
      case MAIOR: return_token = (string("MAIOR\n")); break;
      case MENOR: return_token = (string("MENOR\n")); break;
      case MAIOR_IGUAL: return_token = (string("MAIOR_IGUAL\n")); break;
      case MENOR_IGUAL: return_token = (string("MENOR_IGUAL\n")); break;
      case PARENTESES_ABRE: return_token = (string("PARENTESES_ABRE\n")); break;
      case PARENTESES_FECHA: return_token = (string("PARENTESES_FECHA\n")); break;
      case COLCHETES_ABRE: return_token = (string("COLCHETES_ABRE\n")); break;
      case COLCHETES_FECHA: return_token = (string("COLCHETES_FECHA\n")); break;
      case CHAVES_ABRE: return_token = (string("CHAVES_ABRE\n")); break;
      case CHAVES_FECHA: return_token = (string("CHAVES_FECHA\n")); break;
      case PONTO_VIRGULA: return_token = (string("PONTO_VIRGULA\n")); break;
      case VIRGULA: return_token = (string("VIRGULA\n")); break;

      case ERROR: return_token = ("ERROR\t" +erro_txt+ "\t" + to_string(linhas) +"\n"); break;

    }

    return return_token;
  }
  else
    return "";
}
