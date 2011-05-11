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
     
         // Liga o canal criado ao endereço da máquina.
         if (bind(socketID, (const sockaddr*) &serv_addr, sizeof(serv_addr)))
         {
             puts ("Impossível abrir canal de comunicação externa. Encerrando a aplicação.\n");
             return NULL;
         }
         
         // Abre o canal para recepção de comunicações externas.
         if (listen(socketID, (1<<16)) < 0)
         {
             puts ("Impossível receber conexões externas (listen). Encerrando a aplicação.\n");
             return NULL;
         }
         
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
                 puts ("Impossível receber conexões externas (accept). Encerrando a aplicação.\n");
                 return NULL;
             }
             if (pthread_create(&(cliente->thread), NULL, recebe_dados, cliente))
             {
                 puts ("Impossível criar thread para tratar conexões externas. Ignorando conexão.\n");
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
