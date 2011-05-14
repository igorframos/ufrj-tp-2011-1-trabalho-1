/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri May 13 13:45:50 2011
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSair;
    QWidget *centralWidget;
    QPlainTextEdit *txt_chat;
    QPlainTextEdit *txt_usuarios;
    QPlainTextEdit *txt_msg_controle;
    QPlainTextEdit *txt_digita;
    QPushButton *btn_enviar;
    QMenuBar *menuBar;
    QMenu *menuMenu;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(636, 522);
        actionSair = new QAction(MainWindow);
        actionSair->setObjectName(QString::fromUtf8("actionSair"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        txt_chat = new QPlainTextEdit(centralWidget);
        txt_chat->setObjectName(QString::fromUtf8("txt_chat"));
        txt_chat->setGeometry(QRect(10, 10, 381, 341));
        txt_chat->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        txt_usuarios = new QPlainTextEdit(centralWidget);
        txt_usuarios->setObjectName(QString::fromUtf8("txt_usuarios"));
        txt_usuarios->setGeometry(QRect(410, 10, 191, 171));
        txt_usuarios->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        txt_msg_controle = new QPlainTextEdit(centralWidget);
        txt_msg_controle->setObjectName(QString::fromUtf8("txt_msg_controle"));
        txt_msg_controle->setGeometry(QRect(410, 190, 191, 161));
        txt_msg_controle->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        txt_digita = new QPlainTextEdit(centralWidget);
        txt_digita->setObjectName(QString::fromUtf8("txt_digita"));
        txt_digita->setGeometry(QRect(10, 360, 491, 84));
        btn_enviar = new QPushButton(centralWidget);
        btn_enviar->setObjectName(QString::fromUtf8("btn_enviar"));
        btn_enviar->setGeometry(QRect(520, 370, 81, 71));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 636, 25));
        menuMenu = new QMenu(menuBar);
        menuMenu->setObjectName(QString::fromUtf8("menuMenu"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuMenu->menuAction());
        menuMenu->addSeparator();
        menuMenu->addAction(actionSair);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionSair->setText(QApplication::translate("MainWindow", "Sair", 0, QApplication::UnicodeUTF8));
        btn_enviar->setText(QApplication::translate("MainWindow", "Enviar", 0, QApplication::UnicodeUTF8));
        menuMenu->setTitle(QApplication::translate("MainWindow", "Menu", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
