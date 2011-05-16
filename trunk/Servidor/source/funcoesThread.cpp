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

        int socketID;               // Identificador do socket UDP.
        const int porta = 2012;     // Porta que será usada para envio de mensagens.
        sockaddr_in grupo;          // Informações sobre o grupo para o qual serão enviadas as mensagens.
        char enderecoGrupo[] = "230.145.0.1";   // Endereço IP do grupo.
        in_addr interfaceLocal;     // Interface usada para ser enviar as mensagens.

        // Iniciando o socket UDP. (SOCK_DGRAM)
        socketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (socketID < 0)
        {
            std::cout << "Erro ao criar socket UDP. Encerrando thread de envio de mensagens." << std::endl;
            exit(1);
        }

        // Estabelecendo detalhes da comunicação para envio das mensagens via UDP.
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

                // Envia mensagem via UDP para um grupo predeterminado.
                if (sendto(socketID, &m, sizeof(m), 0, (sockaddr*) &grupo, sizeof(grupo)) < 0)
                {
                    close(socketID);
                    std::cout << "Erro ao enviar mensagem via socket UDP. Encerrando thread de envio de mensagens." << std::endl;
                    exit(1);
                }

                // Imprime no console a mensagem enviada precedida pelo nome do usuário que a enviou caso a mensagem
                // não seja um aviso de que o usuário desconectou. Do contrário, imprime que o usuário se desconectou
                if (!strcmp(m.texto, "exit_program"))
                {
                    std::cout << m.remetente << " desconectou-se." << std::endl;
                }
                else if (!strcmp(m.texto, "new_client_connecting"))
                {
                    std::cout << m.remetente << " conectou-se." << std::endl;
                }
                else
                {
                    std::cout << m.remetente << ">> " << m.texto << std::endl;
                }
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

        // Sai do programa com status 1 (falha) se não conseguir criar o socket TCP que recebe as mensagens.
        if (!comunicacao::criaSocket(controle, socketID, serv_addr))
        {
            close(socketID);
            exit(1);
        }

        // Sai do programa com status 1 (falha) se houver problemas com a criação de comunicações.
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

        // Remove um cliente da lista de clientes se a sua thread for terminar.
        pthread_mutex_lock(&controle->mutexListaClientes);
        controle->listaClientes.erase(controle->listaClientes.find(cliente));
        pthread_mutex_unlock(&controle->mutexListaClientes);

        return NULL;
    }
}

