#include <iostream>
#include <set>
#include <cstdlib>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

#include "../headers/mensagem"
#include "../headers/controle"
#include "../headers/prototipos"

namespace comunicacao
{
    void escolheNome(Controle *controle, char nome[32], int socketID)
    {
        bool nomeInvalido = true;
        while (nomeInvalido)
        {
            std::cin.getline(nome, 32);
            std::cout << "Nome escolhido: " << nome << std::endl;

            if (write(socketID, nome, 32) < 0)
            {
                close(socketID);
                std::cout << "Não foi possível enviar o nome para o servidor. Encerrando programa." << std::endl;
                exit(1);
            }

            if (read(socketID, &nomeInvalido, sizeof(nomeInvalido)) < 0)
            {
                close(socketID); 
                std::cout << "Não foi possível receber resposta do servidor sobre o nome. Encerrando programa." << std::endl;
                exit(1);
            }

            if (nomeInvalido)
            {
                std::cout << "Nome não disponível. Tente novamente." << std::endl;
            }
        }
    }


    void recebeListaClientes(Controle* controle, int socketID)
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

    void enviaMensagens(Controle *controle, int socketID, char nome[32])
    {
        while (!controle->sair)
        {
            char linha[1024];
            std::cin.getline(linha, 1024);

            Mensagem m;
            strncpy(m.remetente, nome, sizeof(m.remetente));
            strncpy(m.texto, linha, sizeof(m.texto));

            if (write(socketID, &m, sizeof(m)) < 0)
            {
                std::cout << "Mensagem não pôde ser enviada." << std::endl;
            }

            if (!strcmp(linha, "exit_program"))
            {
                close(socketID);
                std::cout << "Encerrando programa." << std::endl;
                exit(0);
            }
        }
    }
}

