/****************************************************************************
** Meta object code from reading C++ file 'AddressBookModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/model/AddressBookModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AddressBookModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AddressBookModel_t {
    QByteArrayData data[14];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AddressBookModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AddressBookModel_t qt_meta_stringdata_AddressBookModel = {
    {
QT_MOC_LITERAL(0, 0, 16), // "AddressBookModel"
QT_MOC_LITERAL(1, 17, 10), // "startReset"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 8), // "endReset"
QT_MOC_LITERAL(4, 38, 9), // "deleteRow"
QT_MOC_LITERAL(5, 48, 3), // "row"
QT_MOC_LITERAL(6, 52, 15), // "lookupPaymentID"
QT_MOC_LITERAL(7, 68, 10), // "payment_id"
QT_MOC_LITERAL(8, 79, 18), // "AddressBookRowRole"
QT_MOC_LITERAL(9, 98, 15), // "AddressBookRole"
QT_MOC_LITERAL(10, 114, 22), // "AddressBookAddressRole"
QT_MOC_LITERAL(11, 137, 26), // "AddressBookDescriptionRole"
QT_MOC_LITERAL(12, 164, 24), // "AddressBookPaymentIdRole"
QT_MOC_LITERAL(13, 189, 20) // "AddressBookRowIdRole"

    },
    "AddressBookModel\0startReset\0\0endReset\0"
    "deleteRow\0row\0lookupPaymentID\0payment_id\0"
    "AddressBookRowRole\0AddressBookRole\0"
    "AddressBookAddressRole\0"
    "AddressBookDescriptionRole\0"
    "AddressBookPaymentIdRole\0AddressBookRowIdRole"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AddressBookModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       1,   42, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       4,    1,   36,    2, 0x02 /* Public */,
       6,    1,   39,    2, 0x02 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Bool, QMetaType::Int,    5,
    QMetaType::Int, QMetaType::QString,    7,

 // enums: name, flags, count, data
       8, 0x0,    5,   46,

 // enum data: key, value
       9, uint(AddressBookModel::AddressBookRole),
      10, uint(AddressBookModel::AddressBookAddressRole),
      11, uint(AddressBookModel::AddressBookDescriptionRole),
      12, uint(AddressBookModel::AddressBookPaymentIdRole),
      13, uint(AddressBookModel::AddressBookRowIdRole),

       0        // eod
};

void AddressBookModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AddressBookModel *_t = static_cast<AddressBookModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startReset(); break;
        case 1: _t->endReset(); break;
        case 2: { bool _r = _t->deleteRow((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->lookupPaymentID((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject AddressBookModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_AddressBookModel.data,
      qt_meta_data_AddressBookModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AddressBookModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AddressBookModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AddressBookModel.stringdata0))
        return static_cast<void*>(const_cast< AddressBookModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int AddressBookModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
