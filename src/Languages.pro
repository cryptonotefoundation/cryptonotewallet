CODECFORTR = UTF-8

CODECFORSRC = UTF-8

 

INCLUDEPATH = . \
			  ./gui \
			  ./gui/ui \


DEPENDPATH = $$INCLUDEPATH

 

SOURCES = main.cpp \
CommandLineParser.cpp \
CryptoNoteWrapper.cpp \
CurrencyAdapter.cpp \
LoggerAdapter.cpp \
main.cpp \
Miner.cpp \
NodeAdapter.cpp \
Settings.cpp \
SignalHandler.cpp \
StratumClient.cpp \
WalletAdapter.cpp \
Worker.cpp \
PaymentServer.cpp \
gui/AboutDialog.cpp \
gui/AddressBookDialog.cpp \
gui/AddressBookFrame.cpp \
gui/AddressBookModel.cpp \
gui/AnimatedLabel.cpp \
gui/ChangePasswordDialog.cpp \
gui/ExitWidget.cpp \
gui/ImportKeyDialog.cpp \
gui/MainWindow.cpp \
gui/MiningFrame.cpp \
gui/NewAddressDialog.cpp \
gui/NewPasswordDialog.cpp \
gui/NewPoolDialog.cpp \
gui/OverviewFrame.cpp \
gui/PasswordDialog.cpp \
gui/PoolModel.cpp \
gui/AccountFrame.cpp \
gui/ReceiveFrame.cpp \
gui/RecentTransactionsModel.cpp \
gui/SendFrame.cpp \
gui/SortedTransactionsModel.cpp \
gui/TransactionDetailsDialog.cpp \
gui/TransactionFrame.cpp \
gui/TransactionsFrame.cpp \
gui/TransactionsListModel.cpp \
gui/TransactionsModel.cpp \
gui/TransferFrame.cpp \
update.cpp \
gui/ConnectionSettings.cpp \
gui/NewNodeDialog.cpp \
gui/NodeModel.cpp \
gui/QRLabel.cpp \
gui/OpenUriDialog.cpp \
gui/ConfirmSendDialog.cpp \
gui/ExportTrackingKeyDialog.cpp \
gui/ImportTrackingKeyDialog.cpp \
gui/InfoDialog.cpp \
gui/MnemonicSeedDialog.cpp \
gui/RestoreFromMnemonicSeedDialog.cpp \
gui/VerifyMnemonicSeedDialog.cpp \
gui/NoWalletFrame.cpp \
gui/SendGlassFrame.cpp \
gui/SignMessageDialog.cpp \
gui/OptimizationSettings.cpp \
gui/GetBalanceProofDialog.cpp \
 

HEADERS = CommandLineParser.h \
CryptoNoteWrapper.h \
CurrencyAdapter.h \
LoggerAdapter.h \
Miner.h \
miniupnpcstrings.h \
NodeAdapter.h \
Settings.h \
SignalHandler.h \
StratumClient.h \
WalletAdapter.h \
Worker.h \
PaymentServer.h \
gui/AboutDialog.h \
gui/AddressBookDialog.h \
gui/AddressBookFrame.h \
gui/AddressBookModel.h \
gui/AnimatedLabel.h \
gui/ChangePasswordDialog.h \
gui/ExitWidget.h \
gui/ImportKeyDialog.h \
gui/MainWindow.h \
gui/MiningFrame.h \
gui/NewAddressDialog.h \
gui/NewPasswordDialog.h \
gui/NewPoolDialog.h \
gui/OverviewFrame.h \
gui/PasswordDialog.h \
gui/PoolModel.h \
gui/AccountFrame.h \
gui/ReceiveFrame.h \
gui/RecentTransactionsModel.h \
gui/SendFrame.h \
gui/SortedTransactionsModel.h \
gui/TransactionDetailsDialog.h \
gui/TransactionFrame.h \
gui/TransactionsFrame.h \
gui/TransactionsListModel.h \
gui/TransactionsModel.h \
gui/TransferFrame.h \
gui/WalletEvents.h \
Update.h \
gui/ConnectionSettings.h \
gui/NewNodeDialog.h \
gui/NodeModel.h \
gui/QRLabel.h \
gui/OpenUriDialog.h \
gui/ConfirmSendDialog.h \
gui/ExportTrackingKeyDialog.h \
gui/ImportTrackingKeyDialog.h \
gui/InfoDialog.h \
gui/MnemonicSeedDialog.h \
gui/RestoreFromMnemonicSeedDialog.h \
gui/VerifyMnemonicSeedDialog.h \
gui/NoWalletFrame.h \
gui/SendGlassFrame.h \
gui/SignMessageDialog.h \
gui/OptimizationSettings.h \
gui/GetBalanceProofDialog.h \


FORMS = gui/ui/aboutdialog.ui \
gui/ui/addressbookdialog.ui \
gui/ui/addressbookframe.ui \
gui/ui/changepassworddialog.ui \
gui/ui/exitwidget.ui \
gui/ui/importkeydialog.ui \
gui/ui/mainwindow.ui \
gui/ui/miningframe.ui \
gui/ui/newaddressdialog.ui \
gui/ui/newpassworddialog.ui \
gui/ui/newpooldialog.ui \
gui/ui/overviewframe.ui \
gui/ui/passworddialog.ui \
gui/ui/receiveframe.ui \
gui/ui/accountframe.ui \
gui/ui/sendframe.ui \
gui/ui/transactiondetailsdialog.ui \
gui/ui/transactionframe.ui \
gui/ui/transactionsframe.ui \
gui/ui/transferframe.ui \
gui/ui/changelanguagedialog.ui \
gui/ui/privatekeysdialog.ui \
gui/ui/connectionsettingsdialog.ui \
gui/ui/newnodedialog.ui \
gui/ui/showpaymentrequest.ui \
gui/ui/openuridialog.ui \
gui/ui/confirmsenddialog.ui \
gui/ui/importtrackingkeydialog.ui \
gui/ui/exporttrackingkeydialog.ui \
gui/ui/infodialog.ui \
gui/ui/mnemonicseeddialog.ui \
gui/ui/restorefrommnemonicseeddialog.ui \
gui/ui/verifymnemonicseeddialog.ui \
gui/ui/nowalletframe.ui \
gui/ui/signmessagedialog.ui \
gui/ui/optimizationsettingsdialog.ui \
gui/ui/getbalanceproofdialog.ui \


TRANSLATIONS = 	languages/uk.ts \
				languages/ru.ts \
				languages/pl.ts \
				languages/be.ts \
				languages/de.ts \
				languages/es.ts \
				languages/fr.ts \
				languages/pt.ts \
				languages/ja.ts \
				languages/it.ts \
				languages/ko.ts \
				languages/zh.ts \
				languages/hi.ts \
				languages/ar.ts \
				languages/fa.ts \