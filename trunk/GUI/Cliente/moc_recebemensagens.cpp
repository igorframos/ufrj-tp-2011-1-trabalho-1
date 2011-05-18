/****************************************************************************
** Meta object code from reading C++ file 'recebemensagens.h'
**
** Created: Mon May 16 18:31:11 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "recebemensagens.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recebemensagens.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RecebeMensagens[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      50,   16,   16,   16, 0x05,
      75,   16,   16,   16, 0x05,
     101,   16,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_RecebeMensagens[] = {
    "RecebeMensagens\0\0adicionaMensagemUsuario(QString)\0"
    "adicionaUsuario(QString)\0"
    "adicionaControle(QString)\0removeUsuario()\0"
};

const QMetaObject RecebeMensagens::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_RecebeMensagens,
      qt_meta_data_RecebeMensagens, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RecebeMensagens::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RecebeMensagens::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RecebeMensagens::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RecebeMensagens))
        return static_cast<void*>(const_cast< RecebeMensagens*>(this));
    return QThread::qt_metacast(_clname);
}

int RecebeMensagens::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: adicionaMensagemUsuario((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: adicionaUsuario((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: adicionaControle((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: removeUsuario(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void RecebeMensagens::adicionaMensagemUsuario(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RecebeMensagens::adicionaUsuario(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RecebeMensagens::adicionaControle(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RecebeMensagens::removeUsuario()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
