/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri May 13 16:57:06 2011
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
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
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
    QAction *actionConectar;
    QAction *actionSair;
    QAction *actionDesconectar;
    QWidget *centralWidget;
    QPlainTextEdit *txt_digita;
    QPlainTextEdit *txt_chat;
    QPlainTextEdit *txt_usuarios;
    QPlainTextEdit *txt_msg_controle;
    QPushButton *btn_enviar;
    QFrame *fr_inicial;
    QLineEdit *line_login;
    QPushButton *btn_entrar;
    QLabel *lbl_usuario;
    QLabel *lbl_max_caracteres;
    QLabel *lbl_usuario_existente;
    QLabel *lbl_escolher_usuario;
    QMenuBar *menuBar;
    QMenu *menuMenu;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(690, 581);
        actionConectar = new QAction(MainWindow);
        actionConectar->setObjectName(QString::fromUtf8("actionConectar"));
        actionSair = new QAction(MainWindow);
        actionSair->setObjectName(QString::fromUtf8("actionSair"));
        actionDesconectar = new QAction(MainWindow);
        actionDesconectar->setObjectName(QString::fromUtf8("actionDesconectar"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        txt_digita = new QPlainTextEdit(centralWidget);
        txt_digita->setObjectName(QString::fromUtf8("txt_digita"));
        txt_digita->setGeometry(QRect(10, 430, 591, 84));
        txt_chat = new QPlainTextEdit(centralWidget);
        txt_chat->setObjectName(QString::fromUtf8("txt_chat"));
        txt_chat->setEnabled(true);
        txt_chat->setGeometry(QRect(10, 10, 431, 411));
        txt_chat->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        txt_usuarios = new QPlainTextEdit(centralWidget);
        txt_usuarios->setObjectName(QString::fromUtf8("txt_usuarios"));
        txt_usuarios->setEnabled(true);
        txt_usuarios->setGeometry(QRect(460, 10, 211, 271));
        txt_usuarios->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        txt_msg_controle = new QPlainTextEdit(centralWidget);
        txt_msg_controle->setObjectName(QString::fromUtf8("txt_msg_controle"));
        txt_msg_controle->setEnabled(true);
        txt_msg_controle->setGeometry(QRect(460, 290, 211, 131));
        txt_msg_controle->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        btn_enviar = new QPushButton(centralWidget);
        btn_enviar->setObjectName(QString::fromUtf8("btn_enviar"));
        btn_enviar->setGeometry(QRect(610, 440, 71, 61));
        btn_enviar->setStyleSheet(QString::fromUtf8(""));
        fr_inicial = new QFrame(centralWidget);
        fr_inicial->setObjectName(QString::fromUtf8("fr_inicial"));
        fr_inicial->setGeometry(QRect(180, 110, 361, 241));
        QFont font;
        font.setFamily(QString::fromUtf8("Helvetica"));
        font.setPointSize(11);
        fr_inicial->setFont(font);
        fr_inicial->setFrameShape(QFrame::StyledPanel);
        fr_inicial->setFrameShadow(QFrame::Raised);
        line_login = new QLineEdit(fr_inicial);
        line_login->setObjectName(QString::fromUtf8("line_login"));
        line_login->setGeometry(QRect(100, 40, 151, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Helvetica"));
        font1.setPointSize(15);
        line_login->setFont(font1);
        btn_entrar = new QPushButton(fr_inicial);
        btn_entrar->setObjectName(QString::fromUtf8("btn_entrar"));
        btn_entrar->setGeometry(QRect(130, 160, 92, 27));
        btn_entrar->setDefault(false);
        lbl_usuario = new QLabel(fr_inicial);
        lbl_usuario->setObjectName(QString::fromUtf8("lbl_usuario"));
        lbl_usuario->setGeometry(QRect(30, 50, 61, 17));
        QFont font2;
        font2.setPointSize(14);
        lbl_usuario->setFont(font2);
        lbl_max_caracteres = new QLabel(fr_inicial);
        lbl_max_caracteres->setObjectName(QString::fromUtf8("lbl_max_caracteres"));
        lbl_max_caracteres->setGeometry(QRect(110, 80, 161, 17));
        QFont font3;
        font3.setPointSize(8);
        lbl_max_caracteres->setFont(font3);
        lbl_usuario_existente = new QLabel(fr_inicial);
        lbl_usuario_existente->setObjectName(QString::fromUtf8("lbl_usuario_existente"));
        lbl_usuario_existente->setGeometry(QRect(10, 100, 351, 31));
        QFont font4;
        font4.setPointSize(9);
        lbl_usuario_existente->setFont(font4);
        lbl_escolher_usuario = new QLabel(fr_inicial);
        lbl_escolher_usuario->setObjectName(QString::fromUtf8("lbl_escolher_usuario"));
        lbl_escolher_usuario->setGeometry(QRect(70, 130, 241, 20));
        lbl_escolher_usuario->setFont(font4);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 690, 25));
        menuMenu = new QMenu(menuBar);
        menuMenu->setObjectName(QString::fromUtf8("menuMenu"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuMenu->menuAction());
        menuMenu->addAction(actionConectar);
        menuMenu->addAction(actionDesconectar);
        menuMenu->addSeparator();
        menuMenu->addAction(actionSair);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionConectar->setText(QApplication::translate("MainWindow", "Conectar", 0, QApplication::UnicodeUTF8));
        actionSair->setText(QApplication::translate("MainWindow", "Sair", 0, QApplication::UnicodeUTF8));
        actionDesconectar->setText(QApplication::translate("MainWindow", "Desconectar", 0, QApplication::UnicodeUTF8));
        btn_enviar->setText(QApplication::translate("MainWindow", "Enviar", 0, QApplication::UnicodeUTF8));
        line_login->setText(QString());
        btn_entrar->setText(QApplication::translate("MainWindow", "Entrar", 0, QApplication::UnicodeUTF8));
        lbl_usuario->setText(QApplication::translate("MainWindow", "Usu\303\241rio", 0, QApplication::UnicodeUTF8));
        lbl_max_caracteres->setText(QApplication::translate("MainWindow", "M\303\241ximo de 20 caracteres", 0, QApplication::UnicodeUTF8));
        lbl_usuario_existente->setText(QApplication::translate("MainWindow", "Esse nome de usu\303\241rio j\303\241 est\303\241 sendo utilizado por outro cliente.", 0, QApplication::UnicodeUTF8));
        lbl_escolher_usuario->setText(QApplication::translate("MainWindow", "Por favor, escolha outro nome de usu\303\241rio.", 0, QApplication::UnicodeUTF8));
        menuMenu->setTitle(QApplication::translate("MainWindow", "Menu", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
