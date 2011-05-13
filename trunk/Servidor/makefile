OBJ = object/main.o object/funcoesThread.o object/cliente.o
CPP = source/main.cpp source/funcoesThread.cpp source/cliente.cpp
CC = g++
DEBUG = -g
LFLAGS = -Wall -lm -lrt
CFLAGS = -c -Wall
HEADERSTHREAD =  headers/prototipos headers/cliente headers/controle headers/dados headers/mensagem
HEADERSMAIN = headers/prototipos headers/controle

bin/chat-server: $(OBJ)
	$(CC) $(DEBUG) -o bin/chat-server $(OBJ) $(LFLAGS)

object/cliente.o: headers/cliente
	$(CC) $(CFLAGS) $(DEBUG) source/cliente.cpp && mv cliente.o object

object/main.o: source/main.cpp $(HEADERSMAIN)
	$(CC) $(CFLAGS) $(DEBUG) source/main.cpp && mv main.o object 

object/funcoesThread.o: source/funcoesThread.cpp $(HEADERSTHREAD)
	$(CC) $(CFLAGS) $(DEBUG) source/funcoesThread.cpp && mv funcoesThread.o object

clean:
	rm bin/* object/*

