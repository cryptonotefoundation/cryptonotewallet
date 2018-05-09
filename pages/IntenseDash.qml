import QtQuick 2.0

//import moneroComponents.Wallet 1.0
//import moneroComponents.WalletManager 1.0
//import moneroComponents.TransactionHistory 1.0
import moneroComponents.TransactionInfo 1.0
//import moneroComponents.TransactionHistoryModel 1.0

import "../components"

Rectangle {
    id: root
    property var model

    function buildTxDetailsString(data, rank, type) {
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

    function changeStatus(bt){
        console.log(bt + "my buton path -------------------------------")
        if (bt == "qrc:///images/pon.png"){
            pon.source = "../images/poff.png"
            shield.source = "../images/vgshield.png"
            runningText.text = "Connected"
            startText.text = "Click to Stop"
        }else{
            pon.source = "../images/pon.png"
            shield.source = "../images/shield.png"
            runningText.text = "Not running"
            startText.text = "Click to Start"
        }

    }

    QtObject {
        id: d
        property bool initialized: false
    }

    color: "#F0EEEE"


    Rectangle {
        anchors.left: parent.left
        //anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 27
        anchors.topMargin: 27
        height: 200
        width: 170
        color: "#ffffff"

        Label {
              visible: !isMobile
              id: typeText
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top:  parent.top
              anchors.topMargin: 27
              //width: 156
              text: qsTr("Status") + translationManager.emptyString
              fontSize: 20
          }

          Image {
              id: shield
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top:  typeText.top
              anchors.topMargin: 37
              width: 80; height: 80
              fillMode: Image.PreserveAspectFit
              source: "../images/shield.png"
          }
    /*
          Image {
              id: wgshield
              anchors.left: typeText.left
              anchors.top:  typeText.top
              anchors.leftMargin: -15
              anchors.topMargin: 37
              width: 80; height: 80
              fillMode: Image.PreserveAspectFit
              source: "../images/wgshield.png"
          }
    */

          Label {
                visible: !isMobile
                id: runningText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top:  shield.top
                anchors.topMargin: 100
                //width: 156
                text: qsTr("Not running") + translationManager.emptyString
                fontSize: 20
          }

      }

    Rectangle {
        anchors.left: parent.left
        //anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 207
        anchors.topMargin: 27
        height: 200
        width: 190
        color: "#ffffff"

          Label {
                visible: !isMobile
                id: startText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top:  parent.top
                anchors.topMargin: 27
                //width: 156
                text: qsTr("Click to Start") + translationManager.emptyString
                fontSize: 20
            }

              Image {
                  id: pon
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.top:  startText.top
                  anchors.topMargin: 47
                  width: 70; height: 70
                  fillMode: Image.PreserveAspectFit
                  source: "../images/pon.png"
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          changeStatus(pon.source)
                      }
                  }
              }
        }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 407
        anchors.topMargin: 27
        anchors.rightMargin: 27
        height: 200
        //width: 280
        color: "#ffffff"

        StandardButton {
            visible: !isMobile
            id: learnButton
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 27
            width: 260
            text: qsTr("Learn how to use the VPN services") + translationManager.emptyString
            shadowReleasedColor: "#983CFF"
            shadowPressedColor: "#B32D00"
            releasedColor: "#813CFF"
            pressedColor: "#983CFF"
            onClicked:  {

            }
        }

        StandardButton {
            visible: !isMobile
            id: searchButton
            anchors.top: learnButton.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 47
            width: 260
            text: qsTr("Search for provider") + translationManager.emptyString
            shadowReleasedColor: "#983CFF"
            shadowPressedColor: "#B32D00"
            releasedColor: "#813CFF"
            pressedColor: "#983CFF"
            onClicked:  {

            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 27
        anchors.rightMargin: 27
        anchors.bottomMargin: 27
        anchors.topMargin: 234
        height: 200
        //width: 280
        color: "#ffffff"

        Label {
              visible: !isMobile
              id: detailsText
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top:  parent.top
              anchors.topMargin: 27
              //width: 156
              text: qsTr("Details") + translationManager.emptyString
              fontSize: 20
              //fontWeight: bold
          }

        Label {
              visible: !isMobile
              id: timeonlineText
              anchors.left: parent.left
              anchors.top:  detailsText.top
              anchors.topMargin: 47
              anchors.leftMargin: 27
              width: 200
              //anchors.left: Text.AlignRight
              text: qsTr("Time online:") + translationManager.emptyString
              fontSize: 16
          }
        Label {
              visible: !isMobile
              id: transferredText
              anchors.left: parent.left
              anchors.top:  timeonlineText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 200
              text: qsTr("Transferred:") + translationManager.emptyString
              fontSize: 16
          }

        Label {
              visible: !isMobile
              id: paiduntilnowText
              anchors.left: parent.left
              anchors.top:  transferredText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 200
              text: qsTr("Paid until now:") + translationManager.emptyString
              fontSize: 16
          }

        Label {
              visible: !isMobile
              id: providerText
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top:  paiduntilnowText.top
              anchors.topMargin: 37
              //width: 156
              text: qsTr("Provider") + translationManager.emptyString
              fontSize: 20
          }
        Label {
              visible: !isMobile
              id: nameText
              anchors.left: parent.left
              anchors.top:  providerText.top
              anchors.topMargin: 47
              anchors.leftMargin: 27
              width: 200
              text: qsTr("Name:") + translationManager.emptyString
              fontSize: 16
          }
        Label {
              visible: !isMobile
              id: planText
              anchors.left: parent.left
              anchors.top:  nameText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 200
              text: qsTr("Plan:") + translationManager.emptyString
              fontSize: 16
          }
        Label {
              visible: !isMobile
              id: costText
              anchors.left: parent.left
              anchors.top:  planText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 200
              text: qsTr("Cost:") + translationManager.emptyString
              fontSize: 16
          }
        Label {
              visible: !isMobile
              id: servercountryText
              anchors.left: parent.left
              anchors.top:  costText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 200
              text: qsTr("Server coutry:") + translationManager.emptyString
              fontSize: 16
          }
        Label {
              visible: !isMobile
              id: serveripText
              anchors.left: parent.left
              anchors.top:  servercountryText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 200
              text: qsTr("Server IP:") + translationManager.emptyString
              fontSize: 16
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
