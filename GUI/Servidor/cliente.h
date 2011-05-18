#ifndef CLIENTE_H
#define CLIENTE_H

#include <pthread.h>
#include <sys/types.h>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>

struct Cliente
{
    std::string nome;
    int socketEnvioID;
    sockaddr_in endereco;
    socklen_t tamanho;
    pthread_t thread;

    Cliente(const std::string = "", const int socketEnvioID = -1);
};

bool operator<(const Cliente&, const Cliente&);

#endif // CLIENTE_H
