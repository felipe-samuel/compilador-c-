#include "tabela_de_simbolos.hpp"

#define SIZE 211 //número primo para melhorar a distribuição
			// em uma tabela de hashing com até 211 registros
#define SHIFT 4

int tabela_de_simbolos::hashing(const char * key)
{
  int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  {
    temp = ((temp << SHIFT) + key[i]) % SIZE;
	// O perador << faz deslocamentos de bits para a esquerda
	// no valor de temp (4 bits).
	// Este recurso está sendo usado para evitar sobrecarga
	// em temp.
    ++i;
  }
  return temp;
}

tabela_de_simbolos::tabela_de_simbolos( ){ this->erros.push_back("erro semantico: não há função main") ; }//construtor

bool tabela_de_simbolos::insere(string id, int typeID, string typeDATA, string scop, string line, size_t vetor_tamanho, vector<string> param)
{//insere um determinado ID na tabela de simbolos
  int v,t,g,gv;
  bool no_error=true;
  vector<string> empty;

  if(typeID==FUN)
  {//função

    v = hashing(("global"+id).c_str());
    gv=0;

    for(int i=0; i<this->type_id[v].size();i++)
      if(this->names_id[v][i]==id)
      {
        this->erros.push_back("erro semantico: função "+ id +" já declarada como variavel global, linha " + line);
        no_error=false;

        gv=1;
        break;
      }

    if(gv == 0)
    {
      v=hashing(id.c_str());
      t=0;
      for(int i=0; i<this->type_id[v].size();i++)
        if(this->names_id[v][i]==id)
        {
          if(typeDATA=="" && this->type_id[v][i]==typeID)
            this->lines[v][i]=this->lines[v][i]+string(",")+line;
          else
          {
            this->erros.push_back("erro semantico: função "+ id +" já declarada, linha " + line);
            no_error=false;
          }
          t=1;
          break;
        }
      }

    if(t == 0 && gv == 0)
    {
      if(id=="output")
      {
        param.push_back("var");
        typeDATA="void";
      }
      else if(id=="input")
        typeDATA="int";


      if(typeDATA=="")
      {
        this->erros.push_back("erro semantico: uso de função não declarada, linha "+ line);
        return false;
      }

      this->names_id[v].push_back(id);
      this->type_id[v].push_back(typeID);
      this->type_data[v].push_back(typeDATA);
      this->escopo[v].push_back(scop);
      this->lines[v].push_back(line);
      this->parametros[v].push_back(param);
      this->vetor_tamanho[v].push_back(vetor_tamanho);
      this->posicao_memoria[v].push_back(-1); //-1 = não esta na memoria

      if(id=="main")
        this->erros.erase(this->erros.begin());

    }
  }
  else
  {//variavel e vetor

    if(typeDATA=="")//não é declaração
    {
      v=hashing(("global"+id).c_str());
      g=0;
      for(int i=0; i<this->type_id[v].size();i++)
        if(this->names_id[v][i]==id && this->escopo[v][i]=="global")
        {
          this->lines[v][i]=this->lines[v][i]+string(",")+line;
          g=1;
          break;
        }

      if(g==0)
      {
        v=hashing((scop+id).c_str());
        t=0;
        for(int i=0; i<this->type_id[v].size();i++)
          if(this->names_id[v][i]==id && this->escopo[v][i]==scop)
          {
            this->lines[v][i]=this->lines[v][i]+string(",")+line;
            t=1;
            break;
          }

        if(t==0)
        {
          this->erros.push_back("erro semantico: variavel "+ id +" não declarada, linha "+ line);
          no_error=false;
        }
      }
    }
    else//é declaracao
    {
      v=hashing(id.c_str());
      gv = 0;
      for(int i=0; i<this->type_id[v].size();i++)
        if(this->names_id[v][i]==id)
        {
          this->erros.push_back("erro semantico: variavel " + id + " já declarada como função, linha " + line);
          no_error=false;
          gv=1;
          break;
        }

      if(gv == 0)
      {
        v=hashing(("global"+id).c_str());
        g=0;
        for(int i=0; i<this->type_id[v].size();i++)
          if(this->names_id[v][i]==id && this->escopo[v][i]=="global")
          {
            this->erros.push_back("erro semantico: variavel " + id + " já declarada no escopo global, linha " + line);
            no_error=false;
            g=1;
            break;
          }
      }

      if(g==0 && gv ==0)
      {
        v=hashing((scop+id).c_str());
        t=0;
        for(int i=0; i<this->type_id[v].size();i++)
          if(this->names_id[v][i]==id && this->escopo[v][i]==scop)
          {
            this->erros.push_back("erro semantico: variavel "+ id +" já declarada, linha "+ line);
            no_error=false;
            t=1;
            break;
          }

        if(typeDATA == "void")
        {
          this->erros.push_back("erro semantico: variavel "+ id +" não pode ser do tipo VOID, linha "+ line);
          no_error=false;
          t=1;
        }
        else if(t==0)
        {         this->names_id[v].push_back(id);
                  this->type_id[v].push_back(typeID);
                  this->type_data[v].push_back(typeDATA);
                  this->escopo[v].push_back(scop);
                  this->lines[v].push_back(line);
                  this->parametros[v].push_back(empty);
                  this->vetor_tamanho[v].push_back(vetor_tamanho);
                  this->posicao_memoria[v].push_back(-1); //-1 = não esta na memoria
        }
      }
    }
  }

  return no_error;
}


tuple<int,int> tabela_de_simbolos::pesquisa_var(string id, string scop)
{
  size_t v=hashing(("global"+id).c_str());

  for(int i=0; i<this->type_id[v].size();i++)
    if(this->names_id[v][i]==id && this->escopo[v][i]=="global")
      return make_tuple(v,i);

  v=hashing((scop+id).c_str());

  for(int i=0; i<this->type_id[v].size();i++)
    if(this->names_id[v][i]==id && this->escopo[v][i]==scop)
      return make_tuple(v,i);

  return make_tuple(-1,-1);

}

tuple<int,int> tabela_de_simbolos::pesquisa_fun(string id)
{
  size_t v=hashing(id.c_str());

  for(int i=0; i<this->type_id[v].size();i++)
    if(this->names_id[v][i]==id && this->type_id[v][i]==FUN)
      return make_tuple(v,i);

  return make_tuple(-1,-1);
}


void tabela_de_simbolos::read(string file_name)
{//faz a leitura de um arquivo e cria a tabela de simbolos

  linhas=1;

  abrirArq(file_name);

  int declaracao=0;
  int chaves=0;
  int parenteses=0;
  int last_token=0;
  int second_last_token=0;
  int last_line=0;
  string declaracao_tipo="";
  string escopo="global";
  string last_fun="";
  string last_text="";

  do
  {
    token = yylex();

    if(chaves==0 && parenteses == 0 )
      escopo="global";
    else
      escopo=last_fun;

    if(last_token==ID)
    {

      switch (token)
      {
        case PARENTESES_ABRE://FUNÇÃO
          if (second_last_token==INT || second_last_token==FLOAT || second_last_token==VOID)//DECLARAÇÃO DE FUNÇÃO
          {
            last_fun=last_text;
            this->insere(last_text,FUN,declaracao_tipo, "" ,to_string(last_line),0);
          }
          else//CHAMADA DE FUNÇÃO
            this->insere(last_text,FUN,"", "" ,to_string(last_line),0);
        break;

        case COLCHETES_ABRE:
          if(declaracao==1)
            this->insere( last_text , VET , declaracao_tipo , escopo , to_string(last_line), 0 );
          else
            this->insere( last_text , VET , "" , escopo , to_string(last_line), 0 );
        break;

        default:
          if(declaracao==1)
            this->insere( last_text , VAR , declaracao_tipo , escopo , to_string(last_line), 0 );
          else
            this->insere( last_text , VAR , "" , escopo , to_string(last_line), 0 );
        break;
      }
    }

    switch(token)
		{
      case ID:
        last_text=string(yytext);
        last_line=linhas;
      break;

			case INT: declaracao=1; declaracao_tipo = string(yytext); break;
      case FLOAT: declaracao=1; declaracao_tipo = string(yytext); break;
      case VOID: declaracao=1; declaracao_tipo = string(yytext); break;

			case PARENTESES_ABRE: parenteses++; break;
			case PARENTESES_FECHA: parenteses--; declaracao=0; break;

			case CHAVES_ABRE: chaves++; break;
			case CHAVES_FECHA: chaves--; break;

    	case PONTO_VIRGULA: declaracao=0; break;

      case NEW_LINE:  linhas++; break;
		}

    second_last_token=last_token;
    last_token=token;

  }while (token!=0);


  int main_index = hashing("main");

  if(this->names_id[main_index].size() == 0)
    this->erros.push_back("erro semantico: programa não possue função main");

	fecharArq();
}

void tabela_de_simbolos::print( )//imprime a tabela de simbolos na tela
{
  string t;
  for(int i=0; i<211; i++)
  {
    for(int j=0;j<names_id[i].size();j++)
    {
      if(type_id[i][j]==FUN) t="fun";
      else if(type_id[i][j]==VET) t="vet";
      else t="var";

      cout<<"\t"<<i<<"\t"<<names_id[i][j]<<"\t"<<t<<"\t"<<type_data[i][j]<<"\t"<<escopo[i][j];

      if(t == "vet")
        cout<<"\t"<<vetor_tamanho[i][j];

      for(size_t k = 0; k<parametros[i][j].size(); k++)
        cout<<" "<<parametros[i][j][k];
      cout<<"\t"<<lines[i][j];
    }
    if(names_id[i].size()>0)
      cout<<"\n";
  }
}

void tabela_de_simbolos::print( string file_name )
{//imprime a tabela de simbolos em um arquivo
  ofstream writer;

  writer.open(file_name.c_str());

  for(int i=0; i<211; i++)
  {
    string t;
    for(int j=0;j<names_id[i].size();j++)
    {
      if(type_id[i][j]==0) t="fun";
      else if(type_id[i][j]==1) t="vet";
      else t="var";

      writer<<"\t"<<i<<"\t"<<names_id[i][j]<<"\t"<<t<<"\t"<<type_data[i][j]<<"\t"<<escopo[i][j]<<"\t"<<lines[i][j];
    }
    if(names_id[i].size()>0)
      writer<<"\n";
  }

  writer.close();
}

bool tabela_de_simbolos::no_errors( )
{
  if(this->erros.size() == 0)
    return true;
  else
    return false;
}

void tabela_de_simbolos::print_errors( )
{
  for (size_t i = 0; i < this->erros.size(); i++)
    cout<<this->erros[i]<<endl;
}

vector<string> tabela_de_simbolos::get_errors( )
{return this->erros;}
