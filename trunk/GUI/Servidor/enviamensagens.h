#ifndef ENVIAMENSAGENS_H
#define ENVIAMENSAGENS_H

#include <QThread>

class EnviaMensagens : public QThread
{
    Q_OBJECT
public:
    explicit EnviaMensagens(QObject *parent = 0);

signals:
    void removeUsuario();
    void adicionaUsuario(QString);
    void listaUsuarios();
    void adicionaMensagem(QString);
    void adicionaMsgControle(QString);

public slots:

protected:
    void run();
};

#endif // ENVIAMENSAGENS_H
