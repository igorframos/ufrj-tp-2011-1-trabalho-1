#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QKeyEvent>

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;

private slots:
    void usaBtnEnviar();

    void removeUsuario();
    void escreveChat(string);
    void escreveChat(QString);
    void escreveUsuario(string);
    void escreveUsuario(QString);
    void escreveMsgControle(string);
    void escreveMsgControle(QString);


};

#endif // MAINWINDOW_H
