/****************************************************************************
** Meta object code from reading C++ file 'enviamensagens.h'
**
** Created: Mon May 16 21:52:18 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "enviamensagens.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'enviamensagens.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EnviaMensagens[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      32,   15,   15,   15, 0x05,
      57,   15,   15,   15, 0x05,
      73,   15,   15,   15, 0x05,
      99,   15,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_EnviaMensagens[] = {
    "EnviaMensagens\0\0removeUsuario()\0"
    "adicionaUsuario(QString)\0listaUsuarios()\0"
    "adicionaMensagem(QString)\0"
    "adicionaMsgControle(QString)\0"
};

const QMetaObject EnviaMensagens::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_EnviaMensagens,
      qt_meta_data_EnviaMensagens, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EnviaMensagens::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EnviaMensagens::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EnviaMensagens::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EnviaMensagens))
        return static_cast<void*>(const_cast< EnviaMensagens*>(this));
    return QThread::qt_metacast(_clname);
}

int EnviaMensagens::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: removeUsuario(); break;
        case 1: adicionaUsuario((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: listaUsuarios(); break;
        case 3: adicionaMensagem((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: adicionaMsgControle((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void EnviaMensagens::removeUsuario()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void EnviaMensagens::adicionaUsuario(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EnviaMensagens::listaUsuarios()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void EnviaMensagens::adicionaMensagem(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void EnviaMensagens::adicionaMsgControle(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
