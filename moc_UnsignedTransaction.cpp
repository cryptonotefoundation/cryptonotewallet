/****************************************************************************
** Meta object code from reading C++ file 'UnsignedTransaction.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/libwalletqt/UnsignedTransaction.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UnsignedTransaction.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UnsignedTransaction_t {
    QByteArrayData data[24];
    char stringdata0[256];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UnsignedTransaction_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UnsignedTransaction_t qt_meta_stringdata_UnsignedTransaction = {
    {
QT_MOC_LITERAL(0, 0, 19), // "UnsignedTransaction"
QT_MOC_LITERAL(1, 20, 6), // "amount"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "index"
QT_MOC_LITERAL(4, 34, 3), // "fee"
QT_MOC_LITERAL(5, 38, 5), // "mixin"
QT_MOC_LITERAL(6, 44, 4), // "sign"
QT_MOC_LITERAL(7, 49, 8), // "fileName"
QT_MOC_LITERAL(8, 58, 11), // "setFilename"
QT_MOC_LITERAL(9, 70, 6), // "status"
QT_MOC_LITERAL(10, 77, 6), // "Status"
QT_MOC_LITERAL(11, 84, 11), // "errorString"
QT_MOC_LITERAL(12, 96, 7), // "txCount"
QT_MOC_LITERAL(13, 104, 19), // "confirmationMessage"
QT_MOC_LITERAL(14, 124, 16), // "recipientAddress"
QT_MOC_LITERAL(15, 141, 9), // "paymentId"
QT_MOC_LITERAL(16, 151, 13), // "minMixinCount"
QT_MOC_LITERAL(17, 165, 9), // "Status_Ok"
QT_MOC_LITERAL(18, 175, 12), // "Status_Error"
QT_MOC_LITERAL(19, 188, 15), // "Status_Critical"
QT_MOC_LITERAL(20, 204, 8), // "Priority"
QT_MOC_LITERAL(21, 213, 12), // "Priority_Low"
QT_MOC_LITERAL(22, 226, 15), // "Priority_Medium"
QT_MOC_LITERAL(23, 242, 13) // "Priority_High"

    },
    "UnsignedTransaction\0amount\0\0index\0fee\0"
    "mixin\0sign\0fileName\0setFilename\0status\0"
    "Status\0errorString\0txCount\0"
    "confirmationMessage\0recipientAddress\0"
    "paymentId\0minMixinCount\0Status_Ok\0"
    "Status_Error\0Status_Critical\0Priority\0"
    "Priority_Low\0Priority_Medium\0Priority_High"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UnsignedTransaction[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       7,   54, // properties
       2,   75, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x02 /* Public */,
       4,    1,   42,    2, 0x02 /* Public */,
       5,    1,   45,    2, 0x02 /* Public */,
       6,    1,   48,    2, 0x02 /* Public */,
       8,    1,   51,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::ULongLong, QMetaType::Int,    3,
    QMetaType::ULongLong, QMetaType::Int,    3,
    QMetaType::ULongLong, QMetaType::Int,    3,
    QMetaType::Bool, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,

 // properties: name, type, flags
       9, 0x80000000 | 10, 0x00095009,
      11, QMetaType::QString, 0x00095001,
      12, QMetaType::ULongLong, 0x00095001,
      13, QMetaType::QString, 0x00095001,
      14, QMetaType::QStringList, 0x00095001,
      15, QMetaType::QStringList, 0x00095001,
      16, QMetaType::ULongLong, 0x00095001,

 // enums: name, flags, count, data
      10, 0x0,    3,   83,
      20, 0x0,    3,   89,

 // enum data: key, value
      17, uint(UnsignedTransaction::Status_Ok),
      18, uint(UnsignedTransaction::Status_Error),
      19, uint(UnsignedTransaction::Status_Critical),
      21, uint(UnsignedTransaction::Priority_Low),
      22, uint(UnsignedTransaction::Priority_Medium),
      23, uint(UnsignedTransaction::Priority_High),

       0        // eod
};

void UnsignedTransaction::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UnsignedTransaction *_t = static_cast<UnsignedTransaction *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { quint64 _r = _t->amount((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 1: { quint64 _r = _t->fee((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 2: { quint64 _r = _t->mixin((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->sign((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->setFilename((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        UnsignedTransaction *_t = static_cast<UnsignedTransaction *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Status*>(_v) = _t->status(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->errorString(); break;
        case 2: *reinterpret_cast< quint64*>(_v) = _t->txCount(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->confirmationMessage(); break;
        case 4: *reinterpret_cast< QStringList*>(_v) = _t->recipientAddress(); break;
        case 5: *reinterpret_cast< QStringList*>(_v) = _t->paymentId(); break;
        case 6: *reinterpret_cast< quint64*>(_v) = _t->minMixinCount(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject UnsignedTransaction::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UnsignedTransaction.data,
      qt_meta_data_UnsignedTransaction,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UnsignedTransaction::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UnsignedTransaction::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UnsignedTransaction.stringdata0))
        return static_cast<void*>(const_cast< UnsignedTransaction*>(this));
    return QObject::qt_metacast(_clname);
}

int UnsignedTransaction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
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
