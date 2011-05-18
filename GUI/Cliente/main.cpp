#include <QtGui/QApplication>
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
#include <arpa/inet.h>
#include <netinet/in.h>

#include "controle"
#include "prototipos"
#include "mensagem"
#include "mainwindow.h"
#include "enviamensagens.h"
#include "recebemensagens.h"
#include "globais.h"

Controle *controle;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    w->show();

    controle = new Controle(0, "localhost", w);

    EnviaMensagens threadEnvio;
    RecebeMensagens threadRecebimento;

    threadEnvio.start();
    threadRecebimento.start();

    QObject::connect(&threadRecebimento, SIGNAL(adicionaMensagemUsuario(QString)), w, SLOT(escreveChat(QString)));
    QObject::connect(&threadRecebimento, SIGNAL(adicionaUsuario(QString)), w, SLOT(escreveUsuario(QString)));
    QObject::connect(&threadRecebimento, SIGNAL(adicionaControle(QString)), w, SLOT(escreveMsgControle(QString)));
    QObject::connect(&threadRecebimento, SIGNAL(removeUsuario()), w, SLOT(removeUsuario()));
    QObject::connect(&threadEnvio, SIGNAL(adicionaListaOnline(QString)), w, SLOT(adicionaListaOnline(QString)));

    return a.exec();
}

