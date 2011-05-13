/*
 * Funções usadas nas threads do programa.
 *
 * Envio de mensagens, recebimento de conexões e recebimento de mensagens são as tarefas que devem
 * ser executadas pelas threads.
 *
 * Falta fazer o pedaço de enviar mensagens via UDP e enviar para o cliente a questão de se inscrever
 * no grupo UDP.
 *
 * BUGS:
 *  - Recebimento de conexões não está recebendo o sinal de finalizar o programa. Provavelmente
 *    está presa no accept de novas conexões. Ainda não sei como resolver.
 *  - Possivelmente, semáforos.  
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
#include <netinet/in.h>

#include "../headers/dados"
#include "../headers/cliente"
#include "../headers/mensagem"
#include "../headers/controle"
#include "../headers/prototipos"

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

        pthread_mutex_lock(&controle->mutexEncerramento);
        while (!(controle->sair))
        {
            pthread_mutex_unlock(&controle->mutexEncerramento);

            pthread_mutex_lock(&controle->mutexFilaMensagens);  // Trava a fila de mensagens (controle de concorrência)

            while (!(controle->filaMensagens.empty()))          // Enquanto a fila de mensagens contiver alguém
            {
                Mensagem m = controle->filaMensagens.front();             // Retirar primeira mensagem da fila e enviar
                controle->filaMensagens.pop();

                // Falta aqui o código que envia de fato uma mensagem.
            }
            pthread_mutex_unlock(&controle->mutexFilaMensagens);    // Destrava a fila de mensagens

            pthread_mutex_lock(&controle->mutexEncerramento);
        }
        pthread_mutex_unlock(&controle->mutexEncerramento);

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

        // Abre canal de comunicação externa com protocolo TCP.
        socketID = socket(AF_INET, SOCK_STREAM, 0);
        if (socketID < 0)
        {
            std::cout << "Criação do socket falhou." << std::endl;
            return NULL;
        }

        serv_addr.sin_family = AF_INET;            // Determina a família de sockets.
        serv_addr.sin_port = htons(porta);         // Determina uma porta para a comunicação.
        serv_addr.sin_addr.s_addr = INADDR_ANY;    // Recebe pacotes para qualquer IP da máquina.

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

        // Para cada conexão recebida cria uma nova thread para lidar com ela e volta a esperar
        // nova conexão externa.
        pthread_mutex_lock(&controle->mutexEncerramento);   // Controle de concorrência na variável controle->sair.
        while (!controle->sair)
        {
            pthread_mutex_unlock(&controle->mutexEncerramento); // Libera a variável controle->sair.

            Cliente *cliente = new Cliente();       // Cria um novo cliente e aguarda uma conexão.
            cliente->socketEnvioID = accept(socketID, (sockaddr*) &(cliente->endereco), &(cliente->tamanho));
            if (cliente->socketEnvioID < 0)
            {
                std::cout << "Falha na recepção de novas conexões. Encerrando thread. (accept)" << std::endl;
                return NULL;
            }

            Dados dados(cliente, controle);     // Cria uma estrutura para transferir dados para a nova thread.
            // Os dados são as informações da conexão com o cliente e as
            // variáveis de controle.

                                                // Cria a thread que vai lidar especificamente com o cliente atual.
            if (pthread_create(&(cliente->thread), NULL, recebimentoDeMensagens, (void*) &dados))
            {
                std::cout << "Não é possível criar thread para o cliente. Ignorando conexão." << std::endl;
            }

            pthread_mutex_lock(&controle->mutexEncerramento);   // Controle de concorrência na variável controle->sair.
        }
        pthread_mutex_unlock(&controle->mutexEncerramento);

        std::cout << "Thread recebimentoDeConexoes encerrada." << std::endl;
        return NULL;
    }

    void *recebimentoDeMensagens (void *ptr)
    {
        std::cout << "Thread de recebimento de mensagens criada corretamente. Uma conexão foi estabelecida." << std::endl;

        // Recebe os dados e libera a memória da variável que foi usada só para passar essas informações.
        Dados *dados = (Dados*) ptr;
        Cliente cliente = *(dados->cliente);
        Controle *controle = dados->controle;
        delete dados;

        /*
         * FIX
         *
         * Resolver problema de cliente lento travando conexão de todos os outros clientes.
         * Tomar cuidado com cliente lento x cliente rápido tomando o nome que ele vai escolher.
         */

        // Recebe o nome de usuário, verifica sua unicidade e, caso não seja único, avisa o cliente para
        // que este escolha outro.
        char nome[32];              // Armazena em caráter temporário nomes de clientes.
        bool nomeInvalido = true;   // Variável que determina se o nome é válido.
        while (nomeInvalido)
        {    
            read(cliente.socketEnvioID, nome, sizeof(nome));    // Recebe o nome desejado pelo cliente.

            pthread_mutex_lock(&controle->mutexListaClientes);  // Controle de concorrência da lista de clientes.

            // Verifica a existência de um cliente com o mesmo nome.
            nomeInvalido = controle->listaClientes.find(Cliente(nome)) != controle->listaClientes.end();

            // Envia para o cliente o resultado da consulta. Encerra a thread caso não consiga se comunicar.
            if (write(cliente.socketEnvioID, (void*) &nomeInvalido, sizeof(nomeInvalido)) < 0)
            {
                std::cout << "Cliente " << cliente.socketEnvioID << " não recebe resposta. Encerrando thread." << std::endl;
                pthread_mutex_unlock(&controle->mutexListaClientes);
                return NULL;
            }

            // Se o nome for válido, insere o cliente no set com o nome desejado por ele.
            if (!nomeInvalido)
            {
                cliente.nome = std::string(nome);
                controle->listaClientes.insert(cliente);
            }

            pthread_mutex_unlock(&controle->mutexListaClientes);    // Libera a lista de clientes.
        }

        pthread_mutex_lock(&controle->mutexListaClientes);  // Controle de concorrência na lista de clientes.
        int tamanhoListaUsuarios = controle->listaClientes.size(); // Tamanho da lista de usuários.

        // Envia para o cliente o número de clientes online no momento.
        if (write(cliente.socketEnvioID, (void*) &tamanhoListaUsuarios, sizeof(tamanhoListaUsuarios)) < 0)
        {
            // Se não puder enviar, assume que o cliente desapareceu e sai da thread.
            std::cout << "Não pude enviar mensagem com o tamanho da lista de usuários para " << cliente.socketEnvioID << ".";
            std::cout << " Encerrando thread." << std::endl;

            // Retira este cliente da lista para encerrar a thread.
            controle->listaClientes.erase(controle->listaClientes.find(cliente.nome));

            pthread_mutex_unlock(&controle->mutexListaClientes);
            return NULL;
        }

        // Envia os nomes dos clientes online no momento.
        for (std::set<Cliente>::iterator i = controle->listaClientes.begin(); i != controle->listaClientes.end(); ++i)
        {
            strncpy(nome, i->nome.c_str(), sizeof(nome));
            if (write(cliente.socketEnvioID, (void*) nome, sizeof(nome)) < 0)
            {
                std::cout << "Não pude enviar mensagem com o tamanho da lista de usuários para " << cliente.socketEnvioID << ".";
                std::cout << " Encerrando thread." << std::endl;

                // Retira este cliente da lista para encerrar a thread.
                controle->listaClientes.erase(controle->listaClientes.find(cliente.nome));

                pthread_mutex_unlock(&controle->mutexListaClientes);
                return NULL;
            }
        }

        pthread_mutex_unlock(&controle->mutexListaClientes);

        // Só falta aqui mandar a parada do UDP, seja lá como se faz isso.

        Mensagem mensagem;      // Estrutura do tipo mensagem que será recebida do cliente.
        pthread_mutex_lock(&controle->mutexEncerramento);   // Controle de concorrência da variável controle->sair.
        while (!(controle->sair))
        {
            pthread_mutex_unlock(&controle->mutexEncerramento);  // Libera a variável controle->sair.

            read(cliente.socketEnvioID, &mensagem, sizeof(mensagem));  // Recebe uma mensagem do cliente.
            if (mensagem.texto[0] == -1) break;     // Acordo para dizer que o cliente está desconectando.

            // Controla a concorrência sobre a variável controle->filaMensagens com semáforos.
            // Insere a mensagem recebida na fila de mensagens a serem repassadas aos clientes.
            pthread_mutex_lock(&controle->mutexFilaMensagens);
            controle->filaMensagens.push(mensagem);
            pthread_mutex_unlock(&controle->mutexFilaMensagens);

            pthread_mutex_lock(&controle->mutexEncerramento);   // Controle de concorrência da variável controle->sair.
        }
        pthread_mutex_unlock(&controle->mutexEncerramento);

        return NULL;
    }
}
