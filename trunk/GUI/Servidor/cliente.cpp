#include "cliente.h"

Cliente::Cliente(const std::string nome, const int socketEnvioID): nome(nome), socketEnvioID(socketEnvioID)
{
    tamanho = sizeof(endereco);
}

bool operator<(const Cliente& a, const Cliente& b)
{
    return a.nome < b.nome;
}

