/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon May 16 18:50:32 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      27,   11,   11,   11, 0x08,
      42,   11,   11,   11, 0x08,
      60,   11,   11,   11, 0x08,
      73,   11,   11,   11, 0x08,
      93,   11,   11,   11, 0x08,
     114,   11,   11,   11, 0x08,
     137,   11,   11,   11, 0x08,
     161,   11,   11,   11, 0x08,
     188,   11,   11,   11, 0x08,
     216,   11,   11,   11, 0x08,
     232,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0usaBtnEnviar()\0usaBtnEntrar()\0"
    "mostraTelaLogin()\0desconecta()\0"
    "escreveChat(string)\0escreveChat(QString)\0"
    "escreveUsuario(string)\0escreveUsuario(QString)\0"
    "escreveMsgControle(string)\0"
    "escreveMsgControle(QString)\0removeUsuario()\0"
    "adicionaListaOnline(QString)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: usaBtnEnviar(); break;
        case 1: usaBtnEntrar(); break;
        case 2: mostraTelaLogin(); break;
        case 3: desconecta(); break;
        case 4: escreveChat((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 5: escreveChat((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: escreveUsuario((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 7: escreveUsuario((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: escreveMsgControle((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 9: escreveMsgControle((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: removeUsuario(); break;
        case 11: adicionaListaOnline((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
