#ifndef RECEBEMENSAGENS_H
#define RECEBEMENSAGENS_H

#include <QThread>

class RecebeMensagens : public QThread
{
    Q_OBJECT
public:
    explicit RecebeMensagens(QObject *parent = 0);

signals:
    void adicionaMensagemUsuario(QString);
    void adicionaUsuario(QString);
    void adicionaControle(QString);
    void removeUsuario();

public slots:

protected:
    void run();

};

#endif // RECEBEMENSAGENS_H
