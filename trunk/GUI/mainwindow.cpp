#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    if(event->key() == Qt::Key_Enter)
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
    escreveChat(ui->txt_digita->toPlainText());

    ui->txt_digita->clear();
}

void MainWindow::usaBtnEntrar()
{
    /*if(CONDICAO PRA LOGIN ESTAR CERTO)
    {*/
        conectado();
    /*}
    else
    {
        ui->lbl_escolher_usuario->setVisible(true);
        ui->lbl_usuario_existente->setVisible(true);
    }*/
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

    inicializa();
    ui->actionConectar->setEnabled(true);
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
