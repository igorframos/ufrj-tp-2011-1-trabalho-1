#ifndef RECEBECONEXOES_H
#define RECEBECONEXOES_H

#include <QThread>

class RecebeConexoes : public QThread
{
    Q_OBJECT
public:
    explicit RecebeConexoes(QObject *parent = 0);

signals:

public slots:

protected:
    void run();
};

#endif // RECEBECONEXOES_H
