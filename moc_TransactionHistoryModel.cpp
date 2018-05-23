/****************************************************************************
** Meta object code from reading C++ file 'TransactionHistoryModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/model/TransactionHistoryModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TransactionHistoryModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TransactionHistoryModel_t {
    QByteArrayData data[24];
    char stringdata0[546];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TransactionHistoryModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TransactionHistoryModel_t qt_meta_stringdata_TransactionHistoryModel = {
    {
QT_MOC_LITERAL(0, 0, 23), // "TransactionHistoryModel"
QT_MOC_LITERAL(1, 24, 25), // "transactionHistoryChanged"
QT_MOC_LITERAL(2, 50, 0), // ""
QT_MOC_LITERAL(3, 51, 18), // "transactionHistory"
QT_MOC_LITERAL(4, 70, 19), // "TransactionHistory*"
QT_MOC_LITERAL(5, 90, 19), // "TransactionInfoRole"
QT_MOC_LITERAL(6, 110, 15), // "TransactionRole"
QT_MOC_LITERAL(7, 126, 24), // "TransactionDirectionRole"
QT_MOC_LITERAL(8, 151, 22), // "TransactionPendingRole"
QT_MOC_LITERAL(9, 174, 21), // "TransactionFailedRole"
QT_MOC_LITERAL(10, 196, 21), // "TransactionAmountRole"
QT_MOC_LITERAL(11, 218, 28), // "TransactionDisplayAmountRole"
QT_MOC_LITERAL(12, 247, 18), // "TransactionFeeRole"
QT_MOC_LITERAL(13, 266, 26), // "TransactionBlockHeightRole"
QT_MOC_LITERAL(14, 293, 28), // "TransactionConfirmationsRole"
QT_MOC_LITERAL(15, 322, 36), // "TransactionConfirmationsRequi..."
QT_MOC_LITERAL(16, 359, 19), // "TransactionHashRole"
QT_MOC_LITERAL(17, 379, 24), // "TransactionTimeStampRole"
QT_MOC_LITERAL(18, 404, 24), // "TransactionPaymentIdRole"
QT_MOC_LITERAL(19, 429, 20), // "TransactionIsOutRole"
QT_MOC_LITERAL(20, 450, 19), // "TransactionDateRole"
QT_MOC_LITERAL(21, 470, 19), // "TransactionTimeRole"
QT_MOC_LITERAL(22, 490, 27), // "TransactionAtomicAmountRole"
QT_MOC_LITERAL(23, 518, 27) // "TransactionDestinationsRole"

    },
    "TransactionHistoryModel\0"
    "transactionHistoryChanged\0\0"
    "transactionHistory\0TransactionHistory*\0"
    "TransactionInfoRole\0TransactionRole\0"
    "TransactionDirectionRole\0"
    "TransactionPendingRole\0TransactionFailedRole\0"
    "TransactionAmountRole\0"
    "TransactionDisplayAmountRole\0"
    "TransactionFeeRole\0TransactionBlockHeightRole\0"
    "TransactionConfirmationsRole\0"
    "TransactionConfirmationsRequiredRole\0"
    "TransactionHashRole\0TransactionTimeStampRole\0"
    "TransactionPaymentIdRole\0TransactionIsOutRole\0"
    "TransactionDateRole\0TransactionTimeRole\0"
    "TransactionAtomicAmountRole\0"
    "TransactionDestinationsRole"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TransactionHistoryModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       1,   20, // properties
       1,   24, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // properties: name, type, flags
       3, 0x80000000 | 4, 0x0049510b,

 // properties: notify_signal_id
       0,

 // enums: name, flags, count, data
       5, 0x0,   18,   28,

 // enum data: key, value
       6, uint(TransactionHistoryModel::TransactionRole),
       7, uint(TransactionHistoryModel::TransactionDirectionRole),
       8, uint(TransactionHistoryModel::TransactionPendingRole),
       9, uint(TransactionHistoryModel::TransactionFailedRole),
      10, uint(TransactionHistoryModel::TransactionAmountRole),
      11, uint(TransactionHistoryModel::TransactionDisplayAmountRole),
      12, uint(TransactionHistoryModel::TransactionFeeRole),
      13, uint(TransactionHistoryModel::TransactionBlockHeightRole),
      14, uint(TransactionHistoryModel::TransactionConfirmationsRole),
      15, uint(TransactionHistoryModel::TransactionConfirmationsRequiredRole),
      16, uint(TransactionHistoryModel::TransactionHashRole),
      17, uint(TransactionHistoryModel::TransactionTimeStampRole),
      18, uint(TransactionHistoryModel::TransactionPaymentIdRole),
      19, uint(TransactionHistoryModel::TransactionIsOutRole),
      20, uint(TransactionHistoryModel::TransactionDateRole),
      21, uint(TransactionHistoryModel::TransactionTimeRole),
      22, uint(TransactionHistoryModel::TransactionAtomicAmountRole),
      23, uint(TransactionHistoryModel::TransactionDestinationsRole),

       0        // eod
};

void TransactionHistoryModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TransactionHistoryModel *_t = static_cast<TransactionHistoryModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->transactionHistoryChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TransactionHistoryModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistoryModel::transactionHistoryChanged)) {
                *result = 0;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TransactionHistoryModel *_t = static_cast<TransactionHistoryModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< TransactionHistory**>(_v) = _t->transactionHistory(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        TransactionHistoryModel *_t = static_cast<TransactionHistoryModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTransactionHistory(*reinterpret_cast< TransactionHistory**>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject TransactionHistoryModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_TransactionHistoryModel.data,
      qt_meta_data_TransactionHistoryModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TransactionHistoryModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TransactionHistoryModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TransactionHistoryModel.stringdata0))
        return static_cast<void*>(const_cast< TransactionHistoryModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int TransactionHistoryModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TransactionHistoryModel::transactionHistoryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
