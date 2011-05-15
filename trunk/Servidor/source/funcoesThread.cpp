/*
 * Funções usadas nas threads do programa.
 *
 * Envio de mensagens, recebimento de conexões e recebimento de mensagens são as tarefas que devem
 * ser executadas pelas threads.
 *
 * FIX
 *
 * Refatorar o código para dividir essa joça em funções.
 *
 * TODO
 *
 * Falta fazer o pedaço de enviar mensagens via UDP e enviar para o cliente a questão de se inscrever
 * no grupo UDP.
 *
 * BUGS:
 *  - Recebimento de conexões não está recebendo o sinal de finalizar o programa. Provavelmente
 *    está presa no accept de novas conexões. Ainda não sei como resolver.
 *  - Possivelmente, semáforos.  
 */

#include <iostream>
#include <unistd.h>
#include <pthread.h>
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
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../headers/dados"
#include "../headers/cliente"
#include "../headers/mensagem"
#include "../headers/controle"
#include "../headers/prototipos"

namespace funcoesThread
{
    /*
     * Thread que envia as mensagens para os clientes.
     *
     * Fica verificando se a lista de mensagens está vazia. Caso não esteja, envia todas as mensagens que estiverem
     * aguardando na fila.
     */
    void *envioDeMensagens (void *ptr)
    {
        Controle *controle = (Controle*) ptr;   // Recebe as informações globais armazenadas
                                                // na thread da main e passadas no ponteiro.

        int socketID;
        const int porta = 2012;
        sockaddr_in grupo;
        char enderecoGrupo[] = "230.145.0.1";
        in_addr interfaceLocal;

        socketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (socketID < 0)
        {
            std::cout << "Erro ao criar socket UDP. Encerrando thread de envio de mensagens." << std::endl;
            exit(1);
        }

        bzero(&grupo, sizeof(grupo));
        grupo.sin_family = AF_INET;
        grupo.sin_addr.s_addr = inet_addr(enderecoGrupo);
        grupo.sin_port = htons(porta);
        interfaceLocal.s_addr = INADDR_ANY;
        if (setsockopt(socketID, IPPROTO_IP, IP_MULTICAST_IF, &interfaceLocal, sizeof(interfaceLocal)) < 0)
        {
            std::cout << "Erro ao preparar interface local para multicast via UDP. Encerrando thread." << std::endl;
            close(socketID);
            exit(1);
        }

        while (!(controle->sair))
        {
            pthread_mutex_lock(&controle->mutexFilaMensagens);  // Trava a fila de mensagens (controle de concorrência).

            while (!(controle->filaMensagens.empty()))          // Enquanto a fila de mensagens contiver alguém
            {
                Mensagem m = controle->filaMensagens.front();   // Retirar primeira mensagem da fila e enviar.
                controle->filaMensagens.pop();

                // Código ainda em teste que deveria enviar uma mensagem.
                if (sendto(socketID, &m, sizeof(m), 0, (sockaddr*) &grupo, sizeof(grupo)) < 0)
                {
                    close(socketID);
                    std::cout << "Erro ao enviar mensagem via socket UDP. Encerrando thread de envio de mensagens." << std::endl;
                    exit(1);
                }

                std::cout << m.remetente << ">> " << m.texto << std::endl;
            }
            pthread_mutex_unlock(&controle->mutexFilaMensagens);    // Destrava a fila de mensagens.
        }

        close(socketID);

        std::cout << "Thread envioDeMensagens encerrada." << std::endl;
        return NULL;
    }

    /*
     * Thread que aguarda as conexões.
     *
     * Fica o tempo todo esperando uma conexão de algum cliente. Quando a recebe, cria uma thread nova para receber
     * as mensagens que esse cliente envia.
     */
    void *recebimentoDeConexoes (void *ptr)
    {
        Controle *controle = (Controle*) ptr;   // Recebe as informações globais armazenadas
                                                // na thread main e passadas no ponteiro.

        int socketID;           // Identificador do socket que receberá as conexões.
        sockaddr_in serv_addr;  // Cria um endereço para o servidor.

        if (!comunicacao::criaSocket(controle, socketID, serv_addr))
        {
            close(socketID);
            exit(1);
        }

        if (!comunicacao::recebeConexoes(controle, socketID))
        {
            close(socketID);
            exit(1);
        } 

        close(socketID);
        return NULL;
    }

    /*
     * Thread que recebe mensagens de um cliente específico.
     *
     * Envia e recebe dados iniciais (nome de usuário e informações de conexão) e fica aguardando mensagens do cliente.
     * Quando recebe uma mensagem, a adiciona à fila das mensagens que devem ser enviadas.
     */
    void *recebimentoDeMensagens (void *ptr)
    {
        // Recebe os dados e libera a memória da variável que foi usada só para passar essas informações.
        Dados *dados = (Dados*) ptr;
        Cliente cliente = *(dados->cliente);
        Controle *controle = dados->controle;

        // Recebe um nome de usuário e o valida. Coloca o nome em cliente e insere cliente no set de clientes.
        if (!comunicacao::recebeNomeUsuario(controle, cliente))
        {
            exit(1);
        }
        
        // Envia a lista de contatos online para o cliente.
        if (!comunicacao::enviaContatos(controle, cliente))
        {
            exit(1);
        }

        // Fica recebendo as mensagens e adicionando à fila de mensagens a serem enviadas.
        comunicacao::recebeMensagens(controle, cliente);

        pthread_mutex_lock(&controle->mutexListaClientes);
        controle->listaClientes.erase(controle->listaClientes.find(cliente));
        pthread_mutex_unlock(&controle->mutexListaClientes);
        std::cout << "Conexão de " << cliente.nome << " encerrada." << std::endl;

        return NULL;
    }
}

