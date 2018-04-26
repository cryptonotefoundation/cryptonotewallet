/****************************************************************************
** Meta object code from reading C++ file 'AddressBook.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/libwalletqt/AddressBook.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AddressBook.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AddressBook_t {
    QByteArrayData data[24];
    char stringdata0[287];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AddressBook_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AddressBook_t qt_meta_stringdata_AddressBook = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AddressBook"
QT_MOC_LITERAL(1, 12, 14), // "refreshStarted"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 15), // "refreshFinished"
QT_MOC_LITERAL(4, 44, 6), // "getAll"
QT_MOC_LITERAL(5, 51, 30), // "QList<Monero::AddressBookRow*>"
QT_MOC_LITERAL(6, 82, 6), // "update"
QT_MOC_LITERAL(7, 89, 6), // "getRow"
QT_MOC_LITERAL(8, 96, 23), // "Monero::AddressBookRow*"
QT_MOC_LITERAL(9, 120, 5), // "index"
QT_MOC_LITERAL(10, 126, 6), // "addRow"
QT_MOC_LITERAL(11, 133, 7), // "address"
QT_MOC_LITERAL(12, 141, 10), // "payment_id"
QT_MOC_LITERAL(13, 152, 11), // "description"
QT_MOC_LITERAL(14, 164, 9), // "deleteRow"
QT_MOC_LITERAL(15, 174, 5), // "rowId"
QT_MOC_LITERAL(16, 180, 11), // "errorString"
QT_MOC_LITERAL(17, 192, 9), // "errorCode"
QT_MOC_LITERAL(18, 202, 15), // "lookupPaymentID"
QT_MOC_LITERAL(19, 218, 9), // "ErrorCode"
QT_MOC_LITERAL(20, 228, 9), // "Status_Ok"
QT_MOC_LITERAL(21, 238, 13), // "General_Error"
QT_MOC_LITERAL(22, 252, 15), // "Invalid_Address"
QT_MOC_LITERAL(23, 268, 18) // "Invalid_Payment_Id"

    },
    "AddressBook\0refreshStarted\0\0refreshFinished\0"
    "getAll\0QList<Monero::AddressBookRow*>\0"
    "update\0getRow\0Monero::AddressBookRow*\0"
    "index\0addRow\0address\0payment_id\0"
    "description\0deleteRow\0rowId\0errorString\0"
    "errorCode\0lookupPaymentID\0ErrorCode\0"
    "Status_Ok\0General_Error\0Invalid_Address\0"
    "Invalid_Payment_Id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AddressBook[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       1,   88, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       4,    1,   66,    2, 0x02 /* Public */,
       4,    0,   69,    2, 0x22 /* Public | MethodCloned */,
       7,    1,   70,    2, 0x02 /* Public */,
      10,    3,   73,    2, 0x02 /* Public */,
      14,    1,   80,    2, 0x02 /* Public */,
      16,    0,   83,    2, 0x02 /* Public */,
      17,    0,   84,    2, 0x02 /* Public */,
      18,    1,   85,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    0x80000000 | 5, QMetaType::Bool,    6,
    0x80000000 | 5,
    0x80000000 | 8, QMetaType::Int,    9,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,   11,   12,   13,
    QMetaType::Bool, QMetaType::Int,   15,
    QMetaType::QString,
    QMetaType::Int,
    QMetaType::Int, QMetaType::QString,   12,

 // enums: name, flags, count, data
      19, 0x0,    4,   92,

 // enum data: key, value
      20, uint(AddressBook::Status_Ok),
      21, uint(AddressBook::General_Error),
      22, uint(AddressBook::Invalid_Address),
      23, uint(AddressBook::Invalid_Payment_Id),

       0        // eod
};

void AddressBook::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AddressBook *_t = static_cast<AddressBook *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refreshStarted(); break;
        case 1: _t->refreshFinished(); break;
        case 2: { QList<Monero::AddressBookRow*> _r = _t->getAll((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QList<Monero::AddressBookRow*>*>(_a[0]) = _r; }  break;
        case 3: { QList<Monero::AddressBookRow*> _r = _t->getAll();
            if (_a[0]) *reinterpret_cast< QList<Monero::AddressBookRow*>*>(_a[0]) = _r; }  break;
        case 4: { Monero::AddressBookRow* _r = _t->getRow((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< Monero::AddressBookRow**>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->addRow((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->deleteRow((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->errorString();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->errorCode();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { int _r = _t->lookupPaymentID((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AddressBook::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AddressBook::refreshStarted)) {
                *result = 0;
            }
        }
        {
            typedef void (AddressBook::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AddressBook::refreshFinished)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject AddressBook::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AddressBook.data,
      qt_meta_data_AddressBook,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AddressBook::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AddressBook::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AddressBook.stringdata0))
        return static_cast<void*>(const_cast< AddressBook*>(this));
    return QObject::qt_metacast(_clname);
}

int AddressBook::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AddressBook::refreshStarted()const
{
    QMetaObject::activate(const_cast< AddressBook *>(this), &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AddressBook::refreshFinished()const
{
    QMetaObject::activate(const_cast< AddressBook *>(this), &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
