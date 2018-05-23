/****************************************************************************
** Meta object code from reading C++ file 'Wallet.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/libwalletqt/Wallet.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Wallet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Wallet_t {
    QByteArrayData data[126];
    char stringdata0[1760];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Wallet_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Wallet_t qt_meta_stringdata_Wallet = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Wallet"
QT_MOC_LITERAL(1, 7, 7), // "updated"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 9), // "refreshed"
QT_MOC_LITERAL(4, 26, 10), // "moneySpent"
QT_MOC_LITERAL(5, 37, 4), // "txId"
QT_MOC_LITERAL(6, 42, 6), // "amount"
QT_MOC_LITERAL(7, 49, 13), // "moneyReceived"
QT_MOC_LITERAL(8, 63, 24), // "unconfirmedMoneyReceived"
QT_MOC_LITERAL(9, 88, 8), // "newBlock"
QT_MOC_LITERAL(10, 97, 6), // "height"
QT_MOC_LITERAL(11, 104, 12), // "targetHeight"
QT_MOC_LITERAL(12, 117, 19), // "historyModelChanged"
QT_MOC_LITERAL(13, 137, 27), // "walletCreationHeightChanged"
QT_MOC_LITERAL(14, 165, 18), // "transactionCreated"
QT_MOC_LITERAL(15, 184, 19), // "PendingTransaction*"
QT_MOC_LITERAL(16, 204, 11), // "transaction"
QT_MOC_LITERAL(17, 216, 7), // "address"
QT_MOC_LITERAL(18, 224, 9), // "paymentId"
QT_MOC_LITERAL(19, 234, 10), // "mixinCount"
QT_MOC_LITERAL(20, 245, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(21, 269, 16), // "ConnectionStatus"
QT_MOC_LITERAL(22, 286, 6), // "status"
QT_MOC_LITERAL(23, 293, 15), // "setSeedLanguage"
QT_MOC_LITERAL(24, 309, 4), // "lang"
QT_MOC_LITERAL(25, 314, 9), // "connected"
QT_MOC_LITERAL(26, 324, 10), // "forceCheck"
QT_MOC_LITERAL(27, 335, 11), // "setPassword"
QT_MOC_LITERAL(28, 347, 8), // "password"
QT_MOC_LITERAL(29, 356, 5), // "store"
QT_MOC_LITERAL(30, 362, 4), // "path"
QT_MOC_LITERAL(31, 367, 4), // "init"
QT_MOC_LITERAL(32, 372, 13), // "daemonAddress"
QT_MOC_LITERAL(33, 386, 21), // "upperTransactionLimit"
QT_MOC_LITERAL(34, 408, 12), // "isRecovering"
QT_MOC_LITERAL(35, 421, 13), // "restoreHeight"
QT_MOC_LITERAL(36, 435, 9), // "initAsync"
QT_MOC_LITERAL(37, 445, 14), // "setDaemonLogin"
QT_MOC_LITERAL(38, 460, 14), // "daemonUsername"
QT_MOC_LITERAL(39, 475, 14), // "daemonPassword"
QT_MOC_LITERAL(40, 490, 14), // "createViewOnly"
QT_MOC_LITERAL(41, 505, 15), // "connectToDaemon"
QT_MOC_LITERAL(42, 521, 16), // "setTrustedDaemon"
QT_MOC_LITERAL(43, 538, 3), // "arg"
QT_MOC_LITERAL(44, 542, 7), // "balance"
QT_MOC_LITERAL(45, 550, 15), // "unlockedBalance"
QT_MOC_LITERAL(46, 566, 8), // "viewOnly"
QT_MOC_LITERAL(47, 575, 16), // "blockChainHeight"
QT_MOC_LITERAL(48, 592, 22), // "daemonBlockChainHeight"
QT_MOC_LITERAL(49, 615, 28), // "daemonBlockChainTargetHeight"
QT_MOC_LITERAL(50, 644, 7), // "refresh"
QT_MOC_LITERAL(51, 652, 12), // "refreshAsync"
QT_MOC_LITERAL(52, 665, 22), // "setAutoRefreshInterval"
QT_MOC_LITERAL(53, 688, 7), // "seconds"
QT_MOC_LITERAL(54, 696, 19), // "autoRefreshInterval"
QT_MOC_LITERAL(55, 716, 12), // "startRefresh"
QT_MOC_LITERAL(56, 729, 12), // "pauseRefresh"
QT_MOC_LITERAL(57, 742, 17), // "createTransaction"
QT_MOC_LITERAL(58, 760, 8), // "dst_addr"
QT_MOC_LITERAL(59, 769, 10), // "payment_id"
QT_MOC_LITERAL(60, 780, 11), // "mixin_count"
QT_MOC_LITERAL(61, 792, 28), // "PendingTransaction::Priority"
QT_MOC_LITERAL(62, 821, 8), // "priority"
QT_MOC_LITERAL(63, 830, 22), // "createTransactionAsync"
QT_MOC_LITERAL(64, 853, 20), // "createTransactionAll"
QT_MOC_LITERAL(65, 874, 25), // "createTransactionAllAsync"
QT_MOC_LITERAL(66, 900, 31), // "createSweepUnmixableTransaction"
QT_MOC_LITERAL(67, 932, 36), // "createSweepUnmixableTransacti..."
QT_MOC_LITERAL(68, 969, 10), // "loadTxFile"
QT_MOC_LITERAL(69, 980, 20), // "UnsignedTransaction*"
QT_MOC_LITERAL(70, 1001, 8), // "fileName"
QT_MOC_LITERAL(71, 1010, 12), // "submitTxFile"
QT_MOC_LITERAL(72, 1023, 18), // "disposeTransaction"
QT_MOC_LITERAL(73, 1042, 1), // "t"
QT_MOC_LITERAL(74, 1044, 17), // "generatePaymentId"
QT_MOC_LITERAL(75, 1062, 17), // "integratedAddress"
QT_MOC_LITERAL(76, 1080, 11), // "signMessage"
QT_MOC_LITERAL(77, 1092, 7), // "message"
QT_MOC_LITERAL(78, 1100, 8), // "filename"
QT_MOC_LITERAL(79, 1109, 19), // "verifySignedMessage"
QT_MOC_LITERAL(80, 1129, 9), // "signature"
QT_MOC_LITERAL(81, 1139, 9), // "parse_uri"
QT_MOC_LITERAL(82, 1149, 3), // "uri"
QT_MOC_LITERAL(83, 1153, 8), // "QString&"
QT_MOC_LITERAL(84, 1162, 9), // "uint64_t&"
QT_MOC_LITERAL(85, 1172, 14), // "tx_description"
QT_MOC_LITERAL(86, 1187, 14), // "recipient_name"
QT_MOC_LITERAL(87, 1202, 17), // "QVector<QString>&"
QT_MOC_LITERAL(88, 1220, 18), // "unknown_parameters"
QT_MOC_LITERAL(89, 1239, 5), // "error"
QT_MOC_LITERAL(90, 1245, 11), // "setUserNote"
QT_MOC_LITERAL(91, 1257, 4), // "txid"
QT_MOC_LITERAL(92, 1262, 4), // "note"
QT_MOC_LITERAL(93, 1267, 11), // "getUserNote"
QT_MOC_LITERAL(94, 1279, 8), // "getTxKey"
QT_MOC_LITERAL(95, 1288, 11), // "rescanSpent"
QT_MOC_LITERAL(96, 1300, 12), // "useForkRules"
QT_MOC_LITERAL(97, 1313, 7), // "version"
QT_MOC_LITERAL(98, 1321, 11), // "earlyBlocks"
QT_MOC_LITERAL(99, 1333, 4), // "seed"
QT_MOC_LITERAL(100, 1338, 12), // "seedLanguage"
QT_MOC_LITERAL(101, 1351, 6), // "Status"
QT_MOC_LITERAL(102, 1358, 7), // "testnet"
QT_MOC_LITERAL(103, 1366, 12), // "synchronized"
QT_MOC_LITERAL(104, 1379, 11), // "errorString"
QT_MOC_LITERAL(105, 1391, 7), // "history"
QT_MOC_LITERAL(106, 1399, 19), // "TransactionHistory*"
QT_MOC_LITERAL(107, 1419, 12), // "historyModel"
QT_MOC_LITERAL(108, 1432, 34), // "TransactionHistorySortFilterM..."
QT_MOC_LITERAL(109, 1467, 16), // "addressBookModel"
QT_MOC_LITERAL(110, 1484, 17), // "AddressBookModel*"
QT_MOC_LITERAL(111, 1502, 11), // "addressBook"
QT_MOC_LITERAL(112, 1514, 12), // "AddressBook*"
QT_MOC_LITERAL(113, 1527, 13), // "secretViewKey"
QT_MOC_LITERAL(114, 1541, 13), // "publicViewKey"
QT_MOC_LITERAL(115, 1555, 14), // "secretSpendKey"
QT_MOC_LITERAL(116, 1570, 14), // "publicSpendKey"
QT_MOC_LITERAL(117, 1585, 13), // "daemonLogPath"
QT_MOC_LITERAL(118, 1599, 13), // "walletLogPath"
QT_MOC_LITERAL(119, 1613, 20), // "walletCreationHeight"
QT_MOC_LITERAL(120, 1634, 9), // "Status_Ok"
QT_MOC_LITERAL(121, 1644, 12), // "Status_Error"
QT_MOC_LITERAL(122, 1657, 15), // "Status_Critical"
QT_MOC_LITERAL(123, 1673, 26), // "ConnectionStatus_Connected"
QT_MOC_LITERAL(124, 1700, 29), // "ConnectionStatus_Disconnected"
QT_MOC_LITERAL(125, 1730, 29) // "ConnectionStatus_WrongVersion"

    },
    "Wallet\0updated\0\0refreshed\0moneySpent\0"
    "txId\0amount\0moneyReceived\0"
    "unconfirmedMoneyReceived\0newBlock\0"
    "height\0targetHeight\0historyModelChanged\0"
    "walletCreationHeightChanged\0"
    "transactionCreated\0PendingTransaction*\0"
    "transaction\0address\0paymentId\0mixinCount\0"
    "connectionStatusChanged\0ConnectionStatus\0"
    "status\0setSeedLanguage\0lang\0connected\0"
    "forceCheck\0setPassword\0password\0store\0"
    "path\0init\0daemonAddress\0upperTransactionLimit\0"
    "isRecovering\0restoreHeight\0initAsync\0"
    "setDaemonLogin\0daemonUsername\0"
    "daemonPassword\0createViewOnly\0"
    "connectToDaemon\0setTrustedDaemon\0arg\0"
    "balance\0unlockedBalance\0viewOnly\0"
    "blockChainHeight\0daemonBlockChainHeight\0"
    "daemonBlockChainTargetHeight\0refresh\0"
    "refreshAsync\0setAutoRefreshInterval\0"
    "seconds\0autoRefreshInterval\0startRefresh\0"
    "pauseRefresh\0createTransaction\0dst_addr\0"
    "payment_id\0mixin_count\0"
    "PendingTransaction::Priority\0priority\0"
    "createTransactionAsync\0createTransactionAll\0"
    "createTransactionAllAsync\0"
    "createSweepUnmixableTransaction\0"
    "createSweepUnmixableTransactionAsync\0"
    "loadTxFile\0UnsignedTransaction*\0"
    "fileName\0submitTxFile\0disposeTransaction\0"
    "t\0generatePaymentId\0integratedAddress\0"
    "signMessage\0message\0filename\0"
    "verifySignedMessage\0signature\0parse_uri\0"
    "uri\0QString&\0uint64_t&\0tx_description\0"
    "recipient_name\0QVector<QString>&\0"
    "unknown_parameters\0error\0setUserNote\0"
    "txid\0note\0getUserNote\0getTxKey\0"
    "rescanSpent\0useForkRules\0version\0"
    "earlyBlocks\0seed\0seedLanguage\0Status\0"
    "testnet\0synchronized\0errorString\0"
    "history\0TransactionHistory*\0historyModel\0"
    "TransactionHistorySortFilterModel*\0"
    "addressBookModel\0AddressBookModel*\0"
    "addressBook\0AddressBook*\0secretViewKey\0"
    "publicViewKey\0secretSpendKey\0"
    "publicSpendKey\0daemonLogPath\0walletLogPath\0"
    "walletCreationHeight\0Status_Ok\0"
    "Status_Error\0Status_Critical\0"
    "ConnectionStatus_Connected\0"
    "ConnectionStatus_Disconnected\0"
    "ConnectionStatus_WrongVersion"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Wallet[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      65,   14, // methods
      23,  588, // properties
       2,  680, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  339,    2, 0x06 /* Public */,
       3,    0,  340,    2, 0x06 /* Public */,
       4,    2,  341,    2, 0x06 /* Public */,
       7,    2,  346,    2, 0x06 /* Public */,
       8,    2,  351,    2, 0x06 /* Public */,
       9,    2,  356,    2, 0x06 /* Public */,
      12,    0,  361,    2, 0x06 /* Public */,
      13,    0,  362,    2, 0x06 /* Public */,
      14,    4,  363,    2, 0x06 /* Public */,
      20,    1,  372,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      23,    1,  375,    2, 0x02 /* Public */,
      25,    1,  378,    2, 0x02 /* Public */,
      25,    0,  381,    2, 0x22 /* Public | MethodCloned */,
      27,    1,  382,    2, 0x02 /* Public */,
      29,    1,  385,    2, 0x02 /* Public */,
      29,    0,  388,    2, 0x22 /* Public | MethodCloned */,
      31,    4,  389,    2, 0x02 /* Public */,
      31,    3,  398,    2, 0x22 /* Public | MethodCloned */,
      31,    2,  405,    2, 0x22 /* Public | MethodCloned */,
      31,    1,  410,    2, 0x22 /* Public | MethodCloned */,
      36,    4,  413,    2, 0x02 /* Public */,
      36,    3,  422,    2, 0x22 /* Public | MethodCloned */,
      36,    2,  429,    2, 0x22 /* Public | MethodCloned */,
      36,    1,  434,    2, 0x22 /* Public | MethodCloned */,
      37,    2,  437,    2, 0x02 /* Public */,
      37,    1,  442,    2, 0x22 /* Public | MethodCloned */,
      37,    0,  445,    2, 0x22 /* Public | MethodCloned */,
      40,    2,  446,    2, 0x02 /* Public */,
      41,    0,  451,    2, 0x02 /* Public */,
      42,    1,  452,    2, 0x02 /* Public */,
      44,    0,  455,    2, 0x02 /* Public */,
      45,    0,  456,    2, 0x02 /* Public */,
      46,    0,  457,    2, 0x02 /* Public */,
      47,    0,  458,    2, 0x02 /* Public */,
      48,    0,  459,    2, 0x02 /* Public */,
      49,    0,  460,    2, 0x02 /* Public */,
      50,    0,  461,    2, 0x02 /* Public */,
      51,    0,  462,    2, 0x02 /* Public */,
      52,    1,  463,    2, 0x02 /* Public */,
      54,    0,  466,    2, 0x02 /* Public */,
      55,    0,  467,    2, 0x02 /* Public */,
      56,    0,  468,    2, 0x02 /* Public */,
      57,    5,  469,    2, 0x02 /* Public */,
      63,    5,  480,    2, 0x02 /* Public */,
      64,    4,  491,    2, 0x02 /* Public */,
      65,    4,  500,    2, 0x02 /* Public */,
      66,    0,  509,    2, 0x02 /* Public */,
      67,    0,  510,    2, 0x02 /* Public */,
      68,    1,  511,    2, 0x02 /* Public */,
      71,    1,  514,    2, 0x02 /* Public */,
      72,    1,  517,    2, 0x02 /* Public */,
      72,    1,  520,    2, 0x02 /* Public */,
      74,    0,  523,    2, 0x02 /* Public */,
      75,    1,  524,    2, 0x02 /* Public */,
      76,    2,  527,    2, 0x02 /* Public */,
      76,    1,  532,    2, 0x22 /* Public | MethodCloned */,
      79,    4,  535,    2, 0x02 /* Public */,
      79,    3,  544,    2, 0x22 /* Public | MethodCloned */,
      81,    8,  551,    2, 0x02 /* Public */,
      90,    2,  568,    2, 0x02 /* Public */,
      93,    1,  573,    2, 0x02 /* Public */,
      94,    1,  576,    2, 0x02 /* Public */,
      95,    0,  579,    2, 0x02 /* Public */,
      96,    2,  580,    2, 0x02 /* Public */,
      96,    1,  585,    2, 0x22 /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong,    5,    6,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::ULongLong,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15, QMetaType::QString, QMetaType::QString, QMetaType::UInt,   16,   17,   18,   19,
    QMetaType::Void, 0x80000000 | 21,   22,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,   24,
    0x80000000 | 21, QMetaType::Bool,   26,
    0x80000000 | 21,
    QMetaType::Bool, QMetaType::QString,   28,
    QMetaType::Bool, QMetaType::QString,   30,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::QString, QMetaType::ULongLong, QMetaType::Bool, QMetaType::ULongLong,   32,   33,   34,   35,
    QMetaType::Bool, QMetaType::QString, QMetaType::ULongLong, QMetaType::Bool,   32,   33,   34,
    QMetaType::Bool, QMetaType::QString, QMetaType::ULongLong,   32,   33,
    QMetaType::Bool, QMetaType::QString,   32,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong, QMetaType::Bool, QMetaType::ULongLong,   32,   33,   34,   35,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong, QMetaType::Bool,   32,   33,   34,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong,   32,   33,
    QMetaType::Void, QMetaType::QString,   32,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   38,   39,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   30,   28,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Bool,   43,
    QMetaType::ULongLong,
    QMetaType::ULongLong,
    QMetaType::Bool,
    QMetaType::ULongLong,
    QMetaType::ULongLong,
    QMetaType::ULongLong,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   53,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 15, QMetaType::QString, QMetaType::QString, QMetaType::ULongLong, QMetaType::UInt, 0x80000000 | 61,   58,   59,    6,   60,   62,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::ULongLong, QMetaType::UInt, 0x80000000 | 61,   58,   59,    6,   60,   62,
    0x80000000 | 15, QMetaType::QString, QMetaType::QString, QMetaType::UInt, 0x80000000 | 61,   58,   59,   60,   62,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UInt, 0x80000000 | 61,   58,   59,   60,   62,
    0x80000000 | 15,
    QMetaType::Void,
    0x80000000 | 69, QMetaType::QString,   70,
    QMetaType::Bool, QMetaType::QString,   70,
    QMetaType::Void, 0x80000000 | 15,   73,
    QMetaType::Void, 0x80000000 | 69,   73,
    QMetaType::QString,
    QMetaType::QString, QMetaType::QString,   18,
    QMetaType::QString, QMetaType::QString, QMetaType::Bool,   77,   78,
    QMetaType::QString, QMetaType::QString,   77,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   77,   17,   80,   78,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,   77,   17,   80,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 83, 0x80000000 | 83, 0x80000000 | 84, 0x80000000 | 83, 0x80000000 | 83, 0x80000000 | 87, 0x80000000 | 83,   82,   17,   59,    6,   85,   86,   88,   89,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   91,   92,
    QMetaType::QString, QMetaType::QString,   91,
    QMetaType::QString, QMetaType::QString,   91,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::UChar, QMetaType::ULongLong,   97,   98,
    QMetaType::Bool, QMetaType::UChar,   97,

 // properties: name, type, flags
      99, QMetaType::QString, 0x00095001,
     100, QMetaType::QString, 0x00095001,
      22, 0x80000000 | 101, 0x00095009,
     102, QMetaType::Bool, 0x00095001,
     103, QMetaType::Bool, 0x00095001,
     104, QMetaType::QString, 0x00095001,
      17, QMetaType::QString, 0x00095001,
      44, QMetaType::ULongLong, 0x00095001,
      45, QMetaType::ULongLong, 0x00095001,
     105, 0x80000000 | 106, 0x00095009,
      18, QMetaType::QString, 0x00095103,
     107, 0x80000000 | 108, 0x00495009,
      30, QMetaType::QString, 0x00095001,
     109, 0x80000000 | 110, 0x00095009,
     111, 0x80000000 | 112, 0x00095009,
      46, QMetaType::Bool, 0x00095001,
     113, QMetaType::QString, 0x00095001,
     114, QMetaType::QString, 0x00095001,
     115, QMetaType::QString, 0x00095001,
     116, QMetaType::QString, 0x00095001,
     117, QMetaType::QString, 0x00095401,
     118, QMetaType::QString, 0x00095401,
     119, QMetaType::ULongLong, 0x00495103,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       6,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       7,

 // enums: name, flags, count, data
     101, 0x0,    3,  688,
      21, 0x0,    3,  694,

 // enum data: key, value
     120, uint(Wallet::Status_Ok),
     121, uint(Wallet::Status_Error),
     122, uint(Wallet::Status_Critical),
     123, uint(Wallet::ConnectionStatus_Connected),
     124, uint(Wallet::ConnectionStatus_Disconnected),
     125, uint(Wallet::ConnectionStatus_WrongVersion),

       0        // eod
};

void Wallet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Wallet *_t = static_cast<Wallet *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updated(); break;
        case 1: _t->refreshed(); break;
        case 2: _t->moneySpent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 3: _t->moneyReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 4: _t->unconfirmedMoneyReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 5: _t->newBlock((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 6: _t->historyModelChanged(); break;
        case 7: _t->walletCreationHeightChanged(); break;
        case 8: _t->transactionCreated((*reinterpret_cast< PendingTransaction*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< quint32(*)>(_a[4]))); break;
        case 9: _t->connectionStatusChanged((*reinterpret_cast< ConnectionStatus(*)>(_a[1]))); break;
        case 10: _t->setSeedLanguage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: { ConnectionStatus _r = _t->connected((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< ConnectionStatus*>(_a[0]) = _r; }  break;
        case 12: { ConnectionStatus _r = _t->connected();
            if (_a[0]) *reinterpret_cast< ConnectionStatus*>(_a[0]) = _r; }  break;
        case 13: { bool _r = _t->setPassword((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: { bool _r = _t->store((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->store();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: { bool _r = _t->init((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< quint64(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { bool _r = _t->init((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { bool _r = _t->init((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: { bool _r = _t->init((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 20: _t->initAsync((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< quint64(*)>(_a[4]))); break;
        case 21: _t->initAsync((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 22: _t->initAsync((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 23: _t->initAsync((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 24: _t->setDaemonLogin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 25: _t->setDaemonLogin((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 26: _t->setDaemonLogin(); break;
        case 27: { bool _r = _t->createViewOnly((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 28: { bool _r = _t->connectToDaemon();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 29: _t->setTrustedDaemon((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: { quint64 _r = _t->balance();
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 31: { quint64 _r = _t->unlockedBalance();
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 32: { bool _r = _t->viewOnly();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 33: { quint64 _r = _t->blockChainHeight();
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 34: { quint64 _r = _t->daemonBlockChainHeight();
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 35: { quint64 _r = _t->daemonBlockChainTargetHeight();
            if (_a[0]) *reinterpret_cast< quint64*>(_a[0]) = _r; }  break;
        case 36: { bool _r = _t->refresh();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 37: _t->refreshAsync(); break;
        case 38: _t->setAutoRefreshInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: { int _r = _t->autoRefreshInterval();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 40: _t->startRefresh(); break;
        case 41: _t->pauseRefresh(); break;
        case 42: { PendingTransaction* _r = _t->createTransaction((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint64(*)>(_a[3])),(*reinterpret_cast< quint32(*)>(_a[4])),(*reinterpret_cast< PendingTransaction::Priority(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< PendingTransaction**>(_a[0]) = _r; }  break;
        case 43: _t->createTransactionAsync((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint64(*)>(_a[3])),(*reinterpret_cast< quint32(*)>(_a[4])),(*reinterpret_cast< PendingTransaction::Priority(*)>(_a[5]))); break;
        case 44: { PendingTransaction* _r = _t->createTransactionAll((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint32(*)>(_a[3])),(*reinterpret_cast< PendingTransaction::Priority(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< PendingTransaction**>(_a[0]) = _r; }  break;
        case 45: _t->createTransactionAllAsync((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint32(*)>(_a[3])),(*reinterpret_cast< PendingTransaction::Priority(*)>(_a[4]))); break;
        case 46: { PendingTransaction* _r = _t->createSweepUnmixableTransaction();
            if (_a[0]) *reinterpret_cast< PendingTransaction**>(_a[0]) = _r; }  break;
        case 47: _t->createSweepUnmixableTransactionAsync(); break;
        case 48: { UnsignedTransaction* _r = _t->loadTxFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< UnsignedTransaction**>(_a[0]) = _r; }  break;
        case 49: { bool _r = _t->submitTxFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 50: _t->disposeTransaction((*reinterpret_cast< PendingTransaction*(*)>(_a[1]))); break;
        case 51: _t->disposeTransaction((*reinterpret_cast< UnsignedTransaction*(*)>(_a[1]))); break;
        case 52: { QString _r = _t->generatePaymentId();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 53: { QString _r = _t->integratedAddress((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 54: { QString _r = _t->signMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 55: { QString _r = _t->signMessage((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 56: { bool _r = _t->verifySignedMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 57: { bool _r = _t->verifySignedMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 58: { bool _r = _t->parse_uri((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< uint64_t(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QVector<QString>(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 59: { bool _r = _t->setUserNote((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 60: { QString _r = _t->getUserNote((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 61: { QString _r = _t->getTxKey((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 62: { bool _r = _t->rescanSpent();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 63: { bool _r = _t->useForkRules((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 64: { bool _r = _t->useForkRules((*reinterpret_cast< quint8(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PendingTransaction* >(); break;
            }
            break;
        case 50:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PendingTransaction* >(); break;
            }
            break;
        case 51:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< UnsignedTransaction* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Wallet::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wallet::updated)) {
                *result = 0;
            }
        }
        {
            typedef void (Wallet::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wallet::refreshed)) {
                *result = 1;
            }
        }
        {
            typedef void (Wallet::*_t)(const QString & , quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wallet::moneySpent)) {
                *result = 2;
            }
        }
        {
            typedef void (Wallet::*_t)(const QString & , quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wallet::moneyReceived)) {
                *result = 3;
            }
        }
        {
            typedef void (Wallet::*_t)(const QString & , quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wallet::unconfirmedMoneyReceived)) {
                *result = 4;
            }
        }
        {
            typedef void (Wallet::*_t)(quint64 , quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wallet::newBlock)) {
                *result = 5;
            }
        }
        {
            typedef void (Wallet::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wallet::historyModelChanged)) {
                *result = 6;
            }
        }
        {
            typedef void (Wallet::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wallet::walletCreationHeightChanged)) {
                *result = 7;
            }
        }
        {
            typedef void (Wallet::*_t)(PendingTransaction * , QString , QString , quint32 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wallet::transactionCreated)) {
                *result = 8;
            }
        }
        {
            typedef void (Wallet::*_t)(ConnectionStatus ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Wallet::connectionStatusChanged)) {
                *result = 9;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Wallet *_t = static_cast<Wallet *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getSeed(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getSeedLanguage(); break;
        case 2: *reinterpret_cast< Status*>(_v) = _t->status(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->testnet(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->synchronized(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->errorString(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->address(); break;
        case 7: *reinterpret_cast< quint64*>(_v) = _t->balance(); break;
        case 8: *reinterpret_cast< quint64*>(_v) = _t->unlockedBalance(); break;
        case 9: *reinterpret_cast< TransactionHistory**>(_v) = _t->history(); break;
        case 10: *reinterpret_cast< QString*>(_v) = _t->paymentId(); break;
        case 11: *reinterpret_cast< TransactionHistorySortFilterModel**>(_v) = _t->historyModel(); break;
        case 12: *reinterpret_cast< QString*>(_v) = _t->path(); break;
        case 13: *reinterpret_cast< AddressBookModel**>(_v) = _t->addressBookModel(); break;
        case 14: *reinterpret_cast< AddressBook**>(_v) = _t->addressBook(); break;
        case 15: *reinterpret_cast< bool*>(_v) = _t->viewOnly(); break;
        case 16: *reinterpret_cast< QString*>(_v) = _t->getSecretViewKey(); break;
        case 17: *reinterpret_cast< QString*>(_v) = _t->getPublicViewKey(); break;
        case 18: *reinterpret_cast< QString*>(_v) = _t->getSecretSpendKey(); break;
        case 19: *reinterpret_cast< QString*>(_v) = _t->getPublicSpendKey(); break;
        case 20: *reinterpret_cast< QString*>(_v) = _t->getDaemonLogPath(); break;
        case 21: *reinterpret_cast< QString*>(_v) = _t->getWalletLogPath(); break;
        case 22: *reinterpret_cast< quint64*>(_v) = _t->getWalletCreationHeight(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Wallet *_t = static_cast<Wallet *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 10: _t->setPaymentId(*reinterpret_cast< QString*>(_v)); break;
        case 22: _t->setWalletCreationHeight(*reinterpret_cast< quint64*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Wallet::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Wallet.data,
      qt_meta_data_Wallet,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Wallet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Wallet::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Wallet.stringdata0))
        return static_cast<void*>(const_cast< Wallet*>(this));
    return QObject::qt_metacast(_clname);
}

int Wallet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 65)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 65;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 65)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 65;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 23;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Wallet::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Wallet::refreshed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void Wallet::moneySpent(const QString & _t1, quint64 _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Wallet::moneyReceived(const QString & _t1, quint64 _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Wallet::unconfirmedMoneyReceived(const QString & _t1, quint64 _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Wallet::newBlock(quint64 _t1, quint64 _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Wallet::historyModelChanged()const
{
    QMetaObject::activate(const_cast< Wallet *>(this), &staticMetaObject, 6, Q_NULLPTR);
}

// SIGNAL 7
void Wallet::walletCreationHeightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}

// SIGNAL 8
void Wallet::transactionCreated(PendingTransaction * _t1, QString _t2, QString _t3, quint32 _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Wallet::connectionStatusChanged(ConnectionStatus _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< Wallet *>(this), &staticMetaObject, 9, _a);
}
QT_END_MOC_NAMESPACE
