/****************************************************************************
** Meta object code from reading C++ file 'DaemonManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/daemon/DaemonManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DaemonManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DaemonManager_t {
    QByteArrayData data[22];
    char stringdata0[234];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DaemonManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DaemonManager_t qt_meta_stringdata_DaemonManager = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DaemonManager"
QT_MOC_LITERAL(1, 14, 13), // "daemonStarted"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 13), // "daemonStopped"
QT_MOC_LITERAL(4, 43, 18), // "daemonStartFailure"
QT_MOC_LITERAL(5, 62, 20), // "daemonConsoleUpdated"
QT_MOC_LITERAL(6, 83, 7), // "message"
QT_MOC_LITERAL(7, 91, 11), // "printOutput"
QT_MOC_LITERAL(8, 103, 10), // "printError"
QT_MOC_LITERAL(9, 114, 12), // "stateChanged"
QT_MOC_LITERAL(10, 127, 22), // "QProcess::ProcessState"
QT_MOC_LITERAL(11, 150, 5), // "state"
QT_MOC_LITERAL(12, 156, 5), // "start"
QT_MOC_LITERAL(13, 162, 5), // "flags"
QT_MOC_LITERAL(14, 168, 7), // "testnet"
QT_MOC_LITERAL(15, 176, 7), // "dataDir"
QT_MOC_LITERAL(16, 184, 4), // "stop"
QT_MOC_LITERAL(17, 189, 7), // "running"
QT_MOC_LITERAL(18, 197, 11), // "sendCommand"
QT_MOC_LITERAL(19, 209, 3), // "cmd"
QT_MOC_LITERAL(20, 213, 4), // "exit"
QT_MOC_LITERAL(21, 218, 15) // "validateDataDir"

    },
    "DaemonManager\0daemonStarted\0\0daemonStopped\0"
    "daemonStartFailure\0daemonConsoleUpdated\0"
    "message\0printOutput\0printError\0"
    "stateChanged\0QProcess::ProcessState\0"
    "state\0start\0flags\0testnet\0dataDir\0"
    "stop\0running\0sendCommand\0cmd\0exit\0"
    "validateDataDir"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DaemonManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    0,   85,    2, 0x06 /* Public */,
       4,    0,   86,    2, 0x06 /* Public */,
       5,    1,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   90,    2, 0x0a /* Public */,
       8,    0,   91,    2, 0x0a /* Public */,
       9,    1,   92,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      12,    3,   95,    2, 0x02 /* Public */,
      12,    2,  102,    2, 0x22 /* Public | MethodCloned */,
      16,    1,  107,    2, 0x02 /* Public */,
      17,    1,  110,    2, 0x02 /* Public */,
      18,    2,  113,    2, 0x02 /* Public */,
      20,    0,  118,    2, 0x02 /* Public */,
      21,    1,  119,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,

 // methods: parameters
    QMetaType::Bool, QMetaType::QString, QMetaType::Bool, QMetaType::QString,   13,   14,   15,
    QMetaType::Bool, QMetaType::QString, QMetaType::Bool,   13,   14,
    QMetaType::Bool, QMetaType::Bool,   14,
    QMetaType::Bool, QMetaType::Bool,   14,
    QMetaType::Bool, QMetaType::QString, QMetaType::Bool,   19,   14,
    QMetaType::Void,
    QMetaType::QVariantMap, QMetaType::QString,   15,

       0        // eod
};

void DaemonManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DaemonManager *_t = static_cast<DaemonManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->daemonStarted(); break;
        case 1: _t->daemonStopped(); break;
        case 2: _t->daemonStartFailure(); break;
        case 3: _t->daemonConsoleUpdated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->printOutput(); break;
        case 5: _t->printError(); break;
        case 6: _t->stateChanged((*reinterpret_cast< QProcess::ProcessState(*)>(_a[1]))); break;
        case 7: { bool _r = _t->start((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->start((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->stop((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = _t->running((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->sendCommand((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: _t->exit(); break;
        case 13: { QVariantMap _r = _t->validateDataDir((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DaemonManager::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DaemonManager::daemonStarted)) {
                *result = 0;
            }
        }
        {
            typedef void (DaemonManager::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DaemonManager::daemonStopped)) {
                *result = 1;
            }
        }
        {
            typedef void (DaemonManager::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DaemonManager::daemonStartFailure)) {
                *result = 2;
            }
        }
        {
            typedef void (DaemonManager::*_t)(QString ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DaemonManager::daemonConsoleUpdated)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject DaemonManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DaemonManager.data,
      qt_meta_data_DaemonManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DaemonManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DaemonManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DaemonManager.stringdata0))
        return static_cast<void*>(const_cast< DaemonManager*>(this));
    return QObject::qt_metacast(_clname);
}

int DaemonManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void DaemonManager::daemonStarted()const
{
    QMetaObject::activate(const_cast< DaemonManager *>(this), &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void DaemonManager::daemonStopped()const
{
    QMetaObject::activate(const_cast< DaemonManager *>(this), &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void DaemonManager::daemonStartFailure()const
{
    QMetaObject::activate(const_cast< DaemonManager *>(this), &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void DaemonManager::daemonConsoleUpdated(QString _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< DaemonManager *>(this), &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
