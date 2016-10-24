#ifndef HASHING_H
#define HASHING_H

#define SIZE 211 //número primo para melhorar a distribuição
			// em uma tabela de hashing com até 211 registros
#define SHIFT 4

int hashing(const char * key)
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

#endif
