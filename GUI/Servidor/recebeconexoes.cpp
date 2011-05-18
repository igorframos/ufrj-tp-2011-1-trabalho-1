#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "cliente.h"
#include "controle"
#include "prototipos"
#include "recebeconexoes.h"

extern Controle *controle;

RecebeConexoes::RecebeConexoes(QObject *parent) :
    QThread(parent)
{
}

void RecebeConexoes::run()
{
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
    exit(0);
}
