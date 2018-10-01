

// Copyright (c) 2014-2015, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import QtQuick 2.2
import QtGraphicalEffects 1.0
import moneroComponents.Wallet 1.0
import "components"

Rectangle {
    id: panel

    property alias unlockedBalanceText: unlockedBalanceText.text
    property alias balanceLabelText: balanceLabel.text
    property alias balanceText: balanceText.text
    property alias networkStatus : networkStatus
    property alias progressBar : progressBar
    property alias minutesToUnlockTxt: unlockedBalanceLabel.text

    signal dashboardClicked()
    signal historyClicked()
    signal transferClicked()
    signal receiveClicked()
    signal txkeyClicked()
    signal settingsClicked()
    signal addressBookClicked()
    signal miningClicked()
    signal signClicked()
    //signal intenseClicked()
    signal intenseDashboardClicked()
    signal intenseProviderClicked()

    function selectItem(pos) {
        menuColumn.previousButton.checked = false
        if(pos === "Dashboard") menuColumn.previousButton = dashboardButton
        else if(pos === "History") menuColumn.previousButton = historyButton
        else if(pos === "Transfer") menuColumn.previousButton = transferButton
        else if(pos === "Receive")  menuColumn.previousButton = receiveButton
        else if(pos === "AddressBook") menuColumn.previousButton = addressBookButton
        else if(pos === "Mining") menuColumn.previousButton = miningButton
        else if(pos === "TxKey")  menuColumn.previousButton = txkeyButton
        else if(pos === "Sign") menuColumn.previousButton = signButton
        else if(pos === "Settings") menuColumn.previousButton = settingsButton

        else if(pos === "Intense") menuColumn.previousButton = intenseButton
        else if(pos === "VPN Dashboard") menuColumn.previousButton = intenseDashboardButton
        else if(pos === "Provider") menuColumn.previousButton = intenseProviderButton
        else if(pos === "Advanced") menuColumn.previousButton = advancedButton

        menuColumn.previousButton.checked = true
    }

    width: (isMobile)? appWindow.width : 260
    color: "#FFFFFF"

    // Item with monero logo
    Item {
        visible: !isMobile
        id: logoItem
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: (persistentSettings.customDecorations)? 66 : 36
        height: logo.implicitHeight

        Image {
            id: logo
            anchors.left: parent.left
            anchors.leftMargin: 50
            source: "images/moneroLogo.png"
        }

        Text {
            id: testnetLabel
            visible: persistentSettings.testnet
            text: qsTr("Testnet") + translationManager.emptyString
            anchors.top: logo.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 50
            font.bold: true
            color: "red"
        }

      /* Disable twitter/news panel
        Image {
            anchors.left: parent.left
            anchors.verticalCenter: logo.verticalCenter
            anchors.leftMargin: 19
            source: appWindow.rightPanelExpanded ? "images/expandRightPanel.png" :
                                                   "images/collapseRightPanel.png"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: appWindow.rightPanelExpanded = !appWindow.rightPanelExpanded
        }
      */
    }



    Column {
        visible: !isMobile
        id: column1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: logoItem.bottom
        anchors.topMargin: 26
        spacing: 5

        Label {
            visible: !isMobile
            id: balanceLabel
            text: qsTr("Balance") + translationManager.emptyString
            anchors.left: parent.left
            anchors.leftMargin: 50
        }

        Row {
            visible: !isMobile
            Item {
                anchors.verticalCenter: parent.verticalCenter
                height: 26
                width: 50

                Image {
                    anchors.centerIn: parent
                    source: "images/lockIcon.png"
                }
            }

            Text {
                visible: !isMobile
                id: balanceText
                anchors.verticalCenter: parent.verticalCenter
                font.family: "Arial"
                color: "#000000"
                text: "N/A"
                // dynamically adjust text size
                font.pixelSize: {
                    var digits = text.split('.')[0].length
                    var defaultSize = 25;
                    if(digits > 2) {
                        return defaultSize - 1.1*digits
                    }
                    return defaultSize;
                }
            }
        }

        Item { //separator
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
        }

        Label {
            id: unlockedBalanceLabel
            text: qsTr("Unlocked balance") + translationManager.emptyString
            anchors.left: parent.left
            anchors.leftMargin: 50
        }

        Text {
            id: unlockedBalanceText
            anchors.left: parent.left
            anchors.leftMargin: 50
            font.family: "Arial"
            color: "#000000"
            text: "N/A"
            // dynamically adjust text size
            font.pixelSize: {
                var digits = text.split('.')[0].length
                var defaultSize = 18;
                if(digits > 3) {
                    return defaultSize - 0.6*digits
                }
                return defaultSize;
            }
        }
    }


    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: menuRect.top
        width: 1
        color: "#DBDBDB"
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 1
        color: "#DBDBDB"
    }



    Rectangle {
        id: menuRect
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: (isMobile)? parent.top : column1.bottom
        anchors.topMargin: (isMobile)? 0 : 25
        color: "#1C1C1C"


        Flickable {
            contentHeight: 500
            anchors.fill: parent
            clip: true


        Column {

            id: menuColumn
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            property var previousButton: transferButton

            // ------------- Dashboard tab ---------------

            /*
            MenuButton {
                id: dashboardButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Dashboard") + translationManager.emptyString
                symbol: qsTr("D") + translationManager.emptyString
                dotColor: "#6C8896"
                checked: true
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = dashboardButton
                    panel.dashboardClicked()
                }
            }


            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: dashboardButton.checked || transferButton.checked ? "#1C1C1C" : "#505050"
                height: 1
            }
            */


            // ------------- Transfer tab ---------------
            MenuButton {
                id: transferButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Send") + translationManager.emptyString
                symbol: qsTr("S") + translationManager.emptyString
                dotColor: "#6C8896"
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = transferButton
                    panel.transferClicked()
                }
            }

            Rectangle {
                visible: transferButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: "#505050"
                height: 1
            }

            // ------------- AddressBook tab ---------------

            MenuButton {
                id: addressBookButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Address book") + translationManager.emptyString
                symbol: qsTr("B") + translationManager.emptyString
                dotColor: "#8441ff"
                under: transferButton
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = addressBookButton
                    panel.addressBookClicked()
                }
            }

            Rectangle {
                visible: addressBookButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: "#505050"
                height: 1
            }

            // ------------- Receive tab ---------------
            MenuButton {
                id: receiveButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Receive") + translationManager.emptyString
                symbol: qsTr("R") + translationManager.emptyString
                dotColor: "#AAFFBB"
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = receiveButton
                    panel.receiveClicked()
                }
            }
            Rectangle {
                visible: receiveButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: "#505050"
                height: 1
            }

            // ------------- History tab ---------------

            MenuButton {
                id: historyButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("History") + translationManager.emptyString
                symbol: qsTr("H") + translationManager.emptyString
                dotColor: "#6C8896"
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = historyButton
                    panel.historyClicked()
                }
            }
            Rectangle {
                visible: historyButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: "#505050"
                height: 1
            }

            // ------------- Advanced tab ---------------
            MenuButton {
                id: advancedButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Advanced") + translationManager.emptyString
                symbol: qsTr("D") + translationManager.emptyString
                dotColor: "#FFD781"
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = advancedButton
                }
            }
            Rectangle {
                visible: advancedButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: "#505050"
                height: 1
            }

            // ------------- Mining tab ---------------
            MenuButton {
                id: miningButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Mining") + translationManager.emptyString
                symbol: qsTr("M") + translationManager.emptyString
                dotColor: "#FFD781"
                under: advancedButton
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = miningButton
                    panel.miningClicked()
                }
            }

            Rectangle {
                visible: miningButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: miningButton.checked || settingsButton.checked ? "#1C1C1C" : "#505050"
                height: 1
            }
            // ------------- TxKey tab ---------------
            MenuButton {
                id: txkeyButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Check payment") + translationManager.emptyString
                symbol: qsTr("K") + translationManager.emptyString
                dotColor: "#FFD781"
                under: advancedButton
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = txkeyButton
                    panel.txkeyClicked()
                }
            }
            Rectangle {
                visible: txkeyButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: "#505050"
                height: 1
            }

            // ------------- Sign/verify tab ---------------
            MenuButton {
                id: signButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Sign/verify") + translationManager.emptyString
                symbol: qsTr("I") + translationManager.emptyString
                dotColor: "#FFD781"
                under: advancedButton
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = signButton
                    panel.signClicked()
                }
            }
            Rectangle {
                visible: signButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: "#505050"
                height: 1
            }
            // ------------- Settings tab ---------------
            MenuButton {
                id: settingsButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Settings") + translationManager.emptyString
                symbol: qsTr("E") + translationManager.emptyString
                dotColor: "#36B25C"
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = settingsButton
                    panel.settingsClicked()
                }
            }
            Rectangle {
                visible: settingsButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: "#505050"
                height: 1
            }


            // ------------- Intese Coin tab ---------------
            MenuButton {
                id: intenseButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("VPN + Proxy") + translationManager.emptyString
                symbol: qsTr("S") + translationManager.emptyString
                dotColor: "#36B25C"
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = intenseButton
                    //panel.intenseClicked()
                }
            }
            Rectangle {
                visible: intenseButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: "#505050"
                height: 1
            }

            MenuButton {
                id: intenseDashboardButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Dashboard") + translationManager.emptyString
                symbol: qsTr("D") + translationManager.emptyString
                dotColor: "#FFD781"
                under: intenseButton
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = intenseDashboardButton
                    panel.intenseDashboardClicked()
                }
            }

            Rectangle {
                visible: intenseDashboardButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: intenseDashboardButton.checked || intenseButton.checked ? "#1C1C1C" : "#505050"
                height: 1
            }

            MenuButton {
                id: intenseProviderButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Provider") + translationManager.emptyString
                symbol: qsTr("P") + translationManager.emptyString
                dotColor: "#36B25C"
                under: intenseButton
                onClicked: {
                    parent.previousButton.checked = false
                    parent.previousButton = intenseProviderButton
                    panel.intenseProviderClicked()
                }
            }
            Rectangle {
                visible: intenseProviderButton.present
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 16
                color: intenseProviderButton.checked || intenseButton.checked ? "#1C1C1C" : "#505050"
                height: 1
            }

        }

        }

        NetworkStatusItem {
            id: networkStatus
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: (progressBar.visible)? progressBar.top : parent.bottom;
            connected: Wallet.ConnectionStatus_Disconnected
        }

        ProgressBar {
            id: progressBar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    }



    // indicate disabled state
//    Desaturate {
//        anchors.fill: parent
//        source: parent
//        desaturation: panel.enabled ? 0.0 : 1.0
//    }


}
