/****************************************************************************
** Meta object code from reading C++ file 'filter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "filter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_filter_t {
    QByteArrayData data[10];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_filter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_filter_t qt_meta_stringdata_filter = {
    {
QT_MOC_LITERAL(0, 0, 6), // "filter"
QT_MOC_LITERAL(1, 7, 15), // "sequencePressed"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 1), // "o"
QT_MOC_LITERAL(4, 26, 3), // "seq"
QT_MOC_LITERAL(5, 30, 16), // "sequenceReleased"
QT_MOC_LITERAL(6, 47, 12), // "mousePressed"
QT_MOC_LITERAL(7, 60, 1), // "x"
QT_MOC_LITERAL(8, 62, 1), // "y"
QT_MOC_LITERAL(9, 64, 13) // "mouseReleased"

    },
    "filter\0sequencePressed\0\0o\0seq\0"
    "sequenceReleased\0mousePressed\0x\0y\0"
    "mouseReleased"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_filter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       5,    2,   39,    2, 0x06 /* Public */,
       6,    3,   44,    2, 0x06 /* Public */,
       9,    3,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    3,    4,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    3,    4,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,    7,    8,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,    7,    8,

       0        // eod
};

void filter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        filter *_t = static_cast<filter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sequencePressed((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 1: _t->sequenceReleased((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 2: _t->mousePressed((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 3: _t->mouseReleased((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (filter::*_t)(const QVariant & , const QVariant & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&filter::sequencePressed)) {
                *result = 0;
            }
        }
        {
            typedef void (filter::*_t)(const QVariant & , const QVariant & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&filter::sequenceReleased)) {
                *result = 1;
            }
        }
        {
            typedef void (filter::*_t)(const QVariant & , const QVariant & , const QVariant & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&filter::mousePressed)) {
                *result = 2;
            }
        }
        {
            typedef void (filter::*_t)(const QVariant & , const QVariant & , const QVariant & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&filter::mouseReleased)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject filter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_filter.data,
      qt_meta_data_filter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *filter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *filter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_filter.stringdata0))
        return static_cast<void*>(const_cast< filter*>(this));
    return QObject::qt_metacast(_clname);
}

int filter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void filter::sequencePressed(const QVariant & _t1, const QVariant & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void filter::sequenceReleased(const QVariant & _t1, const QVariant & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void filter::mousePressed(const QVariant & _t1, const QVariant & _t2, const QVariant & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void filter::mouseReleased(const QVariant & _t1, const QVariant & _t2, const QVariant & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
