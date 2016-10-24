#include "analisador_semantico.hpp"

using namespace std;


int main(int argc, char *argv[])
{
  analisador_semantico a;


  if (argc == 2)
  {
    a.run(string(argv[1]));
  }
  return 1;
}
