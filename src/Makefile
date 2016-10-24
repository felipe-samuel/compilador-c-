all:	clean arvore bison	flex lexer tabela_de_simbolos parser analisador_semantico codigo_intermediario codigo_maquina exec clean-intermed-files

arvore:
	g++ -std=c++11 -c arvore_sintatica.cpp

bison:
	bison -d parser.y

bison-graph:
	bison -g parser.y
	dot parser.dot -Tpng -o ../outputs/parser.png

flex: bison
	flex lexer.l

lexer:flex
	g++ -std=c++11 -c lex.yy.c
	g++ -std=c++11 -c lexer.cpp

tabela_de_simbolos: lexer
	g++ -std=c++11 -c tabela_de_simbolos.cpp

analisador_semantico: tabela_de_simbolos
	g++ -std=c++11 -c analisador_semantico.cpp

parser: analisador_semantico
	g++ -std=c++11 -c parser.tab.c tabela_de_simbolos.o analisador_semantico.o

codigo_intermediario: parser
	g++ -std=c++11 -c codigo_intermediario.cpp tabela_de_simbolos.o arvore_sintatica.o

codigo_maquina: codigo_intermediario
	g++ -std=c++11 -c codigo_maquina.cpp tabela_de_simbolos.o arvore_sintatica.o codigo_intermediario.o


exec: parser
	g++ -std=c++11 -o ../outputs/parser arvore_sintatica.o lex.yy.o parser.tab.o tabela_de_simbolos.o analisador_semantico.o parser.cpp -ly -lfl
	g++ -std=c++11 -o ../outputs/lexer arvore_sintatica.o lex.yy.o parser.tab.o tabela_de_simbolos.o analisador_semantico.o lexer.o -lfl
	g++ -std=c++11 -o ../outputs/gerador_de_codigo_intermediario arvore_sintatica.o lex.yy.o parser.tab.o tabela_de_simbolos.o analisador_semantico.o codigo_intermediario.o gerador_de_codigo_intermediario.cpp -ly -lfl
	g++ -std=c++11 -o ../outputs/gerador_de_codigo_maquina arvore_sintatica.o lex.yy.o parser.tab.o tabela_de_simbolos.o analisador_semantico.o codigo_intermediario.o codigo_maquina.o gerador_de_codigo_maquina.cpp -ly -lfl


graph: bison-graph clean-intermed-files

clean-intermed-files:
	rm -rf lex.yy.c lex.yy.o parser.tab.c parser.tab.h parser.tab.o parser.dot lexer.o arvore_sintatica.o analisador_semantico.o tabela_de_simbolos.o codigo_intermediario.o

clean:
	rm -rf lex.yy.c lex.yy.o parser.tab.c parser.tab.h parser.tab.o parser.dot lexer.o ../outputs/lexer ../outputs/parser arvore_sintatica.o tabela_de_simbolos.o
