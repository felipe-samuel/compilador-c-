list_of_tokens::list_of_tokens(  ){  };//construtor

bool list_of_tokens::read(string file_name)//função de leitura de um arquivo
{
  string temp;
  linhas=1;

  abrirArq(file_name);


  do
  {
    token=yylex();
		switch(token)
		{
			//Outros
			case INT_NUM: tokens.push_back(string("INT_NUM\t") + string(yytext) + string("\n")); break;
			case FLOAT_NUM: tokens.push_back(string("FLOAT_NUM\t") + string(yytext) + string("\n")); break;
			case ID: tokens.push_back(string("ID\t") + string(yytext) + string("\t") + to_string(linhas) + string("\n")); break;

			//Palavra Reservadas
			case IF: tokens.push_back(string("IF\n")); break;
			case ELSE: tokens.push_back(string("ELSE\n")); break;
			case WHILE: tokens.push_back(string("WHILE\n")); break;
			case RETURN: tokens.push_back(string("RETURN\n")); break;
			case INT: tokens.push_back(string("INT\n")); break;
			case FLOAT: tokens.push_back(string("FLOAT\n")); break;
			case VOID: tokens.push_back(string("VOID\n")); break;

			//Simbolos Especiais
			case SOMA: tokens.push_back(string("SOMA\n")); break;
			case SUB: tokens.push_back(string("SUB\n")); break;
			case MULT: tokens.push_back(string("MULT\n")); break;
			case DIV: tokens.push_back(string("DIV\n")); break;
			case ATRIBUICAO: tokens.push_back(string("ATRIBUICAO\n")); break;
			case IGUAL: tokens.push_back(string("IGUAL\n")); break;
			case MAIOR: tokens.push_back(string("MAIOR\n")); break;
			case MENOR: tokens.push_back(string("MENOR\n")); break;
			case MAIOR_IGUAL: tokens.push_back(string("MAIOR_IGUAL\n")); break;
			case MENOR_IGUAL: tokens.push_back(string("MENOR_IGUAL\n")); break;
			case PARENTESES_ABRE: tokens.push_back(string("PARENTESES_ABRE\n")); break;
			case PARENTESES_FECHA: tokens.push_back(string("PARENTESES_FECHA\n")); break;
			case COLCHETES_ABRE: tokens.push_back(string("COLCHETES_ABRE\n")); break;
			case COLCHETES_FECHA: tokens.push_back(string("COLCHETES_FECHA\n")); break;
			case CHAVES_ABRE: tokens.push_back(string("CHAVES_ABRE\n")); break;
			case CHAVES_FECHA: tokens.push_back(string("CHAVES_FECHA\n")); break;
			case PONTO_VIRGULA: tokens.push_back(string("PONTO_VIRGULA\n")); break;
      case VIRGULA: tokens.push_back(string("VIRGULA\n")); break;

			case ERROR: tokens.push_back("ERROR\t" +erro_txt+ "\t" + to_string(linhas) +"\n"); break;

		}
  }while (token!=0);


  if(errors.size()>0)
    return false;
  else
      return true;

  fecharArq();
}

string list_of_tokens::toString( )//coloca todos os tokens lidos em uma string
{
  string temp="";

  for(int i=0; i<tokens.size( ); i++)
    temp=temp+tokens[i];

  return temp;
}

void list_of_tokens::print(  ){ cout<<this->toString( );  }//imprime os token lidos na tela

void list_of_tokens::print( string file_name )//imprime os tokens lidos em um arquivo
{
  ofstream output;
  output.open(file_name.c_str());
  output<<this->toString( );
  output.close( );
}

void list_of_tokens::printERRORS(  )
{
  string temp="";

  for(int i=0; i<errors.size( ); i++)
    cout<<"erro no lexema: "<<errors[i]<<endl;
}
