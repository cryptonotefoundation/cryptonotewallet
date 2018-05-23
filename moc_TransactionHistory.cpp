/****************************************************************************
** Meta object code from reading C++ file 'TransactionHistory.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/libwalletqt/TransactionHistory.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TransactionHistory.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TransactionHistory_t {
    QByteArrayData data[17];
    char stringdata0[222];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TransactionHistory_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TransactionHistory_t qt_meta_stringdata_TransactionHistory = {
    {
QT_MOC_LITERAL(0, 0, 18), // "TransactionHistory"
QT_MOC_LITERAL(1, 19, 14), // "refreshStarted"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 15), // "refreshFinished"
QT_MOC_LITERAL(4, 51, 20), // "firstDateTimeChanged"
QT_MOC_LITERAL(5, 72, 19), // "lastDateTimeChanged"
QT_MOC_LITERAL(6, 92, 11), // "transaction"
QT_MOC_LITERAL(7, 104, 16), // "TransactionInfo*"
QT_MOC_LITERAL(8, 121, 5), // "index"
QT_MOC_LITERAL(9, 127, 6), // "getAll"
QT_MOC_LITERAL(10, 134, 23), // "QList<TransactionInfo*>"
QT_MOC_LITERAL(11, 158, 7), // "refresh"
QT_MOC_LITERAL(12, 166, 5), // "count"
QT_MOC_LITERAL(13, 172, 13), // "firstDateTime"
QT_MOC_LITERAL(14, 186, 12), // "lastDateTime"
QT_MOC_LITERAL(15, 199, 15), // "minutesToUnlock"
QT_MOC_LITERAL(16, 215, 6) // "locked"

    },
    "TransactionHistory\0refreshStarted\0\0"
    "refreshFinished\0firstDateTimeChanged\0"
    "lastDateTimeChanged\0transaction\0"
    "TransactionInfo*\0index\0getAll\0"
    "QList<TransactionInfo*>\0refresh\0count\0"
    "firstDateTime\0lastDateTime\0minutesToUnlock\0"
    "locked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TransactionHistory[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       5,   58, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,
       4,    0,   51,    2, 0x06 /* Public */,
       5,    0,   52,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       6,    1,   53,    2, 0x02 /* Public */,
       9,    0,   56,    2, 0x02 /* Public */,
      11,    0,   57,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    0x80000000 | 7, QMetaType::Int,    8,
    0x80000000 | 10,
    QMetaType::Void,

 // properties: name, type, flags
      12, QMetaType::Int, 0x00095001,
      13, QMetaType::QDateTime, 0x00495001,
      14, QMetaType::QDateTime, 0x00495001,
      15, QMetaType::Int, 0x00095001,
      16, QMetaType::Bool, 0x00095001,

 // properties: notify_signal_id
       0,
       2,
       3,
       0,
       0,

       0        // eod
};

void TransactionHistory::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TransactionHistory *_t = static_cast<TransactionHistory *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refreshStarted(); break;
        case 1: _t->refreshFinished(); break;
        case 2: _t->firstDateTimeChanged(); break;
        case 3: _t->lastDateTimeChanged(); break;
        case 4: { TransactionInfo* _r = _t->transaction((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< TransactionInfo**>(_a[0]) = _r; }  break;
        case 5: { QList<TransactionInfo*> _r = _t->getAll();
            if (_a[0]) *reinterpret_cast< QList<TransactionInfo*>*>(_a[0]) = _r; }  break;
        case 6: _t->refresh(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TransactionHistory::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistory::refreshStarted)) {
                *result = 0;
            }
        }
        {
            typedef void (TransactionHistory::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistory::refreshFinished)) {
                *result = 1;
            }
        }
        {
            typedef void (TransactionHistory::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistory::firstDateTimeChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (TransactionHistory::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistory::lastDateTimeChanged)) {
                *result = 3;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TransactionHistory *_t = static_cast<TransactionHistory *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->count(); break;
        case 1: *reinterpret_cast< QDateTime*>(_v) = _t->firstDateTime(); break;
        case 2: *reinterpret_cast< QDateTime*>(_v) = _t->lastDateTime(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->minutesToUnlock(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->locked(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject TransactionHistory::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TransactionHistory.data,
      qt_meta_data_TransactionHistory,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TransactionHistory::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TransactionHistory::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TransactionHistory.stringdata0))
        return static_cast<void*>(const_cast< TransactionHistory*>(this));
    return QObject::qt_metacast(_clname);
}

int TransactionHistory::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TransactionHistory::refreshStarted()const
{
    QMetaObject::activate(const_cast< TransactionHistory *>(this), &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void TransactionHistory::refreshFinished()const
{
    QMetaObject::activate(const_cast< TransactionHistory *>(this), &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void TransactionHistory::firstDateTimeChanged()const
{
    QMetaObject::activate(const_cast< TransactionHistory *>(this), &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void TransactionHistory::lastDateTimeChanged()const
{
    QMetaObject::activate(const_cast< TransactionHistory *>(this), &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
