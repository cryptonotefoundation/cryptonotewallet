import QtQuick 2.0
import moneroComponents.TransactionInfo 1.0


import "../components"

Rectangle {
    id: root
    property var model
    property string providerName
    property string name
    property string type
    property string cost
    property string firstPrePaidMinutes
    property string feedback
    property string bton

    function createJsonFeedback(fbId){
        var url = "https://jhx4eq5ijc.execute-api.us-east-1.amazonaws.com/dev/v1/feedback/add"
        var xmlhttpPost = new XMLHttpRequest();
        xmlhttpPost.onreadystatechange=function() {
            if (xmlhttpPost.readyState == 4 && xmlhttpPost.status == 200) {
                var feed = JSON.parse(xmlhttpPost.responseText)
            }
        }
        var data = {"id":fbId, "speed":1, "stability":4}
        data = JSON.stringify(data)
        xmlhttpPost.open("POST", url, true);
        xmlhttpPost.setRequestHeader("Content-type", "application/json");
        xmlhttpPost.send(data);

    }

    function changeStatus(bt){
        if (bt == "qrc:///images/poff.png"){
            pon.source = "../images/pon.png"
            if(type == "openvpn"){
                shield.source = "../images/vgshield.png"
            }else{
                shield.source = "../images/wgshield.png"
            }
            runningText.text = "Connected"
            startText.text = "Click to Stop"

        }else{
            pon.source = "../images/poff.png"
            shield.source = "../images/shield.png"
            runningText.text = "Not running"
            startText.text = "Click to Start"
            bton = ""
            createJsonFeedback(feedback)
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
              source: if(type == "openvpn"){"../images/vgshield.png"}else if(type == "proxy"){"../images/wgshield.png"}else{"../images/shield.png"}
          }

          Label {
                visible: !isMobile
                id: runningText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top:  shield.top
                anchors.topMargin: 100
                //width: 156
                text: if(feedback.length != 36){qsTr("Not running")+ translationManager.emptyString}else{ qsTr("Connected")+ translationManager.emptyString}
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
                text: if(feedback.length != 36){qsTr("Click to Start")+ translationManager.emptyString}else{qsTr("Click to Stop")+ translationManager.emptyString}
                fontSize: 20
            }

              Image {
                  id: pon
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.top:  startText.top
                  anchors.topMargin: 47
                  width: 70; height: 70
                  fillMode: Image.PreserveAspectFit
                  source: if(feedback.length != 36){"../images/poff.png"}else{"../images/pon.png"}
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
        id: providerTable
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
              id: providerNameText
              anchors.left: parent.left
              anchors.top:  providerText.top
              anchors.topMargin: 47
              anchors.leftMargin: 147
              width: 200
              text: qsTr(providerName) + translationManager.emptyString
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
              id: nameIntenseText
              anchors.left: parent.left
              anchors.top:  nameText.top
              anchors.topMargin: 27
              anchors.leftMargin: 147
              width: 200
              text: qsTr(name) + translationManager.emptyString
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
              id: costIntenseText
              anchors.left: parent.left
              anchors.top:  planText.top
              anchors.topMargin: 27
              anchors.leftMargin: 147
              width: 200
              text: qsTr(cost) + translationManager.emptyString
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

    //onJsonService:console.debug(item + "------------------------------------")


    function onPageCompleted() {
        if(bton == "qrc:///images/poff.png"){
            changeStatus("qrc:///images/poff.png")
        }
    }
}

