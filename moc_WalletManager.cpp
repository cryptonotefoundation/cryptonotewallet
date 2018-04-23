/****************************************************************************
** Meta object code from reading C++ file 'WalletManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/libwalletqt/WalletManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WalletManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WalletManager_t {
    QByteArrayData data[93];
    char stringdata0[1085];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WalletManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WalletManager_t qt_meta_stringdata_WalletManager = {
    {
QT_MOC_LITERAL(0, 0, 13), // "WalletManager"
QT_MOC_LITERAL(1, 14, 12), // "walletOpened"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 7), // "Wallet*"
QT_MOC_LITERAL(4, 36, 6), // "wallet"
QT_MOC_LITERAL(5, 43, 12), // "walletClosed"
QT_MOC_LITERAL(6, 56, 13), // "walletAddress"
QT_MOC_LITERAL(7, 70, 20), // "checkUpdatesComplete"
QT_MOC_LITERAL(8, 91, 6), // "result"
QT_MOC_LITERAL(9, 98, 12), // "createWallet"
QT_MOC_LITERAL(10, 111, 4), // "path"
QT_MOC_LITERAL(11, 116, 8), // "password"
QT_MOC_LITERAL(12, 125, 8), // "language"
QT_MOC_LITERAL(13, 134, 7), // "testnet"
QT_MOC_LITERAL(14, 142, 10), // "openWallet"
QT_MOC_LITERAL(15, 153, 15), // "openWalletAsync"
QT_MOC_LITERAL(16, 169, 14), // "recoveryWallet"
QT_MOC_LITERAL(17, 184, 4), // "memo"
QT_MOC_LITERAL(18, 189, 13), // "restoreHeight"
QT_MOC_LITERAL(19, 203, 20), // "createWalletFromKeys"
QT_MOC_LITERAL(20, 224, 7), // "address"
QT_MOC_LITERAL(21, 232, 7), // "viewkey"
QT_MOC_LITERAL(22, 240, 8), // "spendkey"
QT_MOC_LITERAL(23, 249, 11), // "closeWallet"
QT_MOC_LITERAL(24, 261, 16), // "closeWalletAsync"
QT_MOC_LITERAL(25, 278, 12), // "walletExists"
QT_MOC_LITERAL(26, 291, 11), // "findWallets"
QT_MOC_LITERAL(27, 303, 11), // "errorString"
QT_MOC_LITERAL(28, 315, 10), // "moveWallet"
QT_MOC_LITERAL(29, 326, 3), // "src"
QT_MOC_LITERAL(30, 330, 3), // "dst"
QT_MOC_LITERAL(31, 334, 14), // "walletLanguage"
QT_MOC_LITERAL(32, 349, 6), // "locale"
QT_MOC_LITERAL(33, 356, 13), // "displayAmount"
QT_MOC_LITERAL(34, 370, 6), // "amount"
QT_MOC_LITERAL(35, 377, 16), // "amountFromString"
QT_MOC_LITERAL(36, 394, 16), // "amountFromDouble"
QT_MOC_LITERAL(37, 411, 20), // "maximumAllowedAmount"
QT_MOC_LITERAL(38, 432, 27), // "maximumAllowedAmountAsSting"
QT_MOC_LITERAL(39, 460, 14), // "paymentIdValid"
QT_MOC_LITERAL(40, 475, 10), // "payment_id"
QT_MOC_LITERAL(41, 486, 12), // "addressValid"
QT_MOC_LITERAL(42, 499, 8), // "keyValid"
QT_MOC_LITERAL(43, 508, 3), // "key"
QT_MOC_LITERAL(44, 512, 9), // "isViewKey"
QT_MOC_LITERAL(45, 522, 20), // "paymentIdFromAddress"
QT_MOC_LITERAL(46, 543, 12), // "checkPayment"
QT_MOC_LITERAL(47, 556, 4), // "txid"
QT_MOC_LITERAL(48, 561, 5), // "txkey"
QT_MOC_LITERAL(49, 567, 14), // "daemon_address"
QT_MOC_LITERAL(50, 582, 16), // "setDaemonAddress"
QT_MOC_LITERAL(51, 599, 9), // "connected"
QT_MOC_LITERAL(52, 609, 17), // "networkDifficulty"
QT_MOC_LITERAL(53, 627, 16), // "blockchainHeight"
QT_MOC_LITERAL(54, 644, 22), // "blockchainTargetHeight"
QT_MOC_LITERAL(55, 667, 14), // "miningHashRate"
QT_MOC_LITERAL(56, 682, 8), // "isMining"
QT_MOC_LITERAL(57, 691, 11), // "startMining"
QT_MOC_LITERAL(58, 703, 7), // "threads"
QT_MOC_LITERAL(59, 711, 16), // "backgroundMining"
QT_MOC_LITERAL(60, 728, 13), // "ignoreBattery"
QT_MOC_LITERAL(61, 742, 10), // "stopMining"
QT_MOC_LITERAL(62, 753, 14), // "urlToLocalPath"
QT_MOC_LITERAL(63, 768, 3), // "url"
QT_MOC_LITERAL(64, 772, 14), // "localPathToUrl"
QT_MOC_LITERAL(65, 787, 11), // "setLogLevel"
QT_MOC_LITERAL(66, 799, 8), // "logLevel"
QT_MOC_LITERAL(67, 808, 16), // "setLogCategories"
QT_MOC_LITERAL(68, 825, 10), // "categories"
QT_MOC_LITERAL(69, 836, 3), // "add"
QT_MOC_LITERAL(70, 840, 1), // "x"
QT_MOC_LITERAL(71, 842, 1), // "y"
QT_MOC_LITERAL(72, 844, 3), // "sub"
QT_MOC_LITERAL(73, 848, 4), // "addi"
QT_MOC_LITERAL(74, 853, 4), // "subi"
QT_MOC_LITERAL(75, 858, 19), // "getPasswordStrength"
QT_MOC_LITERAL(76, 878, 16), // "resolveOpenAlias"
QT_MOC_LITERAL(77, 895, 9), // "parse_uri"
QT_MOC_LITERAL(78, 905, 3), // "uri"
QT_MOC_LITERAL(79, 909, 8), // "QString&"
QT_MOC_LITERAL(80, 918, 9), // "uint64_t&"
QT_MOC_LITERAL(81, 928, 14), // "tx_description"
QT_MOC_LITERAL(82, 943, 14), // "recipient_name"
QT_MOC_LITERAL(83, 958, 17), // "QVector<QString>&"
QT_MOC_LITERAL(84, 976, 18), // "unknown_parameters"
QT_MOC_LITERAL(85, 995, 5), // "error"
QT_MOC_LITERAL(86, 1001, 10), // "saveQrCode"
QT_MOC_LITERAL(87, 1012, 17), // "checkUpdatesAsync"
QT_MOC_LITERAL(88, 1030, 8), // "software"
QT_MOC_LITERAL(89, 1039, 6), // "subdir"
QT_MOC_LITERAL(90, 1046, 12), // "checkUpdates"
QT_MOC_LITERAL(91, 1059, 16), // "clearWalletCache"
QT_MOC_LITERAL(92, 1076, 8) // "fileName"

    },
    "WalletManager\0walletOpened\0\0Wallet*\0"
    "wallet\0walletClosed\0walletAddress\0"
    "checkUpdatesComplete\0result\0createWallet\0"
    "path\0password\0language\0testnet\0"
    "openWallet\0openWalletAsync\0recoveryWallet\0"
    "memo\0restoreHeight\0createWalletFromKeys\0"
    "address\0viewkey\0spendkey\0closeWallet\0"
    "closeWalletAsync\0walletExists\0findWallets\0"
    "errorString\0moveWallet\0src\0dst\0"
    "walletLanguage\0locale\0displayAmount\0"
    "amount\0amountFromString\0amountFromDouble\0"
    "maximumAllowedAmount\0maximumAllowedAmountAsSting\0"
    "paymentIdValid\0payment_id\0addressValid\0"
    "keyValid\0key\0isViewKey\0paymentIdFromAddress\0"
    "checkPayment\0txid\0txkey\0daemon_address\0"
    "setDaemonAddress\0connected\0networkDifficulty\0"
    "blockchainHeight\0blockchainTargetHeight\0"
    "miningHashRate\0isMining\0startMining\0"
    "threads\0backgroundMining\0ignoreBattery\0"
    "stopMining\0urlToLocalPath\0url\0"
    "localPathToUrl\0setLogLevel\0logLevel\0"
    "setLogCategories\0categories\0add\0x\0y\0"
    "sub\0addi\0subi\0getPasswordStrength\0"
    "resolveOpenAlias\0parse_uri\0uri\0QString&\0"
    "uint64_t&\0tx_description\0recipient_name\0"
    "QVector<QString>&\0unknown_parameters\0"
    "error\0saveQrCode\0checkUpdatesAsync\0"
    "software\0subdir\0checkUpdates\0"
    "clearWalletCache\0fileName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WalletManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      56,   14, // methods
       1,  554, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  294,    2, 0x06 /* Public */,
       5,    1,  297,    2, 0x06 /* Public */,
       7,    1,  300,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       9,    4,  303,    2, 0x02 /* Public */,
       9,    3,  312,    2, 0x22 /* Public | MethodCloned */,
      14,    3,  319,    2, 0x02 /* Public */,
      14,    2,  326,    2, 0x22 /* Public | MethodCloned */,
      15,    3,  331,    2, 0x02 /* Public */,
      15,    2,  338,    2, 0x22 /* Public | MethodCloned */,
      16,    4,  343,    2, 0x02 /* Public */,
      16,    3,  352,    2, 0x22 /* Public | MethodCloned */,
      16,    2,  359,    2, 0x22 /* Public | MethodCloned */,
      19,    7,  364,    2, 0x02 /* Public */,
      19,    6,  379,    2, 0x22 /* Public | MethodCloned */,
      19,    5,  392,    2, 0x22 /* Public | MethodCloned */,
      23,    0,  403,    2, 0x02 /* Public */,
      24,    0,  404,    2, 0x02 /* Public */,
      25,    1,  405,    2, 0x02 /* Public */,
      26,    1,  408,    2, 0x02 /* Public */,
      27,    0,  411,    2, 0x02 /* Public */,
      28,    2,  412,    2, 0x02 /* Public */,
      31,    1,  417,    2, 0x02 /* Public */,
      33,    1,  420,    2, 0x02 /* Public */,
      35,    1,  423,    2, 0x02 /* Public */,
      36,    1,  426,    2, 0x02 /* Public */,
      37,    0,  429,    2, 0x02 /* Public */,
      38,    0,  430,    2, 0x02 /* Public */,
      39,    1,  431,    2, 0x02 /* Public */,
      41,    2,  434,    2, 0x02 /* Public */,
      42,    4,  439,    2, 0x02 /* Public */,
      45,    2,  448,    2, 0x02 /* Public */,
      46,    4,  453,    2, 0x02 /* Public */,
      50,    1,  462,    2, 0x02 /* Public */,
      51,    0,  465,    2, 0x02 /* Public */,
      52,    0,  466,    2, 0x02 /* Public */,
      53,    0,  467,    2, 0x02 /* Public */,
      54,    0,  468,    2, 0x02 /* Public */,
      55,    0,  469,    2, 0x02 /* Public */,
      56,    0,  470,    2, 0x02 /* Public */,
      57,    4,  471,    2, 0x02 /* Public */,
      61,    0,  480,    2, 0x02 /* Public */,
      62,    1,  481,    2, 0x02 /* Public */,
      64,    1,  484,    2, 0x02 /* Public */,
      65,    1,  487,    2, 0x02 /* Public */,
      67,    1,  490,    2, 0x02 /* Public */,
      69,    2,  493,    2, 0x02 /* Public */,
      72,    2,  498,    2, 0x02 /* Public */,
      73,    2,  503,    2, 0x02 /* Public */,
      74,    2,  508,    2, 0x02 /* Public */,
      75,    1,  513,    2, 0x02 /* Public */,
      76,    1,  516,    2, 0x02 /* Public */,
      77,    8,  519,    2, 0x02 /* Public */,
      86,    2,  536,    2, 0x02 /* Public */,
      87,    2,  541,    2, 0x02 /* Public */,
      90,    2,  546,    2, 0x02 /* Public */,
      91,    1,  551,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,

 // methods: parameters
    0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   10,   11,   12,   13,
    0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,   11,   12,
    0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   10,   11,   13,
    0x80000000 | 3, QMetaType::QString, QMetaType::QString,   10,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   10,   11,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,   11,
    0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::ULongLong,   10,   17,   13,   18,
    0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   10,   17,   13,
    0x80000000 | 3, QMetaType::QString, QMetaType::QString,   10,   17,
    0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::ULongLong,   10,   12,   13,   20,   21,   22,   18,
    0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,   12,   13,   20,   21,   22,
    0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::QString, QMetaType::QString,   10,   12,   13,   20,   21,
    QMetaType::QString,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,   10,
    QMetaType::QStringList, QMetaType::QString,   10,
    QMetaType::QString,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   29,   30,
    QMetaType::QString, QMetaType::QString,   32,
    QMetaType::QString, QMetaType::ULongLong,   34,
    QMetaType::ULongLong, QMetaType::QString,   34,
    QMetaType::ULongLong, QMetaType::Double,   34,
    QMetaType::ULongLong,
    QMetaType::QString,
    QMetaType::Bool, QMetaType::QString,   40,
    QMetaType::Bool, QMetaType::QString, QMetaType::Bool,   20,   13,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Bool,   43,   20,   44,   13,
    QMetaType::QString, QMetaType::QString, QMetaType::Bool,   20,   13,
    QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   20,   47,   48,   49,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Bool,
    QMetaType::ULongLong,
    QMetaType::ULongLong,
    QMetaType::ULongLong,
    QMetaType::Double,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::QString, QMetaType::UInt, QMetaType::Bool, QMetaType::Bool,   20,   58,   59,   60,
    QMetaType::Bool,
    QMetaType::QString, QMetaType::QUrl,   63,
    QMetaType::QUrl, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::Int,   66,
    QMetaType::Void, QMetaType::QString,   68,
    QMetaType::ULongLong, QMetaType::ULongLong, QMetaType::ULongLong,   70,   71,
    QMetaType::ULongLong, QMetaType::ULongLong, QMetaType::ULongLong,   70,   71,
    QMetaType::LongLong, QMetaType::LongLong, QMetaType::LongLong,   70,   71,
    QMetaType::LongLong, QMetaType::LongLong, QMetaType::LongLong,   70,   71,
    QMetaType::Double, QMetaType::QString,   11,
    QMetaType::QString, QMetaType::QString,   20,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 79, 0x80000000 | 79, 0x80000000 | 80, 0x80000000 | 79, 0x80000000 | 79, 0x80000000 | 83, 0x80000000 | 79,   78,   20,   40,   34,   81,   82,   84,   85,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   88,   89,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,   88,   89,
    QMetaType::Bool, QMetaType::QString,   92,

 // properties: name, type, flags
      51, QMetaType::Bool, 0x00095001,

       0        // eod
};

void WalletManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WalletManager *_t = static_cast<WalletManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->walletOpened((*reinterpret_cast< Wallet*(*)>(_a[1]))); break;
        case 1: _t->walletClosed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->checkUpdatesComplete((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: { Wallet* _r = _t->createWallet((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< Wallet**>(_a[0]) = _r; }  break;
        case 4: { Wallet* _r = _t->createWallet((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< Wallet**>(_a[0]) = _r; }  break;
        case 5: { Wallet* _r = _t->openWallet((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< Wallet**>(_a[0]) = _r; }  break;
        case 6: { Wallet* _r = _t->openWallet((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< Wallet**>(_a[0]) = _r; }  break;
        case 7: _t->openWalletAsync((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 8: _t->openWalletAsync((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: { Wallet* _r = _t->recoveryWallet((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< quint64(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< Wallet**>(_a[0]) = _r; }  break;
        case 10: { Wallet* _r = _t->recoveryWallet((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< Wallet**>(_a[0]) = _r; }  break;
        case 11: { Wallet* _r = _t->recoveryWallet((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< Wallet**>(_a[0]) = _r; }  break;
        case 12: { Wallet* _r = _t->createWalletFromKeys((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< quint64(*)>(_a[7])));
            if (_a[0]) *reinterpret_cast< Wallet**>(_a[0]) = _r; }  break;
        case 13: { Wallet* _r = _t->createWalletFromKeys((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])));
            if (_a[0]) *reinterpret_cast< Wallet**>(_a[0]) = _r; }  break;
        case 14: { Wallet* _r = _t->createWalletFromKeys((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< Wallet**>(_a[0]) = _r; }  break;
        case 15: { QString _r = _t->closeWallet();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 16: _t->closeWalletAsync(); break;
        case 17: { bool _r = _t->walletExists((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { QStringList _r = _t->findWallets((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 19: { QString _r = _t->errorString();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 20: { bool _r = _t->moveWallet((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 21: { QString _r = _t->walletLanguage((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 22: { QString _r = _t->displayAmount((*reinterpret_cast< quint64(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 23: { quint64 _r = _t->amountFromString((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 24: { quint64 _r = _t->amountFromDouble((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 25: { quint64 _r = _t->maximumAllowedAmount();
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 26: { QString _r = _t->maximumAllowedAmountAsSting();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 27: { bool _r = _t->paymentIdValid((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 28: { bool _r = _t->addressValid((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 29: { bool _r = _t->keyValid((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 30: { QString _r = _t->paymentIdFromAddress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 31: { QString _r = _t->checkPayment((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 32: _t->setDaemonAddress((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 33: { bool _r = _t->connected();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 34: { quint64 _r = _t->networkDifficulty();
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 35: { quint64 _r = _t->blockchainHeight();
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 36: { quint64 _r = _t->blockchainTargetHeight();
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 37: { double _r = _t->miningHashRate();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 38: { bool _r = _t->isMining();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 39: { bool _r = _t->startMining((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 40: { bool _r = _t->stopMining();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 41: { QString _r = _t->urlToLocalPath((*reinterpret_cast< const QUrl(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 42: { QUrl _r = _t->localPathToUrl((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QUrl*>(_a[0]) = _r; }  break;
        case 43: _t->setLogLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->setLogCategories((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 45: { quint64 _r = _t->add((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 46: { quint64 _r = _t->sub((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 47: { qint64 _r = _t->addi((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< qint64*>(_a[0]) = _r; }  break;
        case 48: { qint64 _r = _t->subi((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< qint64*>(_a[0]) = _r; }  break;
        case 49: { double _r = _t->getPasswordStrength((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 50: { QString _r = _t->resolveOpenAlias((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 51: { bool _r = _t->parse_uri((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< uint64_t(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QVector<QString>(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 52: { bool _r = _t->saveQrCode((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 53: _t->checkUpdatesAsync((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 54: { QString _r = _t->checkUpdates((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 55: { bool _r = _t->clearWalletCache((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (WalletManager::*_t)(Wallet * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WalletManager::walletOpened)) {
                *result = 0;
            }
        }
        {
            typedef void (WalletManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WalletManager::walletClosed)) {
                *result = 1;
            }
        }
        {
            typedef void (WalletManager::*_t)(const QString & ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WalletManager::checkUpdatesComplete)) {
                *result = 2;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        WalletManager *_t = static_cast<WalletManager *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->connected(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject WalletManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WalletManager.data,
      qt_meta_data_WalletManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WalletManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WalletManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WalletManager.stringdata0))
        return static_cast<void*>(const_cast< WalletManager*>(this));
    return QObject::qt_metacast(_clname);
}

int WalletManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 56)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 56;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 56)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 56;
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
void WalletManager::walletOpened(Wallet * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WalletManager::walletClosed(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WalletManager::checkUpdatesComplete(const QString & _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< WalletManager *>(this), &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
