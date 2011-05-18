#ifndef ENVIAMENSAGENS_H
#define ENVIAMENSAGENS_H

#include <QThread>

class EnviaMensagens : public QThread
{
    Q_OBJECT
public:
    explicit EnviaMensagens(QObject *parent = 0);

signals:
    void adicionaListaOnline(QString);

public slots:

protected:
    void run();

};

#endif // ENVIAMENSAGENS_H
