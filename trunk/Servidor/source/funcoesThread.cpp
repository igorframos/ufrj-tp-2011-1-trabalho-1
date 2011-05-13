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

#include "../headers/cliente"
#include "../headers/mensagem"
#include "../headers/controle"

namespace funcoesThread
{
    void *envioDeMensagens (void *ptr)
    {
        std::cout << "Thread de envio de mensagens criada corretamente." << std::endl;

        int *sair = (int*) ptr;

        while (!(*sair))
        {
        }

        return NULL;
    }

    void *recebimentoDeConexoes (void *ptr)
    {
        std::cout << "Thread de recebimento de conexões criada corretamente." << std::endl;
        
        Controle *controle = (Controle*) ptr;

        int socketID;
        sockaddr_in serv_addr;
         
        // Abre canal de comunicação externa.
        socketID = socket(AF_INET, SOCK_STREAM, 0);
        if (socketID < 0)
        {
            std::cout << "Criação do socket falhou." << std::endl;
            return NULL;
        }
     
        serv_addr.sin_family = AF_INET;            // Cria um endereço IPv4 local
        serv_addr.sin_port = htons(2010);          // Determina uma porta para a comunicação
        serv_addr.sin_addr.s_addr = INADDR_ANY;    // 
     
        // Liga o canal criado ao endereço da máquina. Usa o sockaddr que foi setado acima
        if (bind(socketID, (const sockaddr*) &serv_addr, sizeof(serv_addr)))
        {
            std::cout << "Impossível abrir canal de comunicação externa. Encerrando a aplicação." << std::endl;
            return NULL;
        }
         
        // Abre o canal para recepção de comunicações externas
        // Determina uma fila de tamanho até 2^16. Se houver 2^16 conexões na fila, novas conexões
        // serão descartadas e a aplicação cliente receberá um código de erro.
        if (listen(socketID, (1<<16)) < 0)
        {
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
        pthread_mutex_lock(&mutex);
        while (!encerra)
        {
            pthread_mutex_unlock(&mutex);
            Cliente *cliente = new Cliente();
            cliente->socketID = accept(socketID, (sockaddr*) &(cliente->addr), &(cliente->size));
            if (cliente->socketID < 0)
            {
                cout << "Impossível receber conexões externas (accept). Encerrando a aplicação." << std::endl;
                return NULL;
            }
            if (pthread_create(&(cliente->thread), NULL, recebe_dados, cliente))
            {
                cout << "Impossível criar thread para tratar conexões externas. Ignorando conexão." << std::endl;
            }
             pthread_mutex_lock(&mutex);
        }
        close(socketID);

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
