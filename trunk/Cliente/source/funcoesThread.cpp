/*
 *
 * Algoritmo de detecção de que uma mensagem não foi enviada:
 *  Envia mensagem
 *  Como o envio é via TCP, isso é garantido
 *  Assim que a mensagem for enviada, inicia um timer
 *  Se o timer acabar antes de receber a mensagem de volta, significa que a mensagem não foi enviada e que, portanto, o 
 *      usuário deve ser avisado
 *  Se o timer não acabar antes de a mensagem chegar de volta, significa que a mensagem foi enviada e recebida ok e, portanto,
 *      não tenho que avisar nada ao usuário
 * 
 * Ideia de funcionamento desse programa Cliente:
 *  Cliente tem uma conexão TCP que envia mensagens para o Servidor.
 *  Cliente tem uma conexão UDP que recebe mensagens do Servidor.
 *  Quando um cliente deseja enviar uma mensagem, ele a envia ao Servidor que a repassa a todos os Clientes (inclusive ao próprio
 *      que enviou para ele) via multicast.
 *  O Cliente recebe a mensagem e acrescenta à janela de mensagens recebidas. Uma mensagem enviada, para a janela de mensagens
 *      recebidas, não existe. Esta só acrescenta uma nova mensagem quando ela chega do Servidor, ou seja, quando os destinatários não
 *      receberem a mensagem, exibe-se um alerta e o cliente vê a própria mensagem anexada ao final da conversa.
 *
 *  Estou "estudando" agora como fazer a conexão e o armazenamento dos nomes de usuários no Servidor. Estudando entre aspas porque,
 *      na verdade, estou apenas pensando a respeito. Não acho que tenha uma fórmula secreta, então não tenho que estudar, só decidir
 *      como vai ser feito e implementar.
 *
 *  Além disso, eu não tenho muita ideia de como vai funcionar. Não tenho a menor ideia de como será feita a interface gráfica ainda e nem
 *      de como ela vai se comunicar com o programa, mas acho que isso não deve ser muito complicado uma vez que vamos usar Qt.
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
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/udp.h>
namespace funcoesThread
{
    // Estabelece uma conexão TCP com o servidor para enviar as msgs e fica aguardando mensagens para serem enviadas.
    void *envioDeMensagens (void *ptr)
    {
        std::cout << "Thread de envio de mensagens criada corretamente." << std::endl;
        
        int *sair = (int*) ptr;

        int sockfd;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        while (!(*sair))
        {
        }
        
        return NULL;
    }
    
    // Estabelece uma conexão UDP com o servidor para receber as msgs enviadas via multicast.
    void *recebimentoDeMensagens (void *ptr)
    {
        std::cout << "Thread de recebimento de mensagens criada corretamente." << std::endl;
        
        int *sair = (int*) ptr;
        
        // Código copiado quase sem entendimento para tentar entender. Não tô entendendo! <o>
        
        int sockfd;
        struct sockaddr_in sockaddr_group;
        struct hostent *group;
        struct ip_mreq mreq;
        
        std::cout << "hey!" << std::endl;
        
        sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Cria um socket UDP. Essa parte é padrão e fácil.
        
        std::cout << "yo o/" << std::endl;

        bzero(&mreq,sizeof(struct ip_mreq)); // Está zerando o mreq, ainda que eu não saiba o que ele é.

        // set group

        std::cout << "olá" << std::endl;
        
	// Tenta pegar o host pelo nome para entrar no grupo dele. Não é bem o que a gente quer fazer, mas talvez seja
	// parecido. Vou ver como isso foi feito no trabalho de SO.
    // Provavelmente é aqui que está falhando, uma vez que estou sem internet e, com isso, não tenho como acessar
	// nenhum DNS.
	if ((group = gethostbyname("localhost")) == (struct hostent *)0) {
            std::cout << "gethostbyname error: fails for host " << "localhost" << std::endl;
            *sair = 1;
        }
        
        std::cout << "gaga" << std::endl;

        struct in_addr ia;
        bcopy((void*)group->h_addr, (void*)&ia, group->h_length); // Copia as informações do grupo pro in_addr ia.
        
        bcopy(&ia, &mreq.imr_multiaddr.s_addr, sizeof(struct in_addr)); // Copia as informações de ia para o mreq.
									                                    // Pelo que parece, é onde ficam guardados os endereços.

        // set interface

        mreq.imr_interface.s_addr = htonl(INADDR_ANY); // Não tenho ideia do que essa linha faz.

        // do membership call

	    // Pelo comentário que já tinha aqui e pela chamada da função, parece que é aqui que o cara pede pra fazer parte do grupo.
        if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) == -1) {
            std::cerr << "error: setsockopt(IP_ADD_MEMBERSHIP) fails with errno " << errno << std::endl;
            *sair = 1;
        }
        
        std::cout << "oi" << std::endl;
        
        while (!(*sair))
        {
        }
        
        return NULL;
    }
}
