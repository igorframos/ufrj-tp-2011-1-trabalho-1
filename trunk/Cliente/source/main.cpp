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
    pthread_t threadEnvio, threadRecebimento;
    Controle controle(0, argv[1]);

    if (pthread_create(&threadEnvio, NULL, funcoesThread::envioDeMensagens, (void*) &controle) != 0)
    {
        std::cout << "Erro na criação da thread que enviaria mensagens. Programa será abortado." << std::endl;
        exit(1);
    }
    
    if (pthread_create(&threadRecebimento, NULL, funcoesThread::recebimentoDeMensagens, (void*) &controle) != 0)
    {
        std::cout << "Erro na criação da thread que receberia mensagnes. Programa será abortado." << std::endl;
        exit(1);
    }
    
    while (!controle.sair);

    std::cout << "Encerrando main." << std::endl;

    return 0;
}

