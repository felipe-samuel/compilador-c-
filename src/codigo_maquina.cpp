#include "codigo_maquina.hpp"

#include <bitset>

codigo_maquina::codigo_maquina( ){ }

codigo_maquina::codigo_maquina( codigo_intermediario intermediario )
  { this->read(intermediario); }

void codigo_maquina::to_binary( )
{
  string code;
  string complete_code;

  for(size_t i=0 ; i<this->codigo[0].size(); i++)
  {

    code = "RAM[8'b"+bitset<8>(i).to_string()+"]=32'b";
    //opcode
    if(this->codigo[0][i]=="Add")       code = code +  bitset<5>(0).to_string();
    else if(this->codigo[0][i]=="AddI") code = code +  bitset<5>(1).to_string();
    else if(this->codigo[0][i]=="Sub")  code = code +  bitset<5>(2).to_string();
    else if(this->codigo[0][i]=="SubI") code = code +  bitset<5>(3).to_string();
    else if(this->codigo[0][i]=="Mult") code = code +  bitset<5>(4).to_string();
    else if(this->codigo[0][i]=="MultI")code = code +  bitset<5>(5).to_string();

    else if(this->codigo[0][i]=="And")  code = code +  bitset<5>(6).to_string();
    else if(this->codigo[0][i]=="AndI") code = code +  bitset<5>(7).to_string();
    else if(this->codigo[0][i]=="Or")   code = code +  bitset<5>(8).to_string();
    else if(this->codigo[0][i]=="OrI")  code = code +  bitset<5>(9).to_string();
    else if(this->codigo[0][i]=="Not")  code = code +  bitset<5>(10).to_string();
    else if(this->codigo[0][i]=="Slt")  code = code +  bitset<5>(11).to_string();
    else if(this->codigo[0][i]=="SltI") code = code +  bitset<5>(12).to_string();

    else if(this->codigo[0][i]=="Sll") code = code +  bitset<5>(13).to_string();
    else if(this->codigo[0][i]=="Slr") code = code +  bitset<5>(14).to_string();

    else if(this->codigo[0][i]=="Move") code = code +  bitset<5>(15).to_string();

    else if(this->codigo[0][i]=="J")   code = code +  bitset<5>(16).to_string();
    else if(this->codigo[0][i]=="Jr")  code = code +  bitset<5>(17).to_string();
    //Jz
    else if(this->codigo[0][i]=="Beq") code = code +  bitset<5>(19).to_string();
    else if(this->codigo[0][i]=="Bnq") code = code +  bitset<5>(20).to_string();
    else if(this->codigo[0][i]=="Bob") code = code +  bitset<5>(21).to_string();
    else if(this->codigo[0][i]=="Bos") code = code +  bitset<5>(22).to_string();

    else if(this->codigo[0][i]=="Load") code = code +  bitset<5>(23).to_string();
    else if(this->codigo[0][i]=="Store") code = code +  bitset<5>(24).to_string();

    else if(this->codigo[0][i]=="Input") code = code +  bitset<5>(25).to_string();
    else if(this->codigo[0][i]=="Output") code = code +  bitset<5>(26).to_string();

    else if(this->codigo[0][i]=="SetR") code = code +  bitset<5>(27).to_string();

    else if(this->codigo[0][i]=="End") code = code +  bitset<5>(31).to_string();

    //novas instruções
    else if(this->codigo[0][i]=="LoadR") code = code +  bitset<5>(28).to_string();
    else if(this->codigo[0][i]=="StoreR") code = code +  bitset<5>(29).to_string();
    else if(this->codigo[0][i]=="MovePC") code = code +  bitset<5>(30).to_string();

    else if(this->codigo[0][i]=="Div") code = code +  bitset<5>(18).to_string();

    //--------------------------------------------------------------------------

    //interpreta argumentos

    for(size_t j = 1 ; j < 4; j++)
    {
      if(this->codigo[j][i]!="")
      {
        string number_size = this->codigo[j][i].substr(0, 2);

        string number_text = this->codigo[j][i].substr(3);

        int number = stoi(number_text);

        if(number_size == "05")
          code = code + bitset<5>(number).to_string();
        else if(number_size == "17")
            code = code + bitset<17>(number).to_string();
        else if(number_size == "22")
            code = code + bitset<22>(number).to_string();

      }
    }

    for (size_t j = code.size(); j < 53; j++) {
      code = code + "0";
    }

    code = code + ";\n";
    complete_code = complete_code + code;
  }

  cout<<complete_code<<"\n\n\n";
}

void codigo_maquina::push_back(string opcode, string arg1, string arg2, string arg3)
{
  this->codigo[0].push_back(opcode);
  this->codigo[1].push_back(arg1);
  this->codigo[2].push_back(arg2);
  this->codigo[3].push_back(arg3);
}

int codigo_maquina::arg_type(string arg)
{
  //0 - numero  1 - variavel 3 - vetor
  if(arg[0] > 47 && arg[0] < 58)//numeros na tabela ASCII
    return num;

  int pos = arg.find_first_of("[");

  if(pos == string::npos)
    return var;
  else
    return vet;
}

void codigo_maquina::interpret_declaracao(int i)
{
  int j,k;

  tie(j,k) = tabela.pesquisa_var( cod_intermediario[1][i] , this->escopo);

  if(j==-1)
  {
    cout<<cod_intermediario[1][i]<<"\n1\n\n";
    tabela.print();
    std::cout <<  std::endl;
    throw "variavel não encontrada";
  }

  tabela.posicao_memoria[j][k] = this->ponteiro_memoria_dados;
  this->ponteiro_memoria_dados += tabela.vetor_tamanho[j][k];

  this->push_back("AddI","05b3","05b1","17b"+to_string(tabela.vetor_tamanho[j][k]));
  this->push_back("Move","05b1","05b3","17b0");
}

void codigo_maquina::pesquisa_argumento1(int i)
{
  int j , k, m, n;

  //define os operadores----------------------------------------------------
  int arg_type1 = this->arg_type(cod_intermediario[1][i]);

  switch (arg_type1)
  {
    case num:
      throw "erro: atribuição a valor constante";
    break;

    case var:
      tie(j,k) = tabela.pesquisa_var( cod_intermediario[1][i] , this->escopo);

      if(j==-1)
      {
        cout<<cod_intermediario[1][i]<<"\n2\n";
        tabela.print();
        std::cout <<  std::endl;
        throw "variavel não encontrada";
      }


      this->push_back( "AddI",  "05b2", "05b0" , "17b" +
                        to_string(tabela.posicao_memoria[j][k]) );

    break;

    case vet:
      int position =  cod_intermediario[1][i].find_first_of("[");

      string vet_name  = cod_intermediario[1][i].substr(0, position);
      string vet_index = cod_intermediario[1][i].substr( position+1,
                         cod_intermediario[1][i].size()-2-position);

      tie(j,k) = tabela.pesquisa_var( vet_name , this->escopo);
      tie(m,n) = tabela.pesquisa_var( "("+vet_index+")" , this->escopo);

      if(j==-1 )
      {
        cout<<vet_name<<"\n3\n";
        tabela.print();
        std::cout << std::endl;
        throw "variavel não encontrada";
      }

      if(m==-1)
      {
        cout<<vet_index<<"\n4\n";
        tabela.print();
        std::cout <<  std::endl;
        throw "variavel não encontrada";
      }



      this->push_back( "AddI",  "05b3", "05b0" , "17b" +
                        to_string(tabela.posicao_memoria[m][n]) );//encontra posição de memoria do index do vetor

      this->push_back( "LoadR", "05b2", "05b3" , "17b0" );//carrega o valor de desse index r2


      this->push_back( "Add",  "05b3", "05b0" , "05b2" );//soma o index com a ativação em r3


      this->push_back( "AddI",  "05b2", "05b3" ,"17b" +
                        to_string(tabela.posicao_memoria[j][k]) );//soma r3 com a posição do vetor acessada


    break;
  }
}

void codigo_maquina::interpret_argumento1(int i)
{
  int j , k, m, n;

  //define os operadores----------------------------------------------------
  int arg_type1 = this->arg_type(cod_intermediario[1][i]);

  switch (arg_type1)
  {
    case num:
      this->push_back( "SetR" , "05b4", "22b" + cod_intermediario[1][i], "" );
    break;

    case var:
      tie(j,k) = tabela.pesquisa_var( cod_intermediario[1][i] , this->escopo);

      if(j==-1)
      {
        cout<<cod_intermediario[1][i]<<"\n8\n";
        tabela.print();
        std::cout <<  std::endl;
        throw "variavel não encontrada";
      }


      this->push_back( "AddI",  "05b3", "05b0" , "17b" +
                        to_string(tabela.posicao_memoria[j][k]) );
      this->push_back( "LoadR", "05b4", "05b3" , "17b0" );

    break;

    case vet:
      int position =  cod_intermediario[1][i].find_first_of("[");

      string vet_name  = cod_intermediario[1][i].substr(0, position);
      string vet_index = cod_intermediario[1][i].substr( position+1,
                         cod_intermediario[1][i].size()-2-position);

      tie(j,k) = tabela.pesquisa_var( vet_name , this->escopo);
      tie(m,n) = tabela.pesquisa_var( "("+vet_index+")" , this->escopo);

      if(j==-1 )
      {
        cout<<vet_name<<"\n9\n";
        tabela.print();
        std::cout <<  std::endl;
        throw "variavel não encontrada";
      }

      if(m==-1)
      {
        cout<<vet_index<<"\n10\n";
        tabela.print();
        std::cout <<  std::endl;
        throw "variavel não encontrada";
      }



      this->push_back( "AddI",  "05b3", "05b0" , "17b" +
                        to_string(tabela.posicao_memoria[m][n]) );//encontra posição de memoria do index do vetor

      this->push_back( "LoadR", "05b2", "05b3" , "17b0" );//carrega o valor de desse index r2


      this->push_back( "Add",  "05b3", "05b0" , "05b2" );//soma o index com a ativação em r3


      this->push_back( "AddI",  "05b2", "05b3" ,"17b" +
                        to_string(tabela.posicao_memoria[j][k]) );//soma r3 com a posição do vetor acessada

      this->push_back( "LoadR", "05b4", "05b2" , "17b0" );//carrega em r4 o conteudo

    break;
  }
}

void codigo_maquina::interpret_argumento2(int i)
{
  int j , k, m, n;

  //define os operadores----------------------------------------------------
  int arg_type2 = this->arg_type(cod_intermediario[2][i]);

  switch (arg_type2)
  {
    case num:
      this->push_back( "SetR" , "05b5", "22b" + cod_intermediario[2][i], "" );
    break;

    case var:
      tie(j,k) = tabela.pesquisa_var( cod_intermediario[2][i] , this->escopo);

      if(j==-1)
      {
        cout<<cod_intermediario[2][i]<<"\n11\n";
        tabela.print();
        std::cout <<  std::endl;
        throw "variavel não encontrada";
      }



      this->push_back( "AddI",  "05b3", "05b0" , "17b" +
                        to_string(tabela.posicao_memoria[j][k]) );
      this->push_back( "LoadR", "05b5", "05b3" , "17b0" );

    break;

    case vet:
      int position =  cod_intermediario[2][i].find_first_of("[");

      string vet_name  = cod_intermediario[2][i].substr(0, position);
      string vet_index = cod_intermediario[2][i].substr( position+1,
                         cod_intermediario[2][i].size()-2-position);

      tie(j,k) = tabela.pesquisa_var( vet_name , this->escopo);
      tie(m,n) = tabela.pesquisa_var( "("+vet_index+")" , this->escopo);

      this->push_back( "AddI",  "05b3", "05b0" , "17b" +
                        to_string(tabela.posicao_memoria[m][n]) );//encontra posição de memoria do index do vetor

      this->push_back( "LoadR", "05b2", "05b3" , "17b0" );//carrega o valor de desse index r2


      this->push_back( "Add",  "05b3", "05b0" , "05b2" );//soma o index com a ativação em r3


      this->push_back( "AddI",  "05b2", "05b3" ,"17b" +
                        to_string(tabela.posicao_memoria[j][k]) );//soma r3 com a posição do vetor acessada

      this->push_back( "LoadR", "05b5", "05b2" , "17b0" );//carrega em r4 o conteudo

    break;
  }
}

void codigo_maquina::interpret_2argumentos(int i)
{
  this->interpret_argumento1(i);
  this->interpret_argumento2(i);
}

void codigo_maquina::interpret_operacoes(int i)
{
  int j , k, m, n;

  this->interpret_2argumentos(i);

  //define a operação realizada---------------------------------------------
  string operacao;
  if(cod_intermediario[0][i] == "+")
    operacao = "Add";
  else if(cod_intermediario[0][i] == "-")
    operacao = "Sub";
  else if(cod_intermediario[0][i] == "*")
    operacao = "Mult";
  else if(cod_intermediario[0][i] == "/")
    operacao = "Div";
  else if(cod_intermediario[0][i] == ">")
    operacao = "Smt";
  else if(cod_intermediario[0][i] == "<")
    operacao = "Slt";
  //------------------------------------------------------------------------

  //salva variavel temporaria na tabela de simbolos-------------------------
  string id_temp = "("+ to_string(i) + ")";

  tabela.insere( id_temp , VAR, "int" , this->escopo , "-1" , 1 );
  tie(j,k) = tabela.pesquisa_var( id_temp , this->escopo);
  tabela.posicao_memoria[j][k] = this->ponteiro_memoria_dados;
  this->ponteiro_memoria_dados += tabela.vetor_tamanho[j][k];
  //------------------------------------------------------------------------

  //realiza a operação e salva na memoria-----------------------------------
  this->push_back( operacao , "05b6", "05b4", "05b5");

  this->push_back( "AddI",  "05b3", "05b0" ,"17b" +
                    to_string(tabela.posicao_memoria[j][k]) );//soma r3 com a posição do vetor acessada

  this->push_back( "StoreR","05b6", "05b3", "17b0");

  this->push_back("AddI","05b3","05b1","17b1");
  this->push_back("Move","05b1","05b3","17b0");

  //------------------------------------------------------------------------
}

void codigo_maquina::interpret_comparacoes(int i)
{
  int j , k, m, n;

  this->interpret_2argumentos(i);

  //define a operação realizada---------------------------------------------
  string operacao;
  if(cod_intermediario[0][i] == "==")
    operacao = "Beq";
  else if(cod_intermediario[0][i] == ">")
    operacao = "Bob";
  else if(cod_intermediario[0][i] == "<")
    operacao = "Bos";
  else if(cod_intermediario[0][i] == "!=")
    operacao = "Bnq";
  //------------------------------------------------------------------------

  //salva variavel temporaria na tabela de simbolos-------------------------
  string id_temp = "("+ to_string(i) + ")";
  tabela.insere( id_temp , VAR, "int" , this->escopo , "-1" , 1 );
  tie(j,k) = tabela.pesquisa_var( id_temp , this->escopo);
  tabela.posicao_memoria[j][k] = this->ponteiro_memoria_dados;
  this->ponteiro_memoria_dados += tabela.vetor_tamanho[j][k];
  //------------------------------------------------------------------------


  //realiza a operação e salva na memoria-----------------------------------
  this->push_back( "SetR" , "05b6", "22b1" , "");
  this->push_back( operacao , "05b4", "05b5" , "17b2");
  this->push_back( "SetR" , "05b6", "22b0" , "");


  this->push_back( "AddI",  "05b3", "05b0" ,"17b" +
                    to_string(tabela.posicao_memoria[j][k]) );//soma r3 com a posição do vetor acessada

  this->push_back( "StoreR","05b6", "05b3", "17b0");
  this->push_back("AddI","05b3","05b1","17b1");
  this->push_back("Move","05b1","05b3","17b0");
  //------------------------------------------------------------------------
}

void codigo_maquina::interpret_atribuicao(int i)
{
  this->interpret_argumento2(i);
  this->pesquisa_argumento1(i);

  this->push_back( "StoreR" , "05b5", "05b2", "17b0");
}

void codigo_maquina::interpret_temp(int i)
{
  int j,k;

  //salva variavel temporaria na tabela de simbolos-------------------------
  string id_temp = "("+ to_string(i) + ")";

  tabela.insere( id_temp , VAR, "int" , this->escopo , "-1" , 1 );
  tie(j,k) = tabela.pesquisa_var( id_temp , this->escopo);
  tabela.posicao_memoria[j][k] = this->ponteiro_memoria_dados;
  this->ponteiro_memoria_dados += tabela.vetor_tamanho[j][k];
  //------------------------------------------------------------------------

  this->interpret_argumento1(i);

  this->push_back( "AddI",  "05b3", "05b0" ,"17b" +
                    to_string(tabela.posicao_memoria[j][k]) );//soma r3 com a posição do vetor acessada

  this->push_back( "StoreR","05b4", "05b3", "17b0");
  this->push_back("AddI","05b3","05b1","17b1");
  this->push_back("Move","05b1","05b3","17b0");
}

void codigo_maquina::interpret_selecao(int i)
{
  this->interpret_argumento1(i);
  this->push_back("SetR","05b05","22b1","");
  this->push_back("Beq","05b04","05b05","17b2");
  this->push_back("J","05b0",cod_intermediario[2][i],"");
}


void codigo_maquina::interpret_ativacao(int i)
{
  if(cod_intermediario[1][i] == "output")
  {
    int j,k;
    tie(j,k) = tabela.pesquisa_var( "("+to_string(i-1)+")" , this->escopo);

    if(j==-1)
    {
      cout<<cod_intermediario[2][i]<<"\n11\n";
      tabela.print();
      std::cout <<  std::endl;
      throw "variavel não encontrada";
    }

    this->push_back( "AddI",  "05b3", "05b0" , "17b" +
                      to_string(tabela.posicao_memoria[j][k]) );
    this->push_back( "LoadR", "05b5", "05b3" , "17b0" );

    this->push_back("Output","05b2","05b5","17b0");
  }
  else if(cod_intermediario[1][i] == "input")
  {
    this->push_back("Input","05b6","05b0" ,"17b0");//leitura do valor das chaves
    this->push_back("Output","05b0","05b6","17b0");
    this->push_back("Input","05b5","05b20","17b0");//leitura do botão
    this->push_back("SetR" ,"05b3","22b1","");
    this->push_back("Beq"  ,"05b3", "05b5", "17b2");
    this->push_back("J"    ,"05b0", "("+to_string(i)+")", "");

    this->push_back("Input","05b5","05b20","17b0");//leitura do botão
    this->push_back("SetR" ,"05b3","22b0","");
    this->push_back("Beq"  ,"05b3", "05b5", "17b2");
    this->push_back("J"    ,"05b0", "+6("+to_string(i)+")", "");




    //salva variavel temporaria na tabela de simbolos-------------------------
    string id_temp = "("+ to_string(i) + ")";
    int j,k;
    tabela.insere( id_temp , VAR, "int" , this->escopo , "-1" , 1 );
    tie(j,k) = tabela.pesquisa_var( id_temp , this->escopo);
    tabela.posicao_memoria[j][k] = this->ponteiro_memoria_dados;
    this->ponteiro_memoria_dados += tabela.vetor_tamanho[j][k];
    //------------------------------------------------------------------------

    this->push_back( "AddI",  "05b3", "05b0" ,"17b" +
                      to_string(tabela.posicao_memoria[j][k]) );//soma r3 com a posição do vetor acessada

    this->push_back("StoreR","05b6", "05b3", "17b0");
    this->push_back("AddI","05b3","05b1","17b1");
    this->push_back("Move","05b1","05b3","17b0");
  }
  else
  {
    for(size_t j = 0 ; j < this->label_lines.size(); j++)
    {
      string label_name;
      int label_linha;
      tie(label_name,label_linha) = this->label_lines[j];
      if(cod_intermediario[1][i] == label_name)
      {
        //salva variavel temporaria na tabela de simbolos-------------------------
        string id_temp = "("+ to_string(i) + ")";
        int m,n;
        tabela.insere( id_temp , VAR, "int" , this->escopo , "-1" , 1 );
        tie(m,n) = tabela.pesquisa_var( id_temp , this->escopo);
        tabela.posicao_memoria[m][n] = this->ponteiro_memoria_dados;
        this->ponteiro_memoria_dados += tabela.vetor_tamanho[m][n];
        //------------------------------------------------------------------------

        this->push_back("MovePC","05b2","22b0","");
        this->push_back("Output","05b1","05b6","17b0");//retirar ...............................................................
        this->push_back("AddI","05b3","05b2","17b11");//mudar de 11 para 10
        this->push_back("StoreR","05b3","05b1","17b0");
        this->push_back("AddI","05b3","05b1","17b1");
        this->push_back("Move","05b1","05b3","17b0");
        this->push_back("StoreR","05b0","05b1","17b0");
        this->push_back("AddI","05b3","05b1","17b1");
        this->push_back("Move","05b1","05b3","17b0");
        this->push_back("Move","05b0","05b1","17b0");

        this->push_back("J", "05b0" ,"("+to_string(label_linha)+")","");
        this->pilha_ativacao_linha.push_back(i);
        this->pilha_ativacao_escopo.push_back(this->escopo);

        break;
      }
    }
  }
}

void codigo_maquina::interpret_param(int i)
{
  int j,k;

  tie(j,k) = tabela.pesquisa_var( cod_intermediario[1][i] , this->escopo);

  if(j==-1)
  {
    cout<<cod_intermediario[1][i]<<"\n1\n\n";
    tabela.print();
    std::cout <<  std::endl;
    throw "variavel não encontrada";
  }

  tabela.posicao_memoria[j][k] = this->ponteiro_memoria_dados;
  this->ponteiro_memoria_dados += tabela.vetor_tamanho[j][k];

  //this->push_back("AddI","05b3","05b1","17b1"); //possivelmente errado...................................................................
  //this->push_back("Move","05b1","05b3","17b0");

  this->push_back("SubI", "05b2","05b0","17b"+cod_intermediario[2][i]);
  this->push_back("LoadR","05b5","05b2","17b0");


  this->push_back("AddI","05b3","05b0","17b"+to_string(tabela.posicao_memoria[j][k]));

  this->push_back("StoreR","05b5","05b3","17b0");

  this->push_back("AddI","05b3","05b1","17b1");
  this->push_back("Move","05b1","05b3","17b0");
}

void codigo_maquina::interpret_retorno(int i)
{
  if(this->escopo=="main")
    this->push_back("End","27b0","","");
  else
  {
    if(cod_intermediario[1][0]!="")
      this->interpret_argumento1(i);

    this->push_back("SubI","05b3","05b0","17b1");
    this->push_back("SubI","05b2","05b0","17b2");

    this->push_back("Move" ,"05b1","05b2","17b0");

    this->push_back("LoadR","05b0","05b3","17b0");
    this->push_back("LoadR","05b6","05b2","17b0");

    if(cod_intermediario[1][0]!="")
      this->push_back("StoreR" ,"05b4" , "05b1","17b0");

    this->push_back("AddI","05b3","05b1","17b1");
    this->push_back("Move","05b1","05b3","17b0");

    this->push_back("Output","05b1","05b6","17b0");//retirar ...............................................................

    this->push_back("Jr","05b0","05b6","17b0");
  }
}

void codigo_maquina::read( codigo_intermediario intermediario)
{
  this->escopo = "global";
  this->ponteiro_memoria_dados = 0;
  this->cod_intermediario = intermediario.get_code( );

  vector<int> correspondencia_intermed_maquina( cod_intermediario[0].size() );

  int j , k, m, n;

  //this->push_back("SetR","05b0","22b0","");
  //this->push_back("SetR","05b1","22b0","");
  this->push_back("J","05b0","","");

  for( size_t i = 0; i < cod_intermediario[0].size( ) ; i++)
  {
    correspondencia_intermed_maquina[i] = this->codigo[0].size();

    if(cod_intermediario[0][i] == "label")
    {
      int temp_line = this->codigo[0].size();
      this->label_lines.push_back(tie(cod_intermediario[1][i], temp_line));
      this->ponteiro_memoria_dados=0;
      this->escopo = cod_intermediario[1][i];
    }
    else if(cod_intermediario[0][i] == "label-end")
    {
      this->ponteiro_memoria_dados=0;
      this->escopo = "global";
    }
    else if(cod_intermediario[0][i] == "declaracao")
      this->interpret_declaracao(i);

    else if(cod_intermediario[0][i] == "+" ||
            cod_intermediario[0][i] == "-" ||
            cod_intermediario[0][i] == "*" ||
            cod_intermediario[0][i] == "/")
      this->interpret_operacoes(i);

    else if(cod_intermediario[0][i] == "==" ||
            cod_intermediario[0][i] == "!=" ||
            cod_intermediario[0][i] == ">" ||
            cod_intermediario[0][i] == "<")
      this->interpret_comparacoes(i);

    else if(cod_intermediario[0][i] == "atribuicao")
      this->interpret_atribuicao(i);

    else if(cod_intermediario[0][i] == "INT_NUM" ||
            cod_intermediario[0][i] == "var")
      this->interpret_temp(i);

    else if(cod_intermediario[0][i] == "selecao")
      this->interpret_selecao(i);

    else if(cod_intermediario[0][i] == "salto")
      this->push_back("J","05b0",cod_intermediario[1][i],"");

    else if(cod_intermediario[0][i] == "ativacao")
      this->interpret_ativacao(i);

    else if(cod_intermediario[0][i] == "argumentos")
      this->interpret_temp(i);

    else if(cod_intermediario[0][i] == "param-decl")
      this->interpret_param(i);

    else if(cod_intermediario[0][i] == "retorno")
      this->interpret_retorno(i);

  }


  for(int i=0 ; i<this->label_lines.size(); i++)
  {
    int temp_line;
    string temp_scope;
    tie(temp_scope, temp_line) = this->label_lines[i];

    if(temp_scope == "main")
      this->codigo[2][0] = "22b"+to_string(temp_line);

  }


  for (size_t i = 1; i < this->codigo[0].size(); i++) //arruma endereços dos junps
  {
    if(this->codigo[0][i] == "J")
    {
      if(this->codigo[2][i][0]=='+')
      {
        string temp_index = this->codigo[2][i].substr(3, this->codigo[1][i].size()-2);
        string deslocamento = this->codigo[2][i].substr(1, 1);
        int e = correspondencia_intermed_maquina[ stoi(temp_index) ];

        cout<<this->codigo[2][i]<<endl;
        cout<<stoi(temp_index)<< " " <<stoi(deslocamento)<<"\n\n\n\n\n";

        this->codigo[2][i] = "22b" + to_string(e + stoi(deslocamento));
      }
      else
      {
        string temp_index = this->codigo[2][i].substr(1, this->codigo[1][i].size() -2);
        int e = correspondencia_intermed_maquina[ stoi(temp_index) ];
        this->codigo[2][i] = "22b" + to_string(e);
      }
    }
  }

  cout<<"\n\n\n";

  this->to_binary( );


}

vector<string>* codigo_maquina::get_code( ){return codigo;}

void codigo_maquina::print( )
{
  for(size_t i=0; i<codigo[0].size(); i++)
      cout<<i<<"\t"<<codigo[0][i]<<"\t"<<codigo[1][i]<<"\t"<<codigo[2][i]<<"\t"<<codigo[3][i]<<endl;
}
