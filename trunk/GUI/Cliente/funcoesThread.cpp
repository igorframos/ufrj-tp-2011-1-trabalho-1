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
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include "controle"
#include "mensagem"
#include "prototipos"

namespace funcoesThread
{
    // Estabelece uma conexão TCP com o servidor para enviar as msgs e fica aguardando mensagens para serem enviadas.
    void *envioDeMensagens (void *ptr)
    {
        Controle *controle = (Controle*) ptr;

        int socketID;                       // Identificador do socket TCP.
        hostent *servidor;                  // Host da comunicação. 
        const int porta = 2011;             // Comunicação TCP será feita na porta 2011.
        sockaddr_in enderecoServidor;       // Informações sobre o servidor.

        // Cria o socket TCP a ser usado no envio de mensagens.
        socketID = socket(AF_INET, SOCK_STREAM, 0);
        if (socketID < 0)
        {
            std::cout << "Impossível abrir socket para comunicação externa. Encerrando programa." << std::endl;
            exit(1);
        }

        // Encontrando informações sobre o servidor de verdade via DNS.
        servidor = gethostbyname(controle->host);
        if (!servidor)
        {
            close(socketID);
            std::cout << "Servidor não encontrado. Encerrando programa." << std::endl;
            exit(1);
        }

        // Determinando detalhes sobre o servidor.
        bzero(&enderecoServidor, sizeof(enderecoServidor));
        enderecoServidor.sin_family = AF_INET;
        enderecoServidor.sin_port = htons(porta);
        bcopy((char*) servidor->h_addr, (char*) &enderecoServidor.sin_addr.s_addr, servidor->h_length);

        // Conectando ao servidor.
        if (connect(socketID, (sockaddr*) &enderecoServidor, sizeof(enderecoServidor)) < 0)
        {
            close(socketID);
            std::cout << "Impossível estabelecer conexão com o servidor. Encerrando programa." << std::endl;
            exit(1);
        }

        // Espera que o usuário dê seu nome e guarda na variável nome.
        char nome[32];
        comunicacao::escolheNome(controle, nome, socketID);
        
        // Recebe a lista de clientes do servidor.
        comunicacao::recebeListaClientes(controle, socketID);

        // Envia mensagens para o servidor.
        comunicacao::enviaMensagens(controle, socketID, nome); 

        return NULL;
    }

    // Estabelece uma conexão UDP com o servidor para receber as msgs enviadas via multicast.
    void *recebimentoDeMensagens (void *ptr)
    {
        Controle *controle = (Controle*) ptr;

        int socketID;               // Identificador do socket UDP.
        ip_mreq grupo;              // Informações do grupo para que seja possível adicionar este programa ao mesmo.
        sockaddr_in socketLocal;    // Informações sobre o socket local usado para a comunicação com o grupo.
        Mensagem m;                 // Mensagem que será recebida.
        const int porta = 2012;     // A comunicação do grupo se dará pela porta 2012.
        char enderecoGrupo[] = "230.145.0.1";   // Endereço IP do grupo. Deve ser na faixa de 224.0.1.0 a 239.255.255.255.
        socklen_t addrlen;          // Variável com o tamanho do endereço do grupo para ser passada ao recvfrom.

        // Cria socket UDP.
        socketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (socketID < 0)
        {
            std::cout << "Impossível abrir socket UDP para receber mensagens. Encerrando programa." << std::endl;
            exit(1);
        }

        // Determina que este socket pode ser reutilizado.
        int reuse = 1;
        if (setsockopt(socketID, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(reuse)) < 0)
        {
            close(socketID);
            std::cout << "Erro ao determinar reuso do socket. Encerrando programa." << std::endl;
            exit(1);
        }

        // Determina detalhes sobre o socket local e cria o endereço de fato.
        bzero(&socketLocal, sizeof(socketLocal));
        socketLocal.sin_family = AF_INET;
        socketLocal.sin_port = htons(porta);
        socketLocal.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(socketID, (sockaddr*) &socketLocal, sizeof(socketLocal)))
        {
            close(socketID);
            std::cout << "Erro ao fazer o bind do socket local. Encerrando programa." << std::endl;
            exit(1);
        }

        // Entra no grupo para que seja possível receber as mensagens enviadas para ele.
        grupo.imr_multiaddr.s_addr = inet_addr(enderecoGrupo);
        grupo.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(socketID, IPPROTO_IP, IP_ADD_MEMBERSHIP, &grupo, sizeof(grupo)) < 0)
        {
            close(socketID);
            std::cout << "Erro ao entrar no grupo de multicasting. Encerrando programa." << std::endl;
            exit(1);
        }

        // Recebe uma mensagem e imprime no console. Caso ser um aviso de que um cliente desconectou, o remove da
        // lista de clientes e imprime a lista atual.
        addrlen = sizeof(grupo);
        while (!controle->sair)
        {
            if (recvfrom(socketID, &m, sizeof(m), 0, (sockaddr*) &grupo, &addrlen)  < 0)
            {
                close(socketID);
                std::cout << "Problemas no recebimento de mensagens. Encerrando programa." << std::endl;
                exit(1);
            }

            if (!strcmp(m.texto, "exit_program"))
            {
                pthread_mutex_lock(&controle->mutexListaClientes);

                std::set<std::string>::iterator it = controle->listaClientes.find(m.remetente);
                if (it != controle->listaClientes.end())
                {
                    std::cout << m.remetente << " desconectou-se." << std::endl;
                    controle->listaClientes.erase(it);
                }
                pthread_mutex_unlock(&controle->mutexListaClientes);
            }
            else if (!strcmp(m.texto, "new_client_connecting"))
            {
                pthread_mutex_lock(&controle->mutexListaClientes);
                controle->listaClientes.insert(m.remetente);
                pthread_mutex_unlock(&controle->mutexListaClientes);

                std::cout << m.remetente << " conectou-se." << std::endl;
            }
            else
            {
                std::cout << m.remetente << ">> " << m.texto << std::endl;
            }
        }

        close(socketID);        
        return NULL;
    }
}

