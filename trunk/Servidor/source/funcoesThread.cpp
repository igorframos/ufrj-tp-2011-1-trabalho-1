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

        std::cout << "Thread de envio de mensagens criada corretamente." << std::endl;

        // =======================================================================================================
        // Experimentando criar o socket e o grupo UDP. Vamos ver no que dá isso.
        // =======================================================================================================

        int socketID;
        const int porta = 2012;
        sockaddr_in grupo;
        char enderecoGrupo[] = "242.192.4.4";

        socketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (socketID < 0)
        {
            std::cout << "Erro ao criar socket UDP. Encerrando thread de envio de mensagens." << std::endl;
            return NULL;
        }

        bzero(&grupo, sizeof(grupo));
        grupo.sin_family = AF_INET;
        grupo.sin_addr.s_addr = inet_addr(enderecoGrupo);
        grupo.sin_port = htons(porta);

        // =======================================================================================================
        // Fim do experimento
        // =======================================================================================================

        pthread_mutex_lock(&controle->mutexEncerramento);   // Controle de concorrência da variável controle->sair.
        while (!(controle->sair))
        {
            pthread_mutex_unlock(&controle->mutexEncerramento); // Destrava a variável controle->sair.

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
                    return NULL;
                }

                std::cout << m.remetente << ">> " << m.texto << std::endl;
            }
            pthread_mutex_unlock(&controle->mutexFilaMensagens);    // Destrava a fila de mensagens.

            pthread_mutex_lock(&controle->mutexEncerramento);   // Controle de concorrência da variável controle->sair.
        }
        pthread_mutex_unlock(&controle->mutexEncerramento);

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
        std::cout << "Thread de recebimento de conexões criada corretamente." << std::endl;

        Controle *controle = (Controle*) ptr;   // Recebe as informações globais armazenadas
                                                // na thread main e passadas no ponteiro.

        int socketID;           // Identificador do socket que receberá as conexões.
        sockaddr_in serv_addr;  // Cria um endereço para o servidor.

        if (!comunicacao::criaSocket(controle, socketID, serv_addr))
        {
            close(socketID);
            return NULL;
        }

        if (!comunicacao::recebeConexoes(controle, socketID))
        {
            close(socketID);
            return NULL;
        } 

        close(socketID);
        std::cout << "Thread recebimentoDeConexoes encerrada." << std::endl;
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
        std::cout << "Thread de recebimento de mensagens criada corretamente. Uma conexão foi estabelecida." << std::endl;

        // Recebe os dados e libera a memória da variável que foi usada só para passar essas informações.
        Dados *dados = (Dados*) ptr;
        Cliente cliente = *(dados->cliente);
        Controle *controle = dados->controle;

        // Recebe um nome de usuário e o valida. Coloca o nome em cliente e insere cliente no set de clientes.
        if (!comunicacao::recebeNomeUsuario(controle, cliente))
        {
            return NULL;
        }
        
        // Envia a lista de contatos online para o cliente.
        if (!comunicacao::enviaContatos(controle, cliente))
        {
            return NULL;
        }

        // Fica recebendo as mensagens e adicionando à fila de mensagens a serem enviadas.
        comunicacao::recebeMensagens(controle, cliente);

        return NULL;
    }
}

