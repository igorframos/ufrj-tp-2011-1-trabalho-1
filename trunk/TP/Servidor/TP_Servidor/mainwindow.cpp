#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btn_enviar, SIGNAL(clicked()), this, SLOT(usaBtnEnviar()));
    connect(ui->actionSair, SIGNAL(triggered()), this, SLOT(close()));
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
