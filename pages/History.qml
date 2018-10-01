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

import QtQuick 2.0

import moneroComponents.Wallet 1.0
import moneroComponents.WalletManager 1.0
import moneroComponents.TransactionHistory 1.0
import moneroComponents.TransactionInfo 1.0
import moneroComponents.TransactionHistoryModel 1.0

import "../components"

Rectangle {
    id: root
    property var model

    QtObject {
        id: d
        property bool initialized: false
    }

    color: "#F0EEEE"

    function getSelectedAmount() {
      if (typeof model === 'undefined' || model == null)
        return ""
      var total = 0
      var count = model.rowCount()
      for (var i = 0; i < count; ++i) {
          var idx = model.index(i, 0)
          var isout = model.data(idx, TransactionHistoryModel.TransactionIsOutRole);
          var amount = model.data(idx, TransactionHistoryModel.TransactionAtomicAmountRole);
          if (isout)
              total = walletManager.subi(total, amount)
          else
              total = walletManager.addi(total, amount)
      }

      var sign = ""
      if (total < 0) {
        total = -total
        sign = "-"
      }
      return count + qsTr(" selected: ") + sign + walletManager.displayAmount(total);
    }

    function resetFilter(model) {
        model.dateFromFilter = "2014-04-18" // genesis block
        model.dateToFilter = "9999-09-09" // fix before september 9999
        // negative values disable filters here;
        model.amountFromFilter = -1;
        model.amountToFilter = -1;
        model.directionFilter = TransactionInfo.Direction_Both;
    }

    onModelChanged: {
        if (typeof model !== 'undefined') {

            selectedAmount.text = getSelectedAmount()

            if (!d.initialized) {
                // setup date filter scope according to real transactions
                fromDatePicker.currentDate = model.transactionHistory.firstDateTime
                toDatePicker.currentDate = model.transactionHistory.lastDateTime

                /* Default sorting by timestamp desc */
                /* Sort indicator on table header */
                /* index of 'sort by blockheight' column */
                header.activeSortColumn = 2
                /* Sorting model */

                model.sortRole = TransactionHistoryModel.TransactionBlockHeightRole
                model.sort(0, Qt.DescendingOrder);
                d.initialized = true
                // TODO: public interface for 'Header' item that will cause 'sortRequest' signal
            }

        }
    }

    function onFilterChanged() {
        var datesValid = fromDatePicker.currentDate <= toDatePicker.currentDate
        var amountsValid = amountFromLine.text === "" ? true :
                            amountToLine.text === "" ? true:
                                parseFloat(amountFromLine.text) <= parseFloat(amountToLine.text)

        // reset error state if amount filter valid
        if (amountsValid) {
            amountFromLine.error = amountToLine.error = false
        }

        filterButton.enabled = datesValid && amountsValid
    }


    Text {
        visible: !isMobile
        id: filterHeaderText
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 17
        anchors.topMargin: 17

        elide: Text.ElideRight
        font.family: "Arial"
        font.pixelSize: 18
        color: "#4A4949"
        text: qsTr("Filter transaction history") + translationManager.emptyString
    }

    Label {
        visible: !isMobile
        id: selectedAmount
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 17
        anchors.topMargin: 17
        text: getSelectedAmount()
        fontSize: 14
    }


    // Filter by Address input (senseless, removing)
    /*
    Label {
        id: addressLabel
        anchors.left: parent.left
        anchors.top: filterHeaderText.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 17
        text: qsTr("Address")
        fontSize: 14
    }

    LineEdit {
        id: addressLine
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: addressLabel.bottom
        anchors.leftMargin: 17
        anchors.rightMargin: 17
        anchors.topMargin: 5
    }
    */

    // Filter by string
    LineEdit {
        visible: !isMobile
        id: searchLine
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: filterHeaderText.bottom
        anchors.leftMargin: 17
        anchors.rightMargin: 17
        anchors.topMargin: 5
        placeholderText: qsTr("Type for incremental search...") + translationManager.emptyString
        onTextChanged:  {
            model.searchFilter = searchLine.text
            selectedAmount.text = getSelectedAmount()
        }
    }

    // Filter by description input (not implemented yet)
    /*
    Label {
        id: descriptionLabel
        anchors.left: parent.left
        anchors.top: searchLine.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 17
        text: qsTr("Description <font size='2'>(Local database)</font>") + translationManager.emptyString
        fontSize: 14
    }

    LineEdit {
        id: descriptionLine
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: descriptionLabel.bottom
        anchors.leftMargin: 17
        anchors.rightMargin: 17
        anchors.topMargin: 5
    }
    */


    // DateFrom picker
    Label {
        visible: !isMobile
        id: dateFromText
        anchors.left: parent.left
        anchors.top:  searchLine.bottom // descriptionLine.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 17
        width: 156
        text: qsTr("Date from") + translationManager.emptyString
        fontSize: 14
    }

    DatePicker {
        visible: !isMobile
        id: fromDatePicker
        anchors.left: parent.left
        anchors.top: dateFromText.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 5
        z: 2
        onCurrentDateChanged: {
            error = currentDate > toDatePicker.currentDate
            onFilterChanged()
        }
    }

    // DateTo picker
    Label {
        visible: !isMobile
        id: dateToText
        anchors.left: dateFromText.right
        anchors.top:  searchLine.bottom //descriptionLine.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 17
        text: qsTr("To") + translationManager.emptyString
        fontSize: 14
    }

    DatePicker {
        visible: !isMobile
        id: toDatePicker
        anchors.left: fromDatePicker.right
        anchors.top: dateToText.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 5
        z: 2
        onCurrentDateChanged: {
            error = currentDate < fromDatePicker.currentDate
            onFilterChanged()
        }
    }



    StandardButton {
        visible: !isMobile
        id: filterButton
        anchors.bottom: toDatePicker.bottom
        anchors.left: toDatePicker.right
        anchors.leftMargin: 17
        width: 60
        text: qsTr("Filter") + translationManager.emptyString
        shadowReleasedColor: "#A7B8C0"
        shadowPressedColor: "#666e71"
        releasedColor: "#6C8896"
        pressedColor: "#A7B8C0"
        onClicked:  {
            // Apply filter here;

            resetFilter(model)

            if (fromDatePicker.currentDate > toDatePicker.currentDate) {
                console.error("Invalid date filter set: ", fromDatePicker.currentDate, toDatePicker.currentDate)
            } else {
                model.dateFromFilter  = fromDatePicker.currentDate
                model.dateToFilter    = toDatePicker.currentDate
            }

            if (advancedFilteringCheckBox.checked) {
                if (amountFromLine.text.length) {
                    model.amountFromFilter = parseFloat(amountFromLine.text)
                }

                if (amountToLine.text.length) {
                    model.amountToFilter = parseFloat(amountToLine.text)
                }

                var directionFilter = transactionsModel.get(transactionTypeDropdown.currentIndex).value
                console.log("Direction filter: " + directionFilter)
                model.directionFilter = directionFilter
            }

            selectedAmount.text = getSelectedAmount()
        }
    }

    CheckBox {
        visible: !isMobile
        id: advancedFilteringCheckBox
        text: qsTr("Advanced filtering") + translationManager.emptyString
        anchors.left: filterButton.right
        anchors.bottom: filterButton.bottom
        anchors.leftMargin: 17
        checkedIcon: "../images/checkedVioletIcon.png"
        uncheckedIcon: "../images/uncheckedIcon.png"
        onClicked: {
            if(checked) tableRect.height = Qt.binding(function(){ return tableRect.collapsedHeight })
            else tableRect.height = Qt.binding(function(){ return tableRect.middleHeight })
        }
    }

    Label {
        visible: !isMobile
        id: transactionTypeText
        anchors.left: parent.left
        anchors.top: fromDatePicker.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 17
        width: 156
        text: qsTr("Type of transaction") + translationManager.emptyString
        fontSize: 14
    }

    ListModel {
        id: transactionsModel
        ListElement { column1: "All"; column2: ""; value: TransactionInfo.Direction_Both }
        ListElement { column1: "Sent"; column2: ""; value: TransactionInfo.Direction_Out }
        ListElement { column1: "Received"; column2: ""; value: TransactionInfo.Direction_In }

    }

    StandardDropdown {
        visible: !isMobile
        id: transactionTypeDropdown
        anchors.left: parent.left
        anchors.top: transactionTypeText.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 5
        width: 156
        shadowReleasedColor: "#A7B8C0"
        shadowPressedColor: "#666e71"
        releasedColor: "#6C8896"
        pressedColor: "#A7B8C0"
        dataModel: transactionsModel
        z: 1
    }

    Label {
        visible: !isMobile
        id: amountFromText
        anchors.left: transactionTypeText.right
        anchors.top: fromDatePicker.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 17
        width: 156
        text: qsTr("Amount from") + translationManager.emptyString
        fontSize: 14
    }

    LineEdit {
        visible: !isMobile
        id: amountFromLine
        anchors.left: transactionTypeDropdown.right
        anchors.top: amountFromText.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 5
        width: 156
        validator: DoubleValidator {
            locale: "C"
            notation: DoubleValidator.StandardNotation
            bottom: 0
        }
        onTextChanged: {
            // indicating error
            amountFromLine.error = amountFromLine.text === "" ? false : parseFloat(amountFromLine.text) > parseFloat(amountToLine.text)
            onFilterChanged()
        }

    }

    Label {
        visible: !isMobile
        id: amountToText
        anchors.left: amountFromText.right
        anchors.top: fromDatePicker.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 17
        width: 156
        text: qsTr("To") + translationManager.emptyString
        fontSize: 14
    }

    LineEdit {
        visible: !isMobile
        id: amountToLine
        anchors.left: amountFromLine.right
        anchors.top: amountToText.bottom
        anchors.leftMargin: 17
        anchors.topMargin: 5
        width: 156
        validator: DoubleValidator {
            locale: "C"
            notation: DoubleValidator.StandardNotation
            bottom: 0.0
        }

        onTextChanged: {
            // indicating error
            amountToLine.error = amountToLine.text === "" ? false : parseFloat(amountFromLine.text) > parseFloat(amountToLine.text)
            onFilterChanged()
        }

    }

    Item {
        visible: !isMobile
        id: expandItem
        property bool expanded: false

        anchors.right: parent.right
        anchors.bottom: tableRect.top
        width: 34
        height: 34

        Image {
            anchors.centerIn: parent
            source: "../images/expandTable.png"
            rotation: parent.expanded ? 180 : 0
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.expanded = !parent.expanded
                if (advancedFilteringCheckBox.checked) {
                    tableRect.height = Qt.binding(function() { return parent.expanded ? tableRect.expandedHeight : tableRect.collapsedHeight })
                } else {
                    tableRect.height = Qt.binding(function() { return parent.expanded ? tableRect.expandedHeight : tableRect.middleHeight })
                }
            }
        }
    }

    Rectangle {
        id: tableRect
        property int expandedHeight: parent.height - filterHeaderText.y - filterHeaderText.height - 5
        property int middleHeight: parent.height - fromDatePicker.y - fromDatePicker.height - 17
        property int collapsedHeight: parent.height - transactionTypeDropdown.y - transactionTypeDropdown.height - 17
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
//        anchors.top: parent.top
        color: "#FFFFFF"
        z: 1

        height: (isMobile)? parent.height : middleHeight
        onHeightChanged: {
            if(height === middleHeight) z = 1
            else if(height === collapsedHeight) z = 0
            else z = 3
        }

        Behavior on height {
            NumberAnimation { duration: 200; easing.type: Easing.InQuad }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            color: "#DBDBDB"
        }

        ListModel {
            id: columnsModel

            ListElement { columnName: "Payment ID"; columnWidth: 127 }
            ListElement { columnName: "Date"; columnWidth: 100 }
            ListElement { columnName: "Block height"; columnWidth: 150 }
            ListElement { columnName: "Amount"; columnWidth: 148 }
            // ListElement { columnName: "Description"; columnWidth: 148 }
        }

        TableHeader {
            id: header
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 17
            anchors.leftMargin: 14
            anchors.rightMargin: 14
            dataModel: columnsModel
            offset: 20
            onSortRequest: {
                console.log("column: " + column + " desc: " + desc)
                switch (column) {
                case 0:
                    // Payment ID
                    model.sortRole = TransactionHistoryModel.TransactionPaymentIdRole
                    break;
                case 1:
                    // Date (actually sort by timestamp as we want to have transactions sorted within one day as well);
                    model.sortRole = TransactionHistoryModel.TransactionTimeStampRole
                    break;
                case 2:
                    // BlockHeight;
                    model.sortRole = TransactionHistoryModel.TransactionBlockHeightRole
                    break;
                case 3:
                    // Amount;
                    model.sortRole = TransactionHistoryModel.TransactionAmountRole
                    break;
                }
                model.sort(0, desc ? Qt.DescendingOrder : Qt.AscendingOrder)
            }
        }

        Scroll {
            id: flickableScroll
            anchors.right: table.right
            anchors.rightMargin: -14
            anchors.top: table.top
            anchors.bottom: table.bottom
            flickable: table
        }

        HistoryTable {
            id: table
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            anchors.leftMargin: 14
            anchors.rightMargin: 14
            onContentYChanged: flickableScroll.flickableContentYChanged()
            model: root.model
            addressBookModel: null
        }
    }

    function onPageCompleted() {
        table.addressBookModel = appWindow.currentWallet ? appWindow.currentWallet.addressBookModel : null
    }
}
