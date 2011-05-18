#include <QDebug>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include "controle"
#include "mensagem"
#include "recebemensagens.h"
#include "globais.h"

extern Controle *controle;

RecebeMensagens::RecebeMensagens(QObject *parent) :
    QThread(parent)
{
}

// Estabelece conexão UDP com o servidor para receber as mensagens.
void RecebeMensagens::run()
{
    int socketID;               // Identificador do socket UDP.
    ip_mreq grupo;              // Informações do grupo para que seja possível adicionar este programa ao mesmo.
    sockaddr_in socketLocal;    // Informações sobre o socket local usado para a comunicação com o grupo.
    Mensagem m;                 // Mensagem que será recebida.
    const int porta = 2012;     // A comunicação do grupo se dará pela porta 2012.
    char enderecoGrupo[] = "230.145.0.1";   // Endereço IP do grupo. Deve ser na faixa de 224.0.1.0 a 239.255.255.255.
    socklen_t addrlen;          // Variável com o tamanho do endereço do grupo para ser passada ao recvfrom.

    // Cria socket UDP.
    socketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (socketID < 0)
    {
        std::cout << "Impossível abrir socket UDP para receber mensagens. Encerrando programa." << std::endl;
        exit(1);
    }

    // Determina que este socket pode ser reutilizado.
    int reuse = 1;
    if (setsockopt(socketID, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(reuse)) < 0)
    {
        close(socketID);
        std::cout << "Erro ao determinar reuso do socket. Encerrando programa." << std::endl;
        exit(1);
    }

    // Determina detalhes sobre o socket local e cria o endereço de fato.
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

    // Entra no grupo para que seja possível receber as mensagens enviadas para ele.
    grupo.imr_multiaddr.s_addr = inet_addr(enderecoGrupo);
    grupo.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(socketID, IPPROTO_IP, IP_ADD_MEMBERSHIP, &grupo, sizeof(grupo)) < 0)
    {
        close(socketID);
        std::cout << "Erro ao entrar no grupo de multicasting. Encerrando programa." << std::endl;
        exit(1);
    }

    // Recebe uma mensagem e imprime no console. Caso ser um aviso de que um cliente desconectou, o remove da
    // lista de clientes e imprime a lista atual.
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

            std::set<std::string>::iterator it = controle->listaClientes.find(m.remetente);
            if (it != controle->listaClientes.end())
            {
                std::cout << m.remetente << " desconectou-se." << std::endl;
                controle->listaClientes.erase(it);
            }
            pthread_mutex_unlock(&controle->mutexListaClientes);
            emit removeUsuario();
            QString s = m.remetente;
            s += " desconectou-se.";
            emit adicionaControle(s);
        }
        else if (!strcmp(m.texto, "new_client_connecting"))
        {
            pthread_mutex_lock(&controle->mutexListaClientes);
            controle->listaClientes.insert(m.remetente);
            pthread_mutex_unlock(&controle->mutexListaClientes);

            QString s = m.remetente;
            if (strcmp(m.remetente, controle->nome))
            {
                emit adicionaUsuario(s);
            }

            s += " conectou-se.";
            emit adicionaControle(s);

            std::cout << m.remetente << " conectou-se." << std::endl;
        }
        else
        {
            QString s = m.remetente;
            s += ">> ";
            s += m.texto;
            emit adicionaMensagemUsuario(s);
            std::cout << m.remetente << ">> " << m.texto << std::endl;
        }
    }

    close(socketID);
    exit(0);
}
