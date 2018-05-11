import QtQuick 2.0
import moneroComponents.TransactionInfo 1.0
import "../components"

Rectangle {
    id: root
    property var model

    function buildTxDetailsString(data, rank) {
        var trStart = '<tr><td width="85" style="padding-top:5px"><b>',
            trMiddle = '</b></td><td style="padding-left:10px;padding-top:5px;">',
            trEnd = "</td></tr>";

        return '<table border="0">'
            + (data.id ? trStart + qsTr("ID: ") + trMiddle + data.id + trEnd : "")
            + (data.provider ? trStart + qsTr("Provider: ") + trMiddle + data.provider  + trEnd : "")
            + (data.name ? trStart + qsTr("Plan: ") + trMiddle + data.name + trEnd : "")
            + (data.type ? trStart + qsTr("Type: ") + trMiddle + data.type  + trEnd : "")
            + (data.cost ? trStart + qsTr("Cost:") + trMiddle + data.cost + trEnd : "")
            + (data.firstPrePaidMinutes ? trStart + qsTr("First Pre Paid Minutes:") + trMiddle + data.firstPrePaidMinutes + trEnd : "")
            + (data.firstVerificationsNeeded ? trStart + qsTr("First Verifications Needed:") + trMiddle + data.firstVerificationsNeeded + trEnd : "")
            + (data.subsequentPrePaidMinutes ? trStart + qsTr("Subsequent Pre Paid Minutes:") + trMiddle + data.subsequentPrePaidMinutes + trEnd : "")
            + (data.subsequentVerificationsNeeded ? trStart + qsTr("Subsequent Verifications Needed:") + trMiddle + data.subsequentVerificationsNeeded + trEnd : "")
            + (data.allowRefunds ? trStart + qsTr("Allow Refunds:") + trMiddle + data.allowRefunds + trEnd : "")
            + (data.downloadSpeed ? trStart + qsTr("Download Speed:") + trMiddle + formatBytes(data.downloadSpeed) + trEnd : "")
            + (data.uploadSpeed ? trStart + qsTr("Upload Speed:") + trMiddle + formatBytes(data.uploadSpeed) + trEnd : "")
            + (rank ? trStart + qsTr("Rating:") + trMiddle + rank + trEnd : "")
            + "</table>"
            + translationManager.emptyString;
    }

    function buildTxConnectionString(data) {
        var trStart = '<tr><td width="85" style="padding-top:5px"><b>',
            trMiddle = '</b></td><td style="padding-left:10px;padding-top:5px;">',
            trEnd = "</td></tr>";

        return '<table border="0">'
            //+ (data.id ? trStart + qsTr("ID: ") + trMiddle + data.id + trEnd : "")
            + (data.providerName ? trStart + qsTr("Provider: ") + trMiddle + data.providerName  + trEnd : "")
            + (data.name ? trStart + qsTr("Plan: ") + trMiddle + data.name + trEnd : "")
            + (data.type ? trStart + qsTr("Type: ") + trMiddle + data.type  + trEnd : "")
            + (data.cost ? trStart + qsTr("Cost:") + trMiddle + data.cost + trEnd : "")
            + (data.firstPrePaidMinutes ? trStart + qsTr("First Pre Paid Minutes:") + trMiddle + data.firstPrePaidMinutes + trEnd : "")
            + "</table>"
            + translationManager.emptyString;
    }

    function formatBytes(bytes,decimals) {
       if(bytes == 0) return '0 Bytes';
       var k = 1024,
           dm = decimals || 2,
           sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'],
           i = Math.floor(Math.log(bytes) / Math.log(k));
       return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
    }

    function getColor(id){
        if(id == 5){
            id = 10
        }else if(id < 5 && id > 4.5){
            id = 9
        }else if(id <= 4.5 && id > 4){
            id = 7
        }else if(id <= 4 && id > 3.5){
            id = 6
        }else if(id <= 3.5 && id > 2){
            id = 5
        }else if(id <= 2 && id > 1.5){
            id = 4
        }else if(id <= 1.5 && id > 1){
            id = 3
        }else if(id <= 1 && id > 0.5){
            id = 2
        }else{
            id = 1
        }

        switch(id){
        case 1:
            return "#ee2c2c"
            break;
        case 2:
            return "#ee6363"
            break;
        case 3:
            return "#ff7f24"
            break;
        case 4:
            return "#ffa54f"
            break;
        case 5:
            return "#ffa500"
            break;
        case 6:
            return "#ffff00"
            break;
        case 7:
            return "#caff70"
            break;
        case 8:
            return "#c0ff3e"
            break;
        case 9:
            return "#66cd00"
            break;
        case 10:
            return "#008b00"
            break;
        }

    }

    function getBackgroundColor(id){
        if(id & 1){
            return "#f0f0f0"
        } else {
            return "#fafafa"
        }
    }

    function getJson(speed, price, tp){
        var url = "https://jhx4eq5ijc.execute-api.us-east-1.amazonaws.com/dev/v1/services/search"
        var xmlhttp = new XMLHttpRequest();

        xmlhttp.onreadystatechange=function() {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                var arr = JSON.parse(xmlhttp.responseText)
                if(speed != undefined || tp != undefined || price != undefined){
                    listView.model.clear()
                }
                for(var i = 0; i < arr.length; i++) {
                    if(arr[i].mStability == null){
                        arr[i].mStability = 0
                    }
                    if(arr[i].mSpeed == null){
                        arr[i].mSpeed = 0
                    }
                    if(arr[i].type == "proxy"){
                        var type = arr[i].proxy
                    }else{
                        var type = arr[i].vpn
                    }

                    var rank = (arr[i].mStability + arr[i].mSpeed)/2
                    if(speed == undefined && tp == undefined && price == undefined){
                        listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                    }

                    // C1
                    if(speed == "" && price == "" && tp == "all"){
                        listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                    }
                    // AC1
                    else if(speed <= arr[i].downloadSpeed && price == "" && tp == "all"){
                        listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                    }
                    // BC1
                    else if(speed == "" && price >= arr[i].cost && tp == "all"){
                        listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                    }
                    // C2
                    else if(speed == "" && price == "" && tp == "vpn"){
                        if(arr[i].vpn.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    // C3
                    else if(speed == "" && price >= arr[i].cost && tp == "proxy"){
                        if(arr[i].proxy.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    // ABC1
                    else if(speed <= arr[i].downloadSpeed && price >= arr[i].cost && tp == "all"){
                        listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                    }
                    // ABC2
                    else if(speed <= arr[i].downloadSpeed && price >= arr[i].cost && tp == "vpn"){
                        if(arr[i].vpn.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    // ABC3
                    else if(speed <= arr[i].downloadSpeed && price >= arr[i].cost && tp == "proxy"){
                        if(arr[i].proxy.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    // BC2
                    else if(speed == "" && price >= arr[i].cost && tp == "vpn"){
                        if(arr[i].vpn.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    // BC3
                    else if(speed == "" && price >= arr[i].cost && tp == "proxy"){
                        if(arr[i].proxy.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    // AC2
                    else if(speed <= arr[i].downloadSpeed && price == "" && tp == "vpn"){
                        if(arr[i].vpn.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    // AC3
                    else if(speed <= arr[i].downloadSpeed && price == "" && tp == "proxy"){
                        if(arr[i].proxy.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-side: 16px; font-weight: bold;'> " + arr[i].providerName + "</div><br /><br />" + arr[i].name +"<br /> "+ type +"<div style='font-weight: bold;'><br /> "+ formatBytes(arr[i].downloadSpeed) +" </div>- "+ arr[i].cost + "ITNS", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }

                }
            }
        }

        xmlhttp.open("GET", url, true);
        xmlhttp.send();
    }

    QtObject {
        id: d
        property bool initialized: false
    }

    color: "#F0EEEE"



    Label {
          visible: !isMobile
          id: typeText
          anchors.left: parent.left
          anchors.top:  parent.top
          anchors.leftMargin: 17
          anchors.topMargin: 17
          width: 156
          text: qsTr("Type") + translationManager.emptyString
          fontSize: 14
      }

      ListModel {
          id: typeTransaction
          ListElement { column1: "ALL"; column2: ""; value: "all" }
          ListElement { column1: "VPN"; column2: ""; value: "vpn" }
          ListElement { column1: "PROXY"; column2: ""; value: "proxy" }

      }

      StandardDropdown {
          visible: !isMobile
          id: typeDrop
          anchors.left: parent.left
          anchors.top: typeText.bottom
          anchors.leftMargin: 17
          anchors.topMargin: 5
          width: 156
          shadowReleasedColor: "#4D0051"
          shadowPressedColor: "#2D002F"
          releasedColor: "#6B0072"
          pressedColor: "#4D0051"
          dataModel: typeTransaction
          z: 100
      }

      Label {
          visible: !isMobile
          id: maxPriceText
          anchors.left: typeText.right
          anchors.top:  parent.top
          anchors.leftMargin: 17
          anchors.topMargin: 17
          width: 156
          text: qsTr("Max Price") + translationManager.emptyString
          fontSize: 14
      }

      LineEdit {
          visible: !isMobile
          id: maxPriceLine
          anchors.left: typeDrop.right
          anchors.top: maxPriceText.bottom
          anchors.leftMargin: 17
          anchors.topMargin: 5
          width: 156
      }

      Label {
          visible: !isMobile
          id: minSpeedText
          anchors.left: maxPriceText.right
          anchors.top: parent.top
          anchors.leftMargin: 17
          anchors.topMargin: 17
          width: 156
          text: qsTr("Min Speed") + translationManager.emptyString
          fontSize: 14
      }

      LineEdit {
          visible: !isMobile
          id: minSpeedLine
          anchors.left: maxPriceLine.right
          anchors.top: minSpeedText.bottom
          anchors.leftMargin: 17
          anchors.topMargin: 5
          width: 156

      }

      CheckBox {
          visible: !isMobile
          id: favoriteFilter
          text: qsTr("Favorite") + translationManager.emptyString
          anchors.left: minSpeedLine.right
          anchors.top: parent.top
          anchors.leftMargin: 17
          anchors.topMargin: 46
          checkedIcon: "../images/star.png"
          uncheckedIcon: "../images/unstar.png"
          onClicked: {
          }
      }

      StandardButton {
          visible: !isMobile
          id: filterButton
          anchors.top: parent.top
          anchors.left: favoriteFilter.right
          anchors.leftMargin: 17
          anchors.topMargin: 40
          width: 60
          text: qsTr("Filter") + translationManager.emptyString
          shadowReleasedColor: "#4D0051"
          shadowPressedColor: "#2D002F"
          releasedColor: "#6B0072"
          pressedColor: "#4D0051"
          onClicked:  {
              getJson(minSpeedLine.text, maxPriceLine.text, typeTransaction.get(typeDrop.currentIndex).value)
          }
      }

    Rectangle {
        //id: tableRect
        property int expandedHeight: parent.height - parent.y - parent.height - 5
        property int middleHeight: parent.height - maxPriceLine.y - maxPriceLine.height - 17
        property int collapsedHeight: parent.height - typeDrop.y - typeDrop.height - 17
        //signal jsonService(variant item)

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

        StandardDialog {
            id: detailsPopup
            cancelVisible: false
            okVisible: true
            width:900
            height: 600
        }



        ListView {
                id: listView
                anchors.fill: parent
                model: listModel

                delegate: Rectangle {
                    width: listView.width
                    height: listView.height / 6.8
                    color: getBackgroundColor(index)

                    Text {
                        text: listdata
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.leftMargin: 70
                        anchors.topMargin: 15


                        CheckBox {
                            visible: !isMobile
                            id: favoriteCheck
                            //text: qsTr("Favorite") + translationManager.emptyString
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.leftMargin: -50
                            anchors.topMargin: 5
                            checkedIcon: "../images/star.png"
                            uncheckedIcon: "../images/unstar.png"
                            onClicked: {
                            }
                        }

                        StandardDialog {
                            id: connectPopup
                            cancelVisible: true
                            okVisible: true
                            width:400
                            height: 380
                            onAccepted:{

                                intenseDashView.providerName = obj.providerName
                                intenseDashView.name = obj.name
                                intenseDashView.type = obj.type
                                intenseDashView.cost = obj.cost
                                intenseDashView.firstPrePaidMinutes = obj.firstPrePaidMinutes
                                middlePanel.state = "ITNS Dashboard"
                            }
                        }

                        StandardButton {
                            visible: !isMobile
                            id: rankButton
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: -50
                            anchors.topMargin: 40
                            width: 25
                            height: 25
                            text: rank
                            shadowReleasedColor: getColor(rank)
                            shadowPressedColor: getColor(rank)
                            releasedColor: getColor(rank)
                            pressedColor: getColor(rank)
                        }

                        StandardButton {
                            visible: !isMobile
                            id: subButton
                            anchors.top: parent.top
                            anchors.right: parent.right
                            anchors.rightMargin: 17
                            anchors.topMargin: 0
                            width: 80
                            text: qsTr("Connect") + translationManager.emptyString
                            shadowReleasedColor: "#983CFF"
                            shadowPressedColor: "#B32D00"
                            releasedColor: "#813CFF"
                            pressedColor: "#983CFF"



                            onClicked:{
                                connectPopup.title = "Connection Confirmation";
                                connectPopup.content = buildTxConnectionString(obj);
                                connectPopup.open();

                            }
                        }



                        StandardButton {
                            visible: !isMobile
                            id: infoButton
                            anchors.top: subButton.bottom
                            anchors.right: parent.right
                            anchors.rightMargin: 17
                            anchors.topMargin: 2
                            width: 80
                            text: qsTr("Details") + translationManager.emptyString
                            shadowReleasedColor: "#983CFF"
                            shadowPressedColor: "#B32D00"
                            releasedColor: "#813CFF"
                            pressedColor: "#983CFF"
                            onClicked:  {    
                                detailsPopup.title = "Services details";
                                detailsPopup.content = buildTxDetailsString(obj,rank);
                                detailsPopup.open();
                            }
                        }
                    }
                }
            }

            ListModel {
                id: listModel

                Component.onCompleted: {

                    getJson()

                }

            }

    }

    function onPageCompleted() {

        //table.addressBookModel = appWindow.currentWallet ? appWindow.currentWallet.addressBookModel : null
    }
}


/*

        StandardButton {
            id: getAll
            width: 80
            shadowReleasedColor: "#983CFF"
            shadowPressedColor: "#B32D00"
            releasedColor: "#813CFF"
            pressedColor: "#983CFF"
            text: qsTr("Refresh");
            onClicked: {
                var xmlhttp = new XMLHttpRequest();
                var url = "https://jhx4eq5ijc.execute-api.us-east-1.amazonaws.com/dev/v1/services/search";

                xmlhttp.onreadystatechange=function() {
                    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                        var obj = xmlhttp.responseText
                        console.log("meu log: " + obj)
                    }
                }
                xmlhttp.open("GET", url, true);
                xmlhttp.send();
            }
        }

        */
