OBJ = object/main.o object/funcoesThread.o object/comunicacao.o
CPP = source/main.cpp source/funcoesThread.cpp source/comunicacao.cpp
CC = g++
DEBUG = -g 
LFLAGS = -Wall -lm -lrt
CFLAGS = -c -Wall -O2

bin/chat-client: $(OBJ)
	$(CC) -o bin/chat-client $(OBJ) $(LFLAGS)

object/main.o: source/main.cpp headers/prototipos headers/controle
	$(CC) $(CFLAGS) $(DEBUG) source/main.cpp && mv main.o object

object/funcoesThread.o: source/funcoesThread.cpp headers/mensagem headers/controle headers/prototipos
	$(CC) $(CFLAGS) $(DEBUG) source/funcoesThread.cpp && mv funcoesThread.o object

object/comunicacao.o: source/comunicacao.cpp headers/prototipos headers/mensagem headers/controle
	$(CC) $(CFLAGS) $(DEBUG) source/comunicacao.cpp && mv comunicacao.o object

clean:
	rm object/* bin/*

