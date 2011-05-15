#include "../headers/prototipos"

namespace comunicacao
{
    bool enviaContatos(Controle *controle, Cliente& cliente)
    {
        char nome[32];

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
            return false;
        }

        // Envia os nomes dos clientes online no momento.
        for (std::set<Cliente>::iterator i = controle->listaClientes.begin(); i != controle->listaClientes.end(); ++i)
        {
            // Envia o nome do próximo cliente. Se não chegar, a thread deve ser encerrada.
            strncpy(nome, i->nome.c_str(), sizeof(nome));
            if (write(cliente.socketEnvioID, (void*) nome, sizeof(nome)) < 0)
            {
                std::cout << "Não pude enviar mensagem com o tamanho da lista de usuários para " << cliente.socketEnvioID << ".";
                std::cout << " Encerrando thread." << std::endl;

                // Retira este cliente da lista para encerrar a thread.
                controle->listaClientes.erase(controle->listaClientes.find(cliente.nome));

                pthread_mutex_unlock(&controle->mutexListaClientes);    // Libera lista de clientes.
                return false;
            }
        }

        pthread_mutex_unlock(&controle->mutexListaClientes);    // Libera lista de clientes.

        return true;
    }

    bool recebeNomeUsuario(Controle* controle, Cliente& cliente)
    {
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
                return false;
            }

            // Se o nome for válido, insere o cliente no set com o nome desejado por ele.
            if (!nomeInvalido)
            {
                cliente.nome = std::string(nome);
                controle->listaClientes.insert(cliente);
            }

            pthread_mutex_unlock(&controle->mutexListaClientes);    // Libera a lista de clientes.
        }

        return true;
    }

    void recebeMensagens(Controle* controle, Cliente& cliente)
    {
        Mensagem mensagem;      // Estrutura do tipo mensagem que será recebida do cliente.
        pthread_mutex_lock(&controle->mutexEncerramento);   // Controle de concorrência da variável controle->sair.
        while (!(controle->sair))
        {
            pthread_mutex_unlock(&controle->mutexEncerramento);  // Libera a variável controle->sair.

            read(cliente.socketEnvioID, &mensagem, sizeof(mensagem));  // Recebe uma mensagem do cliente.
            if (!strcmp(mensagem.texto, "exit_program")) break;     // Acordo para dizer que o cliente está desconectando.

            // Controla a concorrência sobre a variável controle->filaMensagens com semáforos.
            // Insere a mensagem recebida na fila de mensagens a serem repassadas aos clientes.
            pthread_mutex_lock(&controle->mutexFilaMensagens);
            controle->filaMensagens.push(mensagem);
            pthread_mutex_unlock(&controle->mutexFilaMensagens);

            pthread_mutex_lock(&controle->mutexEncerramento);   // Controle de concorrência da variável controle->sair.
        }
        pthread_mutex_unlock(&controle->mutexEncerramento); // Libera variável controle->sair.
    }

    bool criaSocket(Controle* controle, int& socketID, sockaddr_in& serv_addr)
    {
        const int porta = 2011;     // Constante com o número da porta que deve ser usada na comunicação.

        // Abre canal de comunicação externa com protocolo TCP.
        socketID = socket(AF_INET, SOCK_STREAM, 0);
        if (socketID < 0)
        {
            std::cout << "Criação do socket falhou." << std::endl;
            return false;
        }

        serv_addr.sin_family = AF_INET;            // Determina a família de sockets.
        serv_addr.sin_port = htons(porta);         // Determina uma porta para a comunicação.
        serv_addr.sin_addr.s_addr = INADDR_ANY;    // Recebe pacotes para qualquer IP da máquina.

        // Liga o canal criado ao endereço da máquina. Usa o sockaddr que foi criado acima.
        if (bind(socketID, (const sockaddr*) &serv_addr, sizeof(serv_addr)))
        {
            // Thread aborta se não puder receber conexões externas.
            std::cout << "Impossível abrir canal de comunicação externa (bind). Encerrando a aplicação." << std::endl;
            return false;
        }

        // Abre o canal para recepção de comunicações externas.
        // Determina uma fila de tamanho até 2^16. Se houver 2^16 conexões na fila, novas conexões
        // serão descartadas e a aplicação cliente receberá um código de erro.
        if (listen(socketID, (1<<16)) < 0)
        {
            // Thread aborta se não for possível ficar escutando no socket criado.
            std::cout << "Impossível receber conexões externas (listen). Encerrando a aplicação." << std::endl;
            return false;
        }

        return true;
    }

    bool recebeConexoes(Controle* controle, const int socketID)
    {
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
                return false;
            }

            Dados dados(cliente, controle);     // Cria uma estrutura para transferir dados para a nova thread.
            // Os dados são as informações da conexão com o cliente e as
            // variáveis de controle.

            // Cria a thread que vai lidar especificamente com o cliente atual.
            if (pthread_create(&(cliente->thread), NULL, funcoesThread::recebimentoDeMensagens, (void*) &dados))
            {
                std::cout << "Não é possível criar thread para o cliente. Ignorando conexão." << std::endl;
            }

            pthread_mutex_lock(&controle->mutexEncerramento);   // Controle de concorrência na variável controle->sair.
        }
        pthread_mutex_unlock(&controle->mutexEncerramento); // Libera variável controle->sair.

        return true;
    }
}

