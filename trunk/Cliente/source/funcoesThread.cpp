/* 
 * Ideia de funcionamento desse programa Cliente:
 *  Cliente tem uma conexão TCP que envia mensagens para o Servidor.
 *  Cliente tem uma conexão UDP que recebe mensagens do Servidor.
 *  Quando um cliente deseja enviar uma mensagem, ele a envia ao Servidor que a repassa a todos os Clientes (inclusive ao próprio
 *      que enviou para ele) via multicast.
 *  O Cliente recebe a mensagem e acrescenta à janela de mensagens recebidas. Uma mensagem enviada, para a janela de mensagens
 *      recebidas, não existe. Esta só acrescenta uma nova mensagem quando ela chega do Servidor, ou seja, quando os destinatários não
 *      receberem a mensagem, exibe-se um alerta e o cliente vê a própria mensagem anexada ao final da conversa.
 */

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

#include "../headers/controle"
#include "../headers/mensagem"
#include "../headers/prototipos"

namespace funcoesThread
{
    // Estabelece uma conexão TCP com o servidor para enviar as msgs e fica aguardando mensagens para serem enviadas.
    void *envioDeMensagens (void *ptr)
    {
        Controle *controle = (Controle*) ptr;

        int socketID;
        hostent *servidor;
        const int porta = 2011;
        sockaddr_in enderecoServidor;

        socketID = socket(AF_INET, SOCK_STREAM, 0);
        if (socketID < 0)
        {
            std::cout << "Impossível abrir socket para comunicação externa. Encerrando programa." << std::endl;
            exit(1);
        }

        // Está conectando sempre a localhost. Resolver isso.
        servidor = gethostbyname("localhost");
        if (!servidor)
        {
            close(socketID);
            std::cout << "Servidor não encontrado. Encerrando programa." << std::endl;
            exit(1);
        }

        bzero(&enderecoServidor, sizeof(enderecoServidor));
        enderecoServidor.sin_family = AF_INET;
        enderecoServidor.sin_port = htons(porta);
        bcopy((char*) servidor->h_addr, (char*) &enderecoServidor.sin_addr.s_addr, servidor->h_length);

        if (connect(socketID, (sockaddr*) &enderecoServidor, sizeof(enderecoServidor)) < 0)
        {
            close(socketID);
            std::cout << "Impossível estabelecer conexão com o servidor. Encerrando programa." << std::endl;
            exit(1);
        }

        char nome[32];
        bool nomeInvalido = true;
        while (nomeInvalido)
        {
            std::cin.getline(nome, 32);
            std::cout << "Nome escolhido: " << nome << std::endl;

            if (write(socketID, &nome, sizeof(nome)) < 0)
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

        return NULL;
    }

    // Estabelece uma conexão UDP com o servidor para receber as msgs enviadas via multicast.
    void *recebimentoDeMensagens (void *ptr)
    {
        Controle *controle = (Controle*) ptr;

        int socketID;
        ip_mreq grupo;
        sockaddr_in socketLocal;
        Mensagem m;
        const int porta = 2012;
        char enderecoGrupo[] = "230.145.0.1";
        socklen_t addrlen;

        socketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (socketID < 0)
        {
            std::cout << "Impossível abrir socket UDP para receber mensagens. Encerrando programa." << std::endl;
            exit(1);
        }

        int reuse = 1;
        if (setsockopt(socketID, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(reuse)) < 0)
        {
            close(socketID);
            std::cout << "Erro ao determinar reuso do socket. Encerrando programa." << std::endl;
            exit(1);
        }

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

        grupo.imr_multiaddr.s_addr = inet_addr(enderecoGrupo);
        grupo.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(socketID, IPPROTO_IP, IP_ADD_MEMBERSHIP, &grupo, sizeof(grupo)) < 0)
        {
            close(socketID);
            std::cout << "Erro ao entrar no grupo de multicasting. Encerrando programa." << std::endl;
            exit(1);
        }

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
                controle->listaClientes.erase(controle->listaClientes.find(m.remetente));
                for (std::set<std::string>::iterator i = controle->listaClientes.begin(); i != controle->listaClientes.end(); ++i)
                {
                    std::cout << "Cliente " << *i << " online." << std::endl;
                }
                pthread_mutex_unlock(&controle->mutexListaClientes);
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

