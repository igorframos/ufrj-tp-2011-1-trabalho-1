/*
 * 
 * Este arquivo contém o programa principal. Trata-se apenas da função main, que contém as instruções
 * a serem executadas para a criação das threads e fica responsável apenas por verificar se o progra-
 * ma deve ser fechado.
 *
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

// Incluindo a estrutura Controle, que guarda os dados comuns a todas as threads
#include "../headers/controle"

// Incluindo os protótipos da funções que representam as threads
#include "../headers/prototipos"

int main (int argc, char *argv[])
{
    Controle controle(0);       // Cria a estrutura do tipo controle com as informações comuns
                                // a todas as threads do programa.

    pthread_t threadEnvio,      // Apesar de os nomes serem autoexplicativos, trata-se dos ids
              threadConexao;    // das threads criadas pela main.

    if (pthread_create(&threadEnvio, NULL, funcoesThread::envioDeMensagens, (void*) &controle) != 0)
    {
        // Como não faz sentido um servidor que não pode enviar mensagens, o programa é abortado caso não
        // seja possível iniciar a thread que envia as mensagens aos clientes.
        std::cout << "Erro na criação da thread que enviaria mensagens. Programa será abortado." << std::endl;
        exit(1);    
    }
    
    if (pthread_create(&threadConexao, NULL, funcoesThread::recebimentoDeConexoes, (void*) &controle) != 0)
    {
        // Como não faz sentido um servidor que não pode receber mensagens ou iniciar conexões, o programa é 
        // abortado caso não seja possível iniciar a thread que aguarda a conexão dos clientes.
        std::cout << "Erro na criação da thread que receberia conexoes. Programa será abortado." << std::endl;
        exit(1);
    }
    
    // Aguarda o comando que sai do programa. Inicialmente, para fins de teste, está como um char lido da
    // entrada padrão, mas isso deve ser alterado para receber o comando de fechar da interface gráfica.
    while (!controle.sair)
    {
        char linha[1024];
        std::cin.getline(linha, 1024);

        if (!strcmp(linha, "exit_program"))
        {
            controle.sair = 1;
        }
        else
        {
            char nome[32];

            strcpy(nome, "SERVIDOR");

            Mensagem m(nome, linha);

            pthread_mutex_lock(&controle.mutexFilaMensagens);
            controle.filaMensagens.push(m);
            pthread_mutex_unlock(&controle.mutexFilaMensagens);
        }
    }

    std::cout << "Encerrando programa." << std::endl;

    return 0;
}

