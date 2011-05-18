#-------------------------------------------------
#
# Project created by QtCreator 2011-05-12T17:44:49
#
#-------------------------------------------------

TARGET = TP_Servidor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    funcoesThread.cpp \
    comunicacao.cpp \
    cliente.cpp \
    enviamensagens.cpp \
    recebeconexoes.cpp

HEADERS  += mainwindow.h \
    prototipos \
    mensagem \
    dados \
    controle \
    cliente \
    globais.h \
    cliente.h \
    enviamensagens.h \
    recebeconexoes.h

FORMS    += mainwindow.ui
