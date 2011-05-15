OBJ = object/main.o object/funcoesThread.o
CPP = source/main.cpp source/funcoesThread.cpp
CC = g++
DEBUG = -g
LFLAGS = -Wall -lm -lrt
CFLAGS = -c -Wall -O2

bin/chat-client: $(OBJ)
	$(CC) -o bin/chat-client $(OBJ) $(LFLAGS)

object/main.o: source/main.cpp headers/prototipos headers/controle
	$(CC) $(CFLAGS) source/main.cpp && mv main.o object

object/funcoesThread.o: source/funcoesThread.cpp headers/mensagem headers/controle headers/prototipos
	$(CC) $(CFLAGS) source/funcoesThread.cpp && mv funcoesThread.o object

clean:
	rm object/* bin/*

