/****************************************************************************
** Meta object code from reading C++ file 'TransactionHistorySortFilterModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/model/TransactionHistorySortFilterModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TransactionHistorySortFilterModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TransactionHistorySortFilterModel_t {
    QByteArrayData data[22];
    char stringdata0[365];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TransactionHistorySortFilterModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TransactionHistorySortFilterModel_t qt_meta_stringdata_TransactionHistorySortFilterModel = {
    {
QT_MOC_LITERAL(0, 0, 33), // "TransactionHistorySortFilterM..."
QT_MOC_LITERAL(1, 34, 19), // "searchFilterChanged"
QT_MOC_LITERAL(2, 54, 0), // ""
QT_MOC_LITERAL(3, 55, 22), // "paymentIdFilterChanged"
QT_MOC_LITERAL(4, 78, 21), // "dateFromFilterChanged"
QT_MOC_LITERAL(5, 100, 19), // "dateToFilterChanged"
QT_MOC_LITERAL(6, 120, 23), // "amountFromFilterChanged"
QT_MOC_LITERAL(7, 144, 21), // "amountToFilterChanged"
QT_MOC_LITERAL(8, 166, 22), // "directionFilterChanged"
QT_MOC_LITERAL(9, 189, 4), // "sort"
QT_MOC_LITERAL(10, 194, 6), // "column"
QT_MOC_LITERAL(11, 201, 13), // "Qt::SortOrder"
QT_MOC_LITERAL(12, 215, 5), // "order"
QT_MOC_LITERAL(13, 221, 12), // "searchFilter"
QT_MOC_LITERAL(14, 234, 15), // "paymentIdFilter"
QT_MOC_LITERAL(15, 250, 14), // "dateFromFilter"
QT_MOC_LITERAL(16, 265, 12), // "dateToFilter"
QT_MOC_LITERAL(17, 278, 16), // "amountFromFilter"
QT_MOC_LITERAL(18, 295, 14), // "amountToFilter"
QT_MOC_LITERAL(19, 310, 15), // "directionFilter"
QT_MOC_LITERAL(20, 326, 18), // "transactionHistory"
QT_MOC_LITERAL(21, 345, 19) // "TransactionHistory*"

    },
    "TransactionHistorySortFilterModel\0"
    "searchFilterChanged\0\0paymentIdFilterChanged\0"
    "dateFromFilterChanged\0dateToFilterChanged\0"
    "amountFromFilterChanged\0amountToFilterChanged\0"
    "directionFilterChanged\0sort\0column\0"
    "Qt::SortOrder\0order\0searchFilter\0"
    "paymentIdFilter\0dateFromFilter\0"
    "dateToFilter\0amountFromFilter\0"
    "amountToFilter\0directionFilter\0"
    "transactionHistory\0TransactionHistory*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TransactionHistorySortFilterModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       8,   66, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x06 /* Public */,
       5,    0,   57,    2, 0x06 /* Public */,
       6,    0,   58,    2, 0x06 /* Public */,
       7,    0,   59,    2, 0x06 /* Public */,
       8,    0,   60,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       9,    2,   61,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 11,   10,   12,

 // properties: name, type, flags
      13, QMetaType::QString, 0x00495103,
      14, QMetaType::QString, 0x00495103,
      15, QMetaType::QDate, 0x00495103,
      16, QMetaType::QDate, 0x00495103,
      17, QMetaType::Double, 0x00495103,
      18, QMetaType::Double, 0x00495103,
      19, QMetaType::Int, 0x00495103,
      20, 0x80000000 | 21, 0x00095009,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,
       6,
       0,

       0        // eod
};

void TransactionHistorySortFilterModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TransactionHistorySortFilterModel *_t = static_cast<TransactionHistorySortFilterModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->searchFilterChanged(); break;
        case 1: _t->paymentIdFilterChanged(); break;
        case 2: _t->dateFromFilterChanged(); break;
        case 3: _t->dateToFilterChanged(); break;
        case 4: _t->amountFromFilterChanged(); break;
        case 5: _t->amountToFilterChanged(); break;
        case 6: _t->directionFilterChanged(); break;
        case 7: _t->sort((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TransactionHistorySortFilterModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistorySortFilterModel::searchFilterChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (TransactionHistorySortFilterModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistorySortFilterModel::paymentIdFilterChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (TransactionHistorySortFilterModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistorySortFilterModel::dateFromFilterChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (TransactionHistorySortFilterModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistorySortFilterModel::dateToFilterChanged)) {
                *result = 3;
            }
        }
        {
            typedef void (TransactionHistorySortFilterModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistorySortFilterModel::amountFromFilterChanged)) {
                *result = 4;
            }
        }
        {
            typedef void (TransactionHistorySortFilterModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistorySortFilterModel::amountToFilterChanged)) {
                *result = 5;
            }
        }
        {
            typedef void (TransactionHistorySortFilterModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TransactionHistorySortFilterModel::directionFilterChanged)) {
                *result = 6;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TransactionHistorySortFilterModel *_t = static_cast<TransactionHistorySortFilterModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->searchFilter(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->paymentIdFilter(); break;
        case 2: *reinterpret_cast< QDate*>(_v) = _t->dateFromFilter(); break;
        case 3: *reinterpret_cast< QDate*>(_v) = _t->dateToFilter(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->amountFromFilter(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->amountToFilter(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->directionFilter(); break;
        case 7: *reinterpret_cast< TransactionHistory**>(_v) = _t->transactionHistory(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        TransactionHistorySortFilterModel *_t = static_cast<TransactionHistorySortFilterModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSearchFilter(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setPaymentIdFilter(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setDateFromFilter(*reinterpret_cast< QDate*>(_v)); break;
        case 3: _t->setDateToFilter(*reinterpret_cast< QDate*>(_v)); break;
        case 4: _t->setAmountFromFilter(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setAmountToFilter(*reinterpret_cast< double*>(_v)); break;
        case 6: _t->setDirectionFilter(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject TransactionHistorySortFilterModel::staticMetaObject = {
    { &QSortFilterProxyModel::staticMetaObject, qt_meta_stringdata_TransactionHistorySortFilterModel.data,
      qt_meta_data_TransactionHistorySortFilterModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TransactionHistorySortFilterModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TransactionHistorySortFilterModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TransactionHistorySortFilterModel.stringdata0))
        return static_cast<void*>(const_cast< TransactionHistorySortFilterModel*>(this));
    return QSortFilterProxyModel::qt_metacast(_clname);
}

int TransactionHistorySortFilterModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSortFilterProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TransactionHistorySortFilterModel::searchFilterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void TransactionHistorySortFilterModel::paymentIdFilterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void TransactionHistorySortFilterModel::dateFromFilterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void TransactionHistorySortFilterModel::dateToFilterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void TransactionHistorySortFilterModel::amountFromFilterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void TransactionHistorySortFilterModel::amountToFilterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void TransactionHistorySortFilterModel::directionFilterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
