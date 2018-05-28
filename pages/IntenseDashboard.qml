import QtQuick 2.0
import moneroComponents.TransactionInfo 1.0
import QtQml 2.2
import moneroComponents.Wallet 1.0
import moneroComponents.WalletManager 1.0

import "../components"
import "../IntenseConfig.js" as Config

Rectangle {
    id: root
    property var model
    property string idService
    property string providerName
    property string name
    property string type
    property int cost
    property int firstPrePaidMinutes
    property int subsequentPrePaidMinutes
    property string speed
    property string feedback
    property string bton
    property string rank

    function getTime(){
        var value =  (firstPrePaidMinutes*10000) - Config.payTimer
        return value
    }

    function setPayment(){
        var url = Config.jsonRpcURL
        var value = cost*firstPrePaidMinutes
        var xmlhttpPost = new XMLHttpRequest();
        xmlhttpPost.onreadystatechange=function() {
            if (xmlhttpPost.readyState == 4 && xmlhttpPost.status == 200) {
                var payConfirm = JSON.parse(xmlhttpPost.responseText)
            }
        }
        var data = {"jsonrpc":"2.0",
                    "id":"0",
                    "method":"transfer",
                    "params":{
                        "destinations":[{"amount":value,
                        "address":Config.wallet}],
                        "mixin":4,
                        "get_tx_key": true
                     }
                    }
        data = JSON.stringify(data)
        xmlhttpPost.open("POST", url, true);
        xmlhttpPost.setRequestHeader("Content-type", "application/json");
        xmlhttpPost.send(data);
    }

    function postJsonFeedback(fbId){
        var url = Config.url+Config.stage+Config.version+Config.feedback+Config.add
        var xmlhttpPost = new XMLHttpRequest();
        xmlhttpPost.onreadystatechange=function() {
            if (xmlhttpPost.readyState == 4 && xmlhttpPost.status == 200) {
                var feed = JSON.parse(xmlhttpPost.responseText)
            }
        }

        var sp = 0
        var st = 0
        var i = 0
        var arrRank = [rank1, rank2, rank3, rank4, rank5]
        var arrRankText = [rText1, rText2, rText3, rText4, rText5]
        var arrQRank = [rankQ1, rankQ2, rankQ3, rankQ4, rankQ5]
        var arrQRankText = [rqText1, rqText2, rqText3, rqText4, rqText5]
        for(i = 0; i < 5; i++){
            if(arrRank[i].color == '#4d0051'){
                sp = parseInt(arrRankText[i].text)
                console.log(sp + "-------------------")
            }
            if(arrQRank[i].color == '#4d0051'){
                st = parseInt(arrQRankText[i].text)
                console.log(st + "-------------------")
            }
        }

        var data = {"id":fbId, "speed":sp, "stability":st}
        data = JSON.stringify(data)
        xmlhttpPost.open("POST", url, true);
        xmlhttpPost.setRequestHeader("Content-type", "application/json");
        xmlhttpPost.send(data);

    }

    function getMyFeedJson(){
        var url = Config.url+Config.stage+Config.version+Config.feedback+Config.get+"/"+appWindow.currentWallet.address+"/"+idService
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange=function() {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                var mFeed = JSON.parse(xmlhttp.responseText)
                if(mFeed[0].mStability == null){
                    mFeed[0].mStability = 0
                }
                if(mFeed[0].mSpeed == null){
                    mFeed[0].mSpeed = 0
                }
                var myRank = (mFeed[0].mStability + mFeed[0].mSpeed)/2
                myRank = parseFloat(myRank).toFixed(1)
                myRankText.text =  myRank
            }
        }

        xmlhttp.open("GET", url, true);
        xmlhttp.send();
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
            subButtonText.text = "Disconnect"

        }else{
            pon.source = "../images/poff.png"
            shield.source = "../images/shield.png"
            runningText.text = "Not running"
            subButtonText.text = "Connect"
            bton = ""
        }

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
        height: 160
        width: 250
        color: "#ffffff"


        Text {
              visible: !isMobile
              id: typeText
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top:  parent.top
              anchors.topMargin: 14
              //width: 156
              text: qsTr("Status") + translationManager.emptyString
              font.pixelSize: 20
              color: "#6b0072"
              font.bold: true
              font.family: "Arial"
          }

          Image {
              id: shield
              anchors.left: parent.left
              anchors.top:  typeText.top
              anchors.topMargin: 37
              anchors.leftMargin: 17
              width: 72; height: 87
              fillMode: Image.PreserveAspectFit
              source: if(type == "openvpn"){"../images/vgshield.png"}else if(type == "proxy"){"../images/wgshield.png"}else{"../images/shield.png"}
          }

          Text {
                visible: !isMobile
                id: runningText
                anchors.left: shield.left
                anchors.top:  typeText.top
                anchors.topMargin: 65
                anchors.leftMargin: 90
                //width: 156
                text: if(feedback.length != 36){qsTr("Not running")+ translationManager.emptyString}else{ qsTr("Connected")+ translationManager.emptyString}
                font.pixelSize: 19
                font.bold: true
                color: "#535353"
                font.family: "Arial"
          }

      }

    Rectangle {
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.rightMargin: 27
        anchors.leftMargin: 284
        anchors.topMargin: 27
        height: 160
        //width: 190
        color: "#ffffff"

          Text {
                visible: !isMobile
                id: startText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top:  parent.top
                anchors.topMargin: 14
                //width: 156
                text: qsTr("Reconnect")+ translationManager.emptyString
                font.pixelSize: 20
                color: "#6b0072"
                font.bold: true
            }

          Text {
                visible: !isMobile
                id: historicalConnectionLabel
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top:  startText.top
                anchors.topMargin: 65
                text: qsTr("No Historical Connection:") + translationManager.emptyString
                font.pixelSize: 19
                color: "#535353"
                font.family: "Arial"
                font.bold: true
            }

          // Just to show the simple Dashboard !! Dont remove
          Text {
                visible: !isMobile
                id: lastRankLabel
                anchors.right: parent.right
                anchors.top:  startText.top
                anchors.topMargin: 31
                anchors.rightMargin: 57
                width: 60
                text: qsTr("Feedback:") + translationManager.emptyString
                font.pixelSize: 12
                horizontalAlignment: Text.AlignRight
                color: "#535353"
                font.family: "Arial"
            }



          Rectangle {
              visible: !isMobile
              id: rankRectangle
              anchors.top: startText.top
              anchors.right: parent.right
              anchors.rightMargin: 17
              anchors.topMargin: 24
              width: 35
              height: 25
              color: getColor(rank)
              radius: 4

              Text {
                  text: rank
                  font.pixelSize: 13
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.verticalCenter: parent.verticalCenter
                  color: "#ffffff"
                  font.family: "Arial"
                  font.bold: true
              }
          }


          Text {
                visible: !isMobile
                id: lastMyRankLabel
                anchors.right: parent.right
                anchors.top:  lastRankLabel.top
                anchors.topMargin: 31
                anchors.rightMargin: 57
                width: 60
                text: qsTr("My Feedback:") + translationManager.emptyString
                font.pixelSize: 12
                horizontalAlignment: Text.AlignRight
                color: "#535353"
                font.family: "Arial"
            }

          Rectangle {
              visible: !isMobile
              id: myRankRectangle
              anchors.top: rankRectangle.top
              anchors.right: parent.right
              anchors.rightMargin: 17
              anchors.topMargin: 31
              width: 35
              height: 25
              color: getColor(myRank)
              radius: 4

              Text {
                  id: myRankText
                  font.pixelSize: 13
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.verticalCenter: parent.verticalCenter
                  color: "#ffffff"
                  font.family: "Arial"
                  font.bold: true
              }
          }


          Text {
                visible: !isMobile
                id: lastTypeLabel
                anchors.left: parent.left
                anchors.top:  startText.top
                anchors.topMargin: 31
                anchors.leftMargin: 17
                width: 60
                text: qsTr("Type:") + translationManager.emptyString
                font.pixelSize: 12
                horizontalAlignment: Text.AlignRight
                color: "#535353"
                font.family: "Arial"
            }

          Text {
                visible: !isMobile
                id: lastTypeText
                anchors.left: parent.left
                anchors.top:  startText.top
                anchors.topMargin: 31
                anchors.leftMargin: 90
                width: 70
                text: if(type == "openvpn"){qsTr("VPN") + translationManager.emptyString}else if(type == "proxy"){qsTr("PROXY") + translationManager.emptyString}
                font.pixelSize: 13
                horizontalAlignment: Text.AlignLeft
                font.bold: true
                color: "#535353"
                font.family: "Arial"
            }

          Text {
                visible: !isMobile
                id: lastProviderNameLabel
                anchors.left: parent.left
                anchors.top:  lastTypeLabel.top
                anchors.topMargin: 21
                anchors.leftMargin: 17
                width: 60
                text: qsTr("Provider:") + translationManager.emptyString
                font.pixelSize: 12
                horizontalAlignment: Text.AlignRight
                color: "#535353"
                font.family: "Arial"
            }

          Text {
                visible: !isMobile
                id: lastProviderNameText
                anchors.left: parent.left
                anchors.top:  lastTypeText.top
                anchors.topMargin: 21
                anchors.leftMargin: 90
                width: 70
                text: qsTr(providerName) + translationManager.emptyString
                font.pixelSize: 12
                horizontalAlignment: Text.AlignLeft
                color: "#535353"
                font.family: "Arial"
            }



          Text {
                visible: !isMobile
                id: lastPlanLabel
                anchors.left: parent.left
                anchors.top:  lastProviderNameLabel.top
                anchors.topMargin: 21
                anchors.leftMargin: 17
                width: 60
                text: qsTr("Plan:") + translationManager.emptyString
                font.pixelSize: 12
                horizontalAlignment: Text.AlignRight
                color: "#535353"
                font.family: "Arial"
            }
          Text {
                visible: !isMobile
                id: lastNameIntenseText
                anchors.left: parent.left
                anchors.top:  lastProviderNameText.top
                anchors.topMargin: 21
                anchors.leftMargin: 90
                width: 70
                text: qsTr(name) + translationManager.emptyString
                font.pixelSize: 12
                horizontalAlignment: Text.AlignLeft
                color: "#535353"
                font.family: "Arial"
            }


          Text {
                visible: !isMobile
                id: lastCostText
                anchors.left: parent.left
                anchors.top:  lastPlanLabel.top
                anchors.topMargin: 21
                anchors.leftMargin: 17
                width: 60
                text: qsTr("Price:") + translationManager.emptyString
                font.pixelSize: 12
                horizontalAlignment: Text.AlignRight
                color: "#535353"
                font.family: "Arial"
            }
          Text {
                visible: !isMobile
                id: lastCostIntenseText
                anchors.left: parent.left
                anchors.top:  lastNameIntenseText.top
                anchors.topMargin: 21
                anchors.leftMargin: 90
                width: 70
                text: cost
                font.pixelSize: 12
                horizontalAlignment: Text.AlignLeft
                color: "#535353"
                font.family: "Arial"
            }

          Text {
                visible: !isMobile
                id: lastSpeedLabel
                anchors.left: parent.left
                anchors.top:  lastCostText.top
                anchors.topMargin: 21
                anchors.leftMargin: 17
                width: 60
                text: qsTr("Speed:") + translationManager.emptyString
                font.pixelSize: 12
                horizontalAlignment: Text.AlignRight
                color: "#535353"
                font.family: "Arial"
            }

          Text {
                visible: !isMobile
                id: lastSpeedText
                anchors.left: parent.left
                anchors.top:  lastCostIntenseText.top
                anchors.topMargin: 21
                anchors.leftMargin: 90
                width: 70
                text: qsTr(speed) + translationManager.emptyString
                font.pixelSize: 12
                horizontalAlignment: Text.AlignLeft
                color: "#535353"
                font.family: "Arial"
            }

          StandardDialog {
              id: connectPopup
              cancelVisible: true
              okVisible: true
              width:400
              height: 420
              onAccepted:{
                  postJsonFeedback(feedback)
              }

              Text {
                    visible: !isMobile
                    id: providerFeedback
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top:  parent.top
                    anchors.topMargin: 100
                    //width: 156
                    text: qsTr(providerName) + translationManager.emptyString
                    font.pixelSize: 18
                    font.bold: true
                    color: "#6b0072"
                    //fontWeight: bold
                }

              Text {
                    visible: !isMobile
                    id: nameFeedback
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top:  providerFeedback.top
                    anchors.topMargin: 37
                    //width: 156
                    text: qsTr(name) + translationManager.emptyString
                    font.pixelSize: 16
                    font.bold: true
                    color: "#6b0072"
                    //fontWeight: bold
                }

              Text {
                    visible: !isMobile
                    id: speedFeedback
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top:  nameFeedback.top
                    anchors.topMargin: 47
                    //width: 156
                    text: qsTr("Speed") + translationManager.emptyString
                    font.pixelSize: 14
                    font.bold: false
                    color: "#000000"
                    //fontWeight: bold
                }

              Rectangle {
                  visible: !isMobile
                  id: rank1
                  anchors.top: speedFeedback.top
                  anchors.right: rank2.right
                  anchors.rightMargin: 47
                  anchors.topMargin: 27
                  width: 35
                  height: 25
                  color: "#c4c4c4"
                  radius: 4
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          parent.color = '#4D0051'
                          rank2.color = "#c4c4c4"
                          rank3.color = "#c4c4c4"
                          rank4.color = "#c4c4c4"
                          rank5.color = "#c4c4c4"
                      }
                  }

                  Text {
                      id: rText1
                      text: "1"
                      font.pixelSize: 13
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#ffffff"
                      font.family: "Arial"
                      font.bold: true
                  }
              }

              Rectangle {
                  visible: !isMobile
                  id: rank2
                  anchors.top: speedFeedback.top
                  anchors.right: rank3.right
                  anchors.rightMargin: 47
                  anchors.topMargin: 27
                  width: 35
                  height: 25
                  color: "#c4c4c4"
                  radius: 4
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          parent.color = '#4D0051'
                          rank1.color = "#c4c4c4"
                          rank3.color = "#c4c4c4"
                          rank4.color = "#c4c4c4"
                          rank5.color = "#c4c4c4"
                      }
                  }

                  Text {
                      id: rText2
                      text: "2"
                      font.pixelSize: 13
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#ffffff"
                      font.family: "Arial"
                      font.bold: true
                  }
              }

              Rectangle {
                  visible: !isMobile
                  id: rank3
                  anchors.top: speedFeedback.top
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.topMargin: 27
                  width: 35
                  height: 25
                  color: "#c4c4c4"
                  radius: 4
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          parent.color = '#4D0051'
                          rank2.color = "#c4c4c4"
                          rank1.color = "#c4c4c4"
                          rank4.color = "#c4c4c4"
                          rank5.color = "#c4c4c4"
                      }
                  }

                  Text {
                      id: rText3
                      text: "3"
                      font.pixelSize: 13
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#ffffff"
                      font.family: "Arial"
                      font.bold: true
                  }
              }

              Rectangle {
                  visible: !isMobile
                  id: rank4
                  anchors.top: speedFeedback.top
                  anchors.left: rank3.left
                  anchors.topMargin: 27
                  anchors.leftMargin: 47
                  width: 35
                  height: 25
                  color: "#c4c4c4"
                  radius: 4
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          parent.color = '#4D0051'
                          rank2.color = "#c4c4c4"
                          rank3.color = "#c4c4c4"
                          rank1.color = "#c4c4c4"
                          rank5.color = "#c4c4c4"
                      }
                  }

                  Text {
                      id: rText4
                      text: "4"
                      font.pixelSize: 13
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#ffffff"
                      font.family: "Arial"
                      font.bold: true
                  }
              }

              Rectangle {
                  visible: !isMobile
                  id: rank5
                  anchors.top: speedFeedback.top
                  anchors.left: rank4.left
                  anchors.topMargin: 27
                  anchors.leftMargin: 47
                  width: 35
                  height: 25
                  color: "#c4c4c4"
                  radius: 4
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          parent.color = '#4D0051'
                          rank2.color = "#c4c4c4"
                          rank3.color = "#c4c4c4"
                          rank4.color = "#c4c4c4"
                          rank1.color = "#c4c4c4"
                      }
                  }

                  Text {
                      id: rText5
                      text: "5"
                      font.pixelSize: 13
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#ffffff"
                      font.family: "Arial"
                      font.bold: true
                  }
              }

              Text {
                    visible: !isMobile
                    id: qualityFeedback
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top:  rank3.top
                    anchors.topMargin: 47
                    //width: 156
                    text: qsTr("Quality") + translationManager.emptyString
                    font.pixelSize: 14
                    font.bold: false
                    color: "#000000"
                    //fontWeight: bold
                }

              Rectangle {
                  visible: !isMobile
                  id: rankQ1
                  anchors.top: qualityFeedback.top
                  anchors.right: rankQ2.right
                  anchors.rightMargin: 47
                  anchors.topMargin: 27
                  width: 35
                  height: 25
                  color: "#c4c4c4"
                  radius: 4
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          parent.color = '#4D0051'
                          rankQ2.color = "#c4c4c4"
                          rankQ3.color = "#c4c4c4"
                          rankQ4.color = "#c4c4c4"
                          rankQ5.color = "#c4c4c4"
                      }
                  }

                  Text {
                      id: rqText1
                      text: "1"
                      font.pixelSize: 13
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#ffffff"
                      font.family: "Arial"
                      font.bold: true
                  }
              }

              Rectangle {
                  visible: !isMobile
                  id: rankQ2
                  anchors.top: qualityFeedback.top
                  anchors.right: rankQ3.right
                  anchors.rightMargin: 47
                  anchors.topMargin: 27
                  width: 35
                  height: 25
                  color: "#c4c4c4"
                  radius: 4
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          parent.color = '#4D0051'
                          rankQ1.color = "#c4c4c4"
                          rankQ3.color = "#c4c4c4"
                          rankQ4.color = "#c4c4c4"
                          rankQ5.color = "#c4c4c4"
                      }
                  }

                  Text {
                      id: rqText2
                      text: "2"
                      font.pixelSize: 13
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#ffffff"
                      font.family: "Arial"
                      font.bold: true
                  }
              }

              Rectangle {
                  visible: !isMobile
                  id: rankQ3
                  anchors.top: qualityFeedback.top
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.topMargin: 27
                  width: 35
                  height: 25
                  color: "#c4c4c4"
                  radius: 4
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          parent.color = '#4D0051'
                          rankQ2.color = "#c4c4c4"
                          rankQ1.color = "#c4c4c4"
                          rankQ4.color = "#c4c4c4"
                          rankQ5.color = "#c4c4c4"
                      }
                  }

                  Text {
                      id: rqText3
                      text: "3"
                      font.pixelSize: 13
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#ffffff"
                      font.family: "Arial"
                      font.bold: true
                  }
              }

              Rectangle {
                  visible: !isMobile
                  id: rankQ4
                  anchors.top: qualityFeedback.top
                  anchors.left: rankQ3.left
                  anchors.topMargin: 27
                  anchors.leftMargin: 47
                  width: 35
                  height: 25
                  color: "#c4c4c4"
                  radius: 4
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          parent.color = '#4D0051'
                          rankQ2.color = "#c4c4c4"
                          rankQ3.color = "#c4c4c4"
                          rankQ1.color = "#c4c4c4"
                          rankQ5.color = "#c4c4c4"
                      }
                  }

                  Text {
                      id: rqText4
                      text: "4"
                      font.pixelSize: 13
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#ffffff"
                      font.family: "Arial"
                      font.bold: true
                  }
              }

              Rectangle {
                  visible: !isMobile
                  id: rankQ5
                  anchors.top: qualityFeedback.top
                  anchors.left: rankQ4.left
                  anchors.topMargin: 27
                  anchors.leftMargin: 47
                  width: 35
                  height: 25
                  color: "#c4c4c4"
                  radius: 4
                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          parent.color = '#4D0051'
                          rankQ2.color = "#c4c4c4"
                          rankQ3.color = "#c4c4c4"
                          rankQ4.color = "#c4c4c4"
                          rankQ1.color = "#c4c4c4"
                      }
                  }

                  Text {
                      id: rqText5
                      text: "5"
                      font.pixelSize: 13
                      anchors.horizontalCenter: parent.horizontalCenter
                      anchors.verticalCenter: parent.verticalCenter
                      color: "#ffffff"
                      font.family: "Arial"
                      font.bold: true
                  }
              }


          }


          StandardButton {
              visible: !isMobile
              id: subButton
              anchors.bottom: parent.bottom
              anchors.right: parent.right
              anchors.rightMargin: 17
              anchors.bottomMargin: 17
              width: 120
              shadowReleasedColor: "#983CFF"
              shadowPressedColor: "#B32D00"
              releasedColor: "#813CFF"
              pressedColor: "#983CFF"
              onClicked:{
                  changeStatus(pon.source)

                  connectPopup.title = "Provider Feedback";
                  connectPopup.open();


              }

              Text {
                  id: subButtonText
                  anchors.verticalCenter: parent.verticalCenter
                  anchors.left: parent.left
                  anchors.leftMargin: 37
                  color: "#ffffff"
                  font.bold: true
                  text: if(feedback.length != 36){qsTr("Connect") + translationManager.emptyString}else{qsTr("Disconnect") + translationManager.emptyString}

              }

              Image {
                  id: pon
                  anchors.left: parent.left
                  anchors.top:  startText.top
                  anchors.verticalCenter: parent.verticalCenter
                  anchors.leftMargin: 10
                  width: 25; height: 25
                  fillMode: Image.PreserveAspectFit
                  source: if(feedback.length != 36){"../images/poff.png"}else{"../images/pon.png"}
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
        anchors.topMargin: 194
        height: 200
        //width: 280
        color: "#ffffff"

        Text {
              //visible: false//!isMobile
              id: howToUseText
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top:  parent.top
              anchors.topMargin: 100
              //width: 156
              text: qsTr("Lean how to use the VPN service") + translationManager.emptyString
              font.pixelSize: 22
              font.bold: true
              color: "#0645AD"
              font.family: "Arial"
              //fontWeight: bold
              MouseArea{
                  anchors.fill: parent
                  onClicked:Qt.openUrlExternally("https://intensecoin.com/");
              }
          }

        Text {
              //visible: !isMobile
              id: orText
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top:  howToUseText.top
              anchors.topMargin: 70
              //width: 156
              text: qsTr("or") + translationManager.emptyString
              font.pixelSize: 18
              font.bold: true
              color: "#535353"
              font.family: "Arial"
              //fontWeight: bold
          }


        Text {
              //visible: !isMobile
              id: searchForProviderText
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top:  orText.top
              anchors.topMargin: 70
              //width: 156
              text: qsTr("Search for provider") + translationManager.emptyString
              font.pixelSize: 22
              font.bold: true
              color: "#0645AD"
              font.family: "Arial"
              //fontWeight: bold
              MouseArea {
                  anchors.fill: parent
                  onClicked: {
                      middlePanel.state = "ITNS Provider"
                  }
              }
          }


        // Just to show de simple Dashboard !! Dont remove

        Text {
              visible: !isMobile
              id: detailsText
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top:  parent.top
              anchors.topMargin: 27
              //width: 156
              text: qsTr("Details") + translationManager.emptyString
              font.pixelSize: 18
              font.bold: true
              color: "#6b0072"
              //fontWeight: bold
          }


        Text {
              visible: !isMobile
              id: timeonlineText
              anchors.left: parent.left
              anchors.top:  detailsText.top
              anchors.topMargin: 47
              anchors.leftMargin: 27
              width: 140
              //anchors.left: Text.AlignRight
              text: qsTr("Time online:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }
        Text {
              visible: !isMobile
              id: transferredText
              anchors.left: parent.left
              anchors.top:  timeonlineText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 140
              text: qsTr("Transferred:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }


        Text {
              visible: !isMobile
              id: paiduntilnowText
              anchors.left: parent.left
              anchors.top:  transferredText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 140
              text: qsTr("Paid until now:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }



        Text {
              visible: !isMobile
              id: providerText
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top:  paiduntilnowText.top
              anchors.topMargin: 37
              //width: 156
              text: qsTr("Provider") + translationManager.emptyString
              font.pixelSize: 18
              color: "#6b0072"
              font.bold: true
          }


        Text {
              visible: !isMobile
              id: nameText
              anchors.left: parent.left
              anchors.top:  providerText.top
              anchors.topMargin: 47
              anchors.leftMargin: 27
              width: 90
              text: qsTr("Name:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }

        Text {
              visible: !isMobile
              id: providerNameText
              anchors.left: parent.left
              anchors.top:  providerText.top
              anchors.topMargin: 47
              anchors.leftMargin: 147
              width: 140
              text: qsTr(providerName) + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }


        Text {
              visible: !isMobile
              id: planText
              anchors.left: parent.left
              anchors.top:  nameText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 90
              text: qsTr("Plan:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }


        Text {
              visible: !isMobile
              id: nameIntenseText
              anchors.left: parent.left
              anchors.top:  nameText.top
              anchors.topMargin: 27
              anchors.leftMargin: 147
              width: 140
              text: qsTr(name) + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }


        Text {
              visible: !isMobile
              id: costText
              anchors.left: parent.left
              anchors.top:  planText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 90
              text: qsTr("Cost:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }


        Text {
              visible: !isMobile
              id: costIntenseText
              anchors.left: parent.left
              anchors.top:  planText.top
              anchors.topMargin: 27
              anchors.leftMargin: 147
              width: 140
              text: cost
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }


        Text {
              visible: !isMobile
              id: servercountryText
              anchors.left: parent.left
              anchors.top:  costText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 90
              text: qsTr("Server coutry:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }


        Text {
              visible: !isMobile
              id: serveripText
              anchors.left: parent.left
              anchors.top:  servercountryText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 90
              text: qsTr("Server IP:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }


    }

    //onJsonService:console.debug(item + "------------------------------------")
    Timer {
        id: timer
        interval: getTime()
        repeat: true
        running: true

        onTriggered:
        {
            console.log(getTime() + '----------------------')
            setPayment(appWindow.currentWallet.address)
        }
    }

    function onPageCompleted() {
        getMyFeedJson()
        if(providerName != ""){
            howToUseText.visible = false
            orText.visible = false
            searchForProviderText.visible = false
            historicalConnectionLabel.visible = false

            detailsText.visible = true
            timeonlineText.visible = true
            transferredText.visible = true
            paiduntilnowText.visible = true
            providerText.visible = true
            nameText.visible = true
            providerNameText.visible = true
            planText.visible = true
            nameIntenseText.visible = true
            costText.visible = true
            costIntenseText.visible = true
            servercountryText.visible = true
            serveripText.visible = true
            lastRankLabel.visible = true
            rankRectangle.visible = true
            lastMyRankLabel.visible = true
            myRankRectangle.visible = true
            lastTypeLabel.visible = true
            lastTypeText.visible = true
            lastProviderNameLabel.visible = true
            lastProviderNameText.visible = true
            lastPlanLabel.visible = true
            lastNameIntenseText.visible = true
            lastCostText.visible = true
            lastCostIntenseText.visible = true
            lastSpeedLabel.visible = true
            lastSpeedText.visible = true
            subButton.visible = true

        }else{
            howToUseText.visible = true
            orText.visible = true
            searchForProviderText.visible = true
            historicalConnectionLabel.visible = true

            detailsText.visible = false
            timeonlineText.visible = false
            transferredText.visible = false
            paiduntilnowText.visible = false
            providerText.visible = false
            nameText.visible = false
            providerNameText.visible = false
            planText.visible = false
            nameIntenseText.visible = false
            costText.visible = false
            costIntenseText.visible = false
            servercountryText.visible = false
            serveripText.visible = false
            lastRankLabel.visible = false
            rankRectangle.visible = false
            lastMyRankLabel.visible = false
            myRankRectangle.visible = false
            lastTypeLabel.visible = false
            lastTypeText.visible = false
            lastProviderNameLabel.visible = false
            lastProviderNameText.visible = false
            lastPlanLabel.visible = false
            lastNameIntenseText.visible = false
            lastCostText.visible = false
            lastCostIntenseText.visible = false
            lastSpeedLabel.visible = false
            lastSpeedText.visible = false
            subButton.visible = false
        }

        if(bton == "qrc:///images/poff.png"){
            changeStatus("qrc:///images/poff.png")
        }
    }
}

