#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include "controle"
#include "mensagem"
#include "prototipos"
#include "enviamensagens.h"

extern Controle *controle;

EnviaMensagens::EnviaMensagens(QObject *parent) :
    QThread(parent)
{
}

// Estabelece uma conexão TCP com o servidor para enviar as msgs e fica aguardando mensagens para serem enviadas.
void EnviaMensagens::run()
{
    int socketID;                       // Identificador do socket TCP.
    hostent *servidor;                  // Host da comunicação.
    const int porta = 2011;             // Comunicação TCP será feita na porta 2011.
    sockaddr_in enderecoServidor;       // Informações sobre o servidor.

    // Cria o socket TCP a ser usado no envio de mensagens.
    socketID = socket(AF_INET, SOCK_STREAM, 0);
    if (socketID < 0)
    {
        std::cout << "Impossível abrir socket para comunicação externa. Encerrando programa." << std::endl;
        exit(1);
    }

    controle->socketTCP = socketID;

    // Encontrando informações sobre o servidor de verdade via DNS.
    servidor = gethostbyname(controle->host);
    if (!servidor)
    {
        close(socketID);
        std::cout << "Servidor não encontrado. Encerrando programa." << std::endl;
        exit(1);
    }

    // Determinando detalhes sobre o servidor.
    bzero(&enderecoServidor, sizeof(enderecoServidor));
    enderecoServidor.sin_family = AF_INET;
    enderecoServidor.sin_port = htons(porta);
    bcopy((char*) servidor->h_addr, (char*) &enderecoServidor.sin_addr.s_addr, servidor->h_length);

    // Conectando ao servidor.
    if (::connect(socketID, (sockaddr*) &enderecoServidor, sizeof(enderecoServidor)) < 0)
    {
        close(socketID);
        std::cout << "Impossível estabelecer conexão com o servidor. Encerrando programa." << std::endl;
        exit(1);
    }

    // Esperar o sinal de que o nome é válido.
    controle->controleNome.acquire(1);

    // Recebe a lista de clientes do servidor.
    comunicacao::recebeListaClientes(socketID);

    emit adicionaListaOnline(QString(controle->nome));

    // Envia mensagens para o servidor.
    while(1);

    exit(0);

}
