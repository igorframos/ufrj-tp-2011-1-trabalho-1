#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <cmath>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <bits/posix_opt.h>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <queue>
#include <stack>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../headers/controle"
#include "../headers/prototipos"

int main (int argc, char *argv[])
{
    int sair = 0;
    pthread_t threadEnvio, threadRecebimento;
    Controle controle(0);

    std::cout << "Criando thread de envio de mensagens." << std::endl;    
    if (pthread_create(&threadEnvio, NULL, funcoesThread::envioDeMensagens, (void*) &controle) != 0)
    {
        std::cout << "Erro na criação da thread que enviaria mensagens. Programa será abortado." << std::endl;
        exit(1);
    }
    
    std::cout << "Criando thread de recebimento de mensagens." << std::endl;
    if (pthread_create(&threadRecebimento, NULL, funcoesThread::recebimentoDeMensagens, (void*) &sair) != 0)
    {
        std::cout << "Erro na criação da thread que receberia mensagnes. Programa será abortado." << std::endl;
        exit(1);
    }
    
    while (!controle.sair);

    pthread_cancel(threadEnvio);
    pthread_cancel(threadRecebimento);

    return 0;
}

