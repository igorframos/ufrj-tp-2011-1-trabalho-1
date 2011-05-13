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
#include <netinet/in.h>

#include "../headers/cliente"
#include "../headers/mensagem"
#include "../headers/controle"

namespace funcoesThread
{
    /*
     *
     * Thread que envia as mensagens para os clientes.
     *
     * Fica verificando se a lista de mensagens está vazia. Caso não esteja, envia todas as mensagens que estiverem
     * aguardando na fila.
     *
     */
    void *envioDeMensagens (void *ptr)
    {
        Controle *controle = (Controle*) ptr;   // Recebe as informações globais armazenadas
                                                // na thread da main e passadas no ponteiro.

        std::cout << "Thread de envio de mensagens criada corretamente." << std::endl;

        while (!(controle->sair))   // Enquanto não for recebida a ordem de sair do programa
        {
            pthread_mutex_lock(&controle->mutexFilaMensagens);  // Trava a fila de mensagens (controle de concorrência)

            while (!(controle->filaMensagens.empty()))          // Enquanto a fila de mensagens contiver alguém
            {
                Mensagem m = controle->filaMensagens.front();             // Retirar primeira mensagem da fila e enviar
                controle->filaMensagens.pop();

                // Falta aqui o código que envia de fato uma mensagem.
            }
            pthread_mutex_unlock(&controle->mutexFilaMensagens);    // Destrava a fila de mensagens
        }

        std::cout << "Thread envioDeMensagens encerrada." << std::endl;
        return NULL;
    }

    void *recebimentoDeConexoes (void *ptr)
    {
        std::cout << "Thread de recebimento de conexões criada corretamente." << std::endl;
        
        const int porta = 2011;                 // Constante com o número da porta que será usada.
        Controle *controle = (Controle*) ptr;   // Recebe as informações globais armazenadas
                                                // na thread main e passadas no ponteiro.

        int socketID;           // Identificador do socket que receberá as conexões.
        sockaddr_in serv_addr;  // Cria um endereço para o servidor.
         
        // Abre canal de comunicação externa.
        socketID = socket(AF_INET, SOCK_STREAM, 0);
        if (socketID < 0)
        {
            std::cout << "Criação do socket falhou." << std::endl;
            return NULL;
        }
     
        serv_addr.sin_family = AF_INET;            // Cria um endereço IPv4 local.
        serv_addr.sin_port = htons(porta);         // Determina uma porta para a comunicação.
        serv_addr.sin_addr.s_addr = INADDR_ANY;    // Recebe todos os pacotes da porta.
     
        // Liga o canal criado ao endereço da máquina. Usa o sockaddr que foi criado acima.
        if (bind(socketID, (const sockaddr*) &serv_addr, sizeof(serv_addr)))
        {
            // Thread aborta se não puder receber conexões externas.
            std::cout << "Impossível abrir canal de comunicação externa (bind). Encerrando a aplicação." << std::endl;
            return NULL;
        }
         
        // Abre o canal para recepção de comunicações externas.
        // Determina uma fila de tamanho até 2^16. Se houver 2^16 conexões na fila, novas conexões
        // serão descartadas e a aplicação cliente receberá um código de erro.
        if (listen(socketID, (1<<16)) < 0)
        {
            // Thread aborta se não for possível ficar escutando no socket criado.
            std::cout << "Impossível receber conexões externas (listen). Encerrando a aplicação." << std::endl;
            return NULL;
        }
         
        /*
         *
         * FIX
         *
         * Esse trecho está muito dependente das variáveis do programa de SO e deve ser reescrito
         * para se adequar ao trabalho de TP. Também está incongruente com o objetivo do trabalho
         * de TP.
         *
         * Esse trecho deve ser responsável por aguardar uma conexão TCP com algum cliente e criar
         * a thread que vai receber as mensagens vindas por uma conexão TCP que seja estabelecida.
         *
         * A aplicação cliente deve se conectar.
         * O servidor envia uma lista com os usuários online.
         * A aplicação cliente envia o nome de usuário.
         * Servidor envia endereço do grupo para a conexão multicast via UDP e passa a aguardar mensagens.
         *
         * Uma parte desse algoritmo é feita aqui, a outra é feita na thread que trata cada cliente.
         *
         */

        // Para cada conexão recebida cria uma nova thread para lidar com ela e volta a esperar
        // nova conexão externa.
        pthread_mutex_lock(&controle->mutexEncerramento);
        while (!controle->sair)
        {
            pthread_mutex_unlock(&controle->mutexEncerramento);

            Cliente *cliente = new Cliente();
            cliente->socketEnvioID = accept(socketID, (sockaddr*) &(cliente->endereco), &(cliente->tamanho));
            if (cliente->socketEnvioID < 0)
            {
                std::cout << "Falha na recepção de novas conexões. Encerrando thread. (accept)" << std::endl;
                return NULL;
            }

            /*
             * Problemas com a forma como deve ser feita a passagem dos dados.
             * Preciso passar os dados que já tenho do cliente e, ao mesmo tempo, passar os dados globais do programa.
             * Saída deve ser agrupar os dois em um outra estrutura só para essa transmissão.
             */
            if (pthread_create(&(cliente->thread), NULL, recebimentoDeMensagens, /******************************/))
            {
                std::cout << "Não é possível criar thread para o cliente. Ignorando conexão." << std::endl;
            }
            
            pthread_mutex_lock(&controle->mutexEncerramento);
        }
        /*
        pthread_mutex_lock(&controle->mutexEncerramento);
        while (!controle->sair)
        {
            pthread_mutex_unlock(&controle->mutexEncerramento);
            Cliente *cliente = new Cliente();
            cliente->socketEnvioID = accept(socketID, (sockaddr*) &(cliente->addr), &(cliente->size));
            if (cliente->socketEnvioID < 0)
            {
                cout << "Impossível receber conexões externas (accept). Encerrando a aplicação." << std::endl;
                return NULL;
            }
            if (pthread_create(&(cliente->thread), NULL, recebe_dados, cliente))
            {
                cout << "Impossível criar thread para tratar conexões externas. Ignorando conexão." << std::endl;
            }
             pthread_mutex_lock(&controle->mutexEncerramento);
        }
        close(socketID);
        */

        std::cout << "Thread recebimentoDeConexoes encerrada." << std::endl;
        return NULL;
    }

    void *recebimentoDeMensagens (void *ptr)
    {
        std::cout << "Thread de recebimento de mensagens criada corretamente. Uma conexão foi estabelecida." << std::endl;

        int *sair = (int*) ptr;

        while (!(*sair))
        {
        }

        return NULL;
    }
}
