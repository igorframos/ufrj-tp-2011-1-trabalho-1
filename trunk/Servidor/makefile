OBJ = object/main.o object/funcoesThread.o object/cliente.o object/comunicacao.o
CPP = source/main.cpp source/funcoesThread.cpp source/cliente.cpp source/comunicacao.cpp
CC = g++
DEBUG = -g
LFLAGS = -Wall -lm -lrt
CFLAGS = -c -Wall
HEADERSTHREAD =  headers/prototipos headers/cliente headers/controle headers/dados headers/mensagem
HEADERSMAIN = headers/prototipos headers/controle
HEADERSCOMUNICACAO = headers/cliente headers/controle headers/mensagem headers/prototipos

bin/chat-server: $(OBJ)
	$(CC) $(DEBUG) -o bin/chat-server $(OBJ) $(LFLAGS)

object/cliente.o: source/cliente.cpp headers/cliente
	$(CC) $(CFLAGS) $(DEBUG) source/cliente.cpp && mv cliente.o object

object/main.o: source/main.cpp $(HEADERSMAIN)
	$(CC) $(CFLAGS) $(DEBUG) source/main.cpp && mv main.o object 

object/funcoesThread.o: source/funcoesThread.cpp $(HEADERSTHREAD)
	$(CC) $(CFLAGS) $(DEBUG) source/funcoesThread.cpp && mv funcoesThread.o object

object/comunicacao.o: source/comunicacao.cpp $(HEADERSCOMUNICACAO)
	$(CC) $(CFLAGS) $(DEBUG) source/comunicacao.cpp && mv comunicacao.o object

clean:
	rm bin/* object/*

