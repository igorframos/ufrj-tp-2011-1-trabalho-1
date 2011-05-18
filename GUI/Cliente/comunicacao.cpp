#include <iostream>
#include <set>
#include <cstdlib>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

#include "mensagem"
#include "controle"
#include "prototipos"
#include "mainwindow.h"

extern Controle *controle;

namespace comunicacao
{
    void recebeListaClientes(int socketID)
    {
        int tamanhoListaClientes;
        if (read(socketID, &tamanhoListaClientes, sizeof(tamanhoListaClientes)) < 0)
        {
            close(socketID);
            std::cout << "Erro no recebimento da lista de clientes ativos. Não recebido o tamanho. Encerrando programa." << std::endl;
            exit(1);
        }

        for (int i = 0; i < tamanhoListaClientes; ++i)
        {
            char nomeCliente[32];

            if (read(socketID, nomeCliente, sizeof(nomeCliente)) < 0)
            {
                close(socketID);
                std::cout << "Não foi possível receber o nome de todos os clientes. Encerrando programa." << std::endl;
                exit(1);
            }

            pthread_mutex_lock(&controle->mutexListaClientes);
            controle->listaClientes.insert(nomeCliente);
            pthread_mutex_unlock(&controle->mutexListaClientes);
        }

        pthread_mutex_lock(&controle->mutexListaClientes);
        for (std::set<std::string>::iterator i = controle->listaClientes.begin(); i != controle->listaClientes.end(); ++i)
        {
            std::cout << "Cliente " << *i << " online." << std::endl;
        }
        pthread_mutex_unlock(&controle->mutexListaClientes);
        std::cout << std::endl;
    }
}

