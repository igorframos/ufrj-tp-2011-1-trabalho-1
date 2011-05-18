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

#include "mainwindow.h"
#include "controle"
#include "globais.h"
#include "mensagem"
#include "prototipos"
#include "dados"
#include "enviamensagens.h"
#include "recebeconexoes.h"

Controle *controle;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    w->show();

    controle = new Controle(0);

    EnviaMensagens threadEnvio;
    RecebeConexoes threadRecebimentoConexoes;

    threadEnvio.start();
    threadRecebimentoConexoes.start();

    QObject::connect(&threadEnvio, SIGNAL(adicionaMensagem(QString)), w, SLOT(escreveChat(QString)));
    QObject::connect(&threadEnvio, SIGNAL(removeUsuario()), w, SLOT(removeUsuario()));
    QObject::connect(&threadEnvio, SIGNAL(adicionaUsuario(QString)), w, SLOT(escreveUsuario(QString)));
    QObject::connect(&threadEnvio, SIGNAL(adicionaMsgControle(QString)), w, SLOT(escreveMsgControle(QString)));

    return a.exec();
}
