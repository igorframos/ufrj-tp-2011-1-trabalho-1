#include <algorithm>
#include <pthread.h>
#include <set>
#include <sys/socket.h>
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mensagem"
#include "globais.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btn_enviar, SIGNAL(clicked()), this, SLOT(usaBtnEnviar()));
    connect(ui->btn_entrar, SIGNAL(clicked()), this, SLOT(usaBtnEntrar()));

    connect(ui->actionConectar, SIGNAL(triggered()), this, SLOT(mostraTelaLogin()));
    connect(ui->actionDesconectar, SIGNAL(triggered()), this, SLOT(desconecta()));
    connect(ui->actionSair, SIGNAL(triggered()), this, SLOT(close()));

    inicializa();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        if(ui->fr_inicial->isEnabled())
            usaBtnEntrar();
    }
    if(event->key() == Qt::Key_Escape)
    {
        close();
    }
}

void MainWindow::escreveChat(string texto)
{
    if(!texto.empty())
    {
        QString qstring = texto.c_str();

        ui->txt_chat->insertPlainText(qstring);

        ui->txt_chat->insertPlainText("\n");
    }
}

void MainWindow::escreveChat(QString texto)
{
    if(texto.size() > 0)
    {
        ui->txt_chat->insertPlainText(texto);

        ui->txt_chat->insertPlainText("\n");
    }
}

void MainWindow::escreveUsuario(string usuario)
{
    if(!usuario.empty())
    {
        QString qstring = usuario.c_str();

        ui->txt_usuarios->insertPlainText(qstring);

        ui->txt_usuarios->insertPlainText("\n");
    }
}

void MainWindow::escreveUsuario(QString usuario)
{
    if(usuario.size() > 0)
    {
        ui->txt_usuarios->insertPlainText(usuario);

        ui->txt_usuarios->insertPlainText("\n");
    }
}

void MainWindow::removeUsuario()
{
    ui->txt_usuarios->clear();

    pthread_mutex_lock(&controle->mutexListaClientes);
    for (set<string>::iterator i = controle->listaClientes.begin(); i != controle->listaClientes.end(); ++i)
    {
        escreveUsuario(*i);
    }
    pthread_mutex_unlock(&controle->mutexListaClientes);
}

void MainWindow::adicionaListaOnline(QString me)
{
    ui->txt_usuarios->clear();

    pthread_mutex_lock(&controle->mutexListaClientes);
    for (set<string>::iterator i = controle->listaClientes.begin(); i != controle->listaClientes.end(); ++i)
    {
        escreveUsuario(*i);
    }
    pthread_mutex_unlock(&controle->mutexListaClientes);
}

void MainWindow::escreveMsgControle(string msgControle)
{
    if(!msgControle.empty())
    {
        QString qstring = msgControle.c_str();

        ui->txt_msg_controle->insertPlainText(qstring);

        ui->txt_msg_controle->insertPlainText("\n");
    }
}

void MainWindow::escreveMsgControle(QString msgControle)
{
    if(msgControle.size() > 0)
    {
        ui->txt_msg_controle->insertPlainText(msgControle);

        ui->txt_msg_controle->insertPlainText("\n");
    }
}

void MainWindow::usaBtnEnviar()
{
    QString linha = ui->txt_digita->toPlainText();

    Mensagem m;
    strncpy(m.remetente, controle->nome, sizeof(m.remetente));
    strncpy(m.texto, linha.toLocal8Bit(), sizeof(m.texto));

    if (!strcmp(m.texto, "exit_program"))
    {
        strcpy(m.texto, "exit__program");
    }
    else if (strlen(m.texto) && write(controle->socketTCP, &m, sizeof(m)) < 0)
    {
        std::cout << "Mensagem não pôde ser enviada." << std::endl;
    }

    ui->txt_digita->clear();
}

void MainWindow::usaBtnEntrar()
{
    QString nomeUsuario = ui->line_login->text();

    bool nomeInvalido = true;
    char nome[32];
    strncpy(nome, nomeUsuario.toStdString().c_str(), 21);

    std::cout << "Nome escolhido: " << nome << std::endl;

    if (write(controle->socketTCP, nome, 32) < 0)
    {
        ::close(controle->socketTCP);
        std::cout << "Não foi possível enviar o nome para o servidor. Encerrando programa." << std::endl;
        exit(1);
    }

    if (read(controle->socketTCP, &nomeInvalido, sizeof(nomeInvalido)) < 0)
    {
        ::close(controle->socketTCP);
        std::cout << "Não foi possível receber resposta do servidor sobre o nome. Encerrando programa." << std::endl;
        exit(1);
    }

    if (nomeInvalido)
    {
        ui->lbl_escolher_usuario->setVisible(true);
        ui->lbl_usuario_existente->setVisible(true);
    }
    else
    {
        strcpy(controle->nome, nome);
        controle->controleNome.release(1);
        conectado();
    }
}

void MainWindow::mostraTelaLogin()
{
    ui->fr_inicial->setVisible(true);
    ui->line_login->clear();

    ui->actionConectar->setEnabled(false);

    ui->lbl_usuario_existente->setVisible(false);
    ui->lbl_escolher_usuario->setVisible(false);
}

void MainWindow::desconecta()
{
    ui->txt_chat->clear();
    ui->txt_digita->clear();
    ui->txt_msg_controle->clear();
    ui->txt_usuarios->clear();

    Mensagem m;
    strncpy(m.remetente, controle->nome, sizeof(m.remetente));
    strncpy(m.texto, "exit_program", sizeof(m.texto));

    if (write(controle->socketTCP, &m, sizeof(m)) < 0)
    {
        std::cout << "Mensagem não pôde ser enviada." << std::endl;
    }

    ::close(controle->socketTCP);

    inicializa();
}

void MainWindow::inicializa()
{
    ui->fr_inicial->setVisible(false);

    ui->actionDesconectar->setEnabled(false);

    ui->btn_enviar->setVisible(false);
    ui->txt_chat->setVisible(false);
    ui->txt_digita->setVisible(false);
    ui->txt_msg_controle->setVisible(false);
    ui->txt_usuarios->setVisible(false);
}

void MainWindow::conectado()
{
    ui->fr_inicial->setVisible(false);

    ui->actionConectar->setEnabled(false);
    ui->actionDesconectar->setEnabled(true);

    ui->btn_enviar->setVisible(true);
    ui->txt_chat->setVisible(true);
    ui->txt_digita->setVisible(true);
    ui->txt_msg_controle->setVisible(true);
    ui->txt_usuarios->setVisible(true);
}
