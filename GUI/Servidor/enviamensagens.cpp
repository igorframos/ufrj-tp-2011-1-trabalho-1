#include <pthread.h>
#include <sys/types.h>
#include <netdb.h>
#include <set>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#include "enviamensagens.h"
#include "controle"

extern Controle *controle;

EnviaMensagens::EnviaMensagens(QObject *parent) :
    QThread(parent)
{
}

void EnviaMensagens::run()
{
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
                emit removeUsuario();
                QString s = m.remetente;
                s += " desconectou-se.";
                emit adicionaMsgControle(s);
            }
            else if (!strcmp(m.texto, "new_client_connecting"))
            {
                QString s = m.remetente;
                std::cout << m.remetente << " conectou-se." << std::endl;
                emit adicionaUsuario(s);
                s += " conectou-se.";
                emit adicionaMsgControle(s);
            }
            else
            {
                QString s = m.remetente;
                s += ">> ";
                s += m.texto;
                std::cout << m.remetente << ">> " << m.texto << std::endl;
                emit adicionaMensagem(s);
            }
        }
        pthread_mutex_unlock(&controle->mutexFilaMensagens);    // Destrava a fila de mensagens.
    }

    close(socketID);
    std::cout << "Thread envioDeMensagens encerrada." << std::endl;
    exit(0);
}
