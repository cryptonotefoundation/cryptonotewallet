/****************************************************************************
** Meta object code from reading C++ file 'PendingTransaction.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/libwalletqt/PendingTransaction.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PendingTransaction.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PendingTransaction_t {
    QByteArrayData data[20];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PendingTransaction_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PendingTransaction_t qt_meta_stringdata_PendingTransaction = {
    {
QT_MOC_LITERAL(0, 0, 18), // "PendingTransaction"
QT_MOC_LITERAL(1, 19, 6), // "commit"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 11), // "setFilename"
QT_MOC_LITERAL(4, 39, 8), // "fileName"
QT_MOC_LITERAL(5, 48, 6), // "status"
QT_MOC_LITERAL(6, 55, 6), // "Status"
QT_MOC_LITERAL(7, 62, 11), // "errorString"
QT_MOC_LITERAL(8, 74, 6), // "amount"
QT_MOC_LITERAL(9, 81, 4), // "dust"
QT_MOC_LITERAL(10, 86, 3), // "fee"
QT_MOC_LITERAL(11, 90, 4), // "txid"
QT_MOC_LITERAL(12, 95, 7), // "txCount"
QT_MOC_LITERAL(13, 103, 9), // "Status_Ok"
QT_MOC_LITERAL(14, 113, 12), // "Status_Error"
QT_MOC_LITERAL(15, 126, 15), // "Status_Critical"
QT_MOC_LITERAL(16, 142, 8), // "Priority"
QT_MOC_LITERAL(17, 151, 12), // "Priority_Low"
QT_MOC_LITERAL(18, 164, 15), // "Priority_Medium"
QT_MOC_LITERAL(19, 180, 13) // "Priority_High"

    },
    "PendingTransaction\0commit\0\0setFilename\0"
    "fileName\0status\0Status\0errorString\0"
    "amount\0dust\0fee\0txid\0txCount\0Status_Ok\0"
    "Status_Error\0Status_Critical\0Priority\0"
    "Priority_Low\0Priority_Medium\0Priority_High"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PendingTransaction[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       7,   28, // properties
       2,   49, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x02 /* Public */,
       3,    1,   25,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Bool,
    QMetaType::Void, QMetaType::QString,    4,

 // properties: name, type, flags
       5, 0x80000000 | 6, 0x00095009,
       7, QMetaType::QString, 0x00095001,
       8, QMetaType::ULongLong, 0x00095001,
       9, QMetaType::ULongLong, 0x00095001,
      10, QMetaType::ULongLong, 0x00095001,
      11, QMetaType::QStringList, 0x00095001,
      12, QMetaType::ULongLong, 0x00095001,

 // enums: name, flags, count, data
       6, 0x0,    3,   57,
      16, 0x0,    3,   63,

 // enum data: key, value
      13, uint(PendingTransaction::Status_Ok),
      14, uint(PendingTransaction::Status_Error),
      15, uint(PendingTransaction::Status_Critical),
      17, uint(PendingTransaction::Priority_Low),
      18, uint(PendingTransaction::Priority_Medium),
      19, uint(PendingTransaction::Priority_High),

       0        // eod
};

void PendingTransaction::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PendingTransaction *_t = static_cast<PendingTransaction *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->commit();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: _t->setFilename((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        PendingTransaction *_t = static_cast<PendingTransaction *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Status*>(_v) = _t->status(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->errorString(); break;
        case 2: *reinterpret_cast< quint64*>(_v) = _t->amount(); break;
        case 3: *reinterpret_cast< quint64*>(_v) = _t->dust(); break;
        case 4: *reinterpret_cast< quint64*>(_v) = _t->fee(); break;
        case 5: *reinterpret_cast< QStringList*>(_v) = _t->txid(); break;
        case 6: *reinterpret_cast< quint64*>(_v) = _t->txCount(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject PendingTransaction::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PendingTransaction.data,
      qt_meta_data_PendingTransaction,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PendingTransaction::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PendingTransaction::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PendingTransaction.stringdata0))
        return static_cast<void*>(const_cast< PendingTransaction*>(this));
    return QObject::qt_metacast(_clname);
}

int PendingTransaction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
