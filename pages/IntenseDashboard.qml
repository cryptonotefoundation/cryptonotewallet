import QtQuick 2.0
import moneroComponents.TransactionInfo 1.0
import QtQuick.Controls 1.4
import QtQml 2.2
import moneroComponents.Wallet 1.0
import moneroComponents.WalletManager 1.0

import "../components"
import "../pages"
import "../IntenseConfig.js" as Config


Rectangle {
    id: root
    signal paymentClicked(string address, string paymentId, string amount, int mixinCount,
                          int priority, string description)


    property var model
    property string idService
    property string providerName
    property string name
    property string type
    property string cost
    property int firstPrePaidMinutes
    property string subsequentPrePaidMinutes
    property string speed
    property string feedback
    property string bton
    property string rank
    property int flag
    property int secs
    property var obj
    property double itnsStart
    property int macHostFlag

    function getITNS(){
        itnsStart = itnsStart + parseFloat(cost)
        paidTextLine.text = itnsStart.toFixed(8) + " ITNS"
    }

    function getTime(){
        var value =  10000//(firstPrePaidMinutes*10000) - Config.payTimer
        return value
    }

    function setPayment(){
        console.log("Transfer: paymentClicked")
        var priority = 2
        var privacy = 4

        currentWallet.createTransactionAsync(obj.providerWallet, "0000000000000000", 0.0000007, privacy,
                      priority)

        //root.paymentClicked(obj.providerWallet, "0000000000000000", "0.0000007", privacy,
        //               priority, "Intense Coin payment")
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
            }
            if(arrQRank[i].color == '#4d0051'){
                st = parseInt(arrQRankText[i].text)
            }
        }

        var data = {"id":fbId, "speed":sp, "stability":st}
        data = JSON.stringify(data)
        xmlhttpPost.open("POST", url, true);
        xmlhttpPost.setRequestHeader("Content-type", "application/json");
        xmlhttpPost.send(data);

    }

    function csvToArray( strData, strDelimiter ){
            strDelimiter = (strDelimiter || ",");
            var objPattern = new RegExp(
                (
                    // Delimiters.
                    "(\\" + strDelimiter + "|\\r?\\n|\\r|^)" +

                    // Quoted fields.
                    "(?:\"([^\"]*(?:\"\"[^\"]*)*)\"|" +

                    // Standard fields.
                    "([^\"\\" + strDelimiter + "\\r\\n]*))"
                ),
                "gi"
                );

            var arrData = [[]];

            var arrMatches = null;
            while (arrMatches = objPattern.exec( strData )){

                var strMatchedDelimiter = arrMatches[ 1 ];
                if (
                    strMatchedDelimiter.length &&
                    strMatchedDelimiter !== strDelimiter
                    ){
                    arrData.push( [] );

                }

                var strMatchedValue;
                if (arrMatches[ 2 ]){
                    strMatchedValue = arrMatches[ 2 ].replace(
                        new RegExp( "\"\"", "g" ),
                        "\""
                        );

                } else {
                    strMatchedValue = arrMatches[ 3 ];
                }

                arrData[ arrData.length - 1 ].push( strMatchedValue );
            }
            return( arrData );
        }

    function getGeoLocation(){
        var url = "https://geoip.nekudo.com/api/"
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange=function() {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                var location = JSON.parse(xmlhttp.responseText);
                console.log(location.city + " - " + location.country.name)
                serverCountryTextLine.text = location.city + " - " + location.country.name
            }
        }

        xmlhttp.open("GET", url, true);
        xmlhttp.setRequestHeader("Access-Control-Allow-Origin","*")
        xmlhttp.send();
    }

    function getHaproxyStats(obj){
        var url = "http://"+Config.haproxyIp+":"+Config.haproxyPort+"/haproxy_stats;csv"
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange=function() {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                var haproxyStats = csvToArray(xmlhttp.responseText)
                haproxyStats = JSON.stringify(haproxyStats[1]);
                haproxyStats = haproxyStats.split(',')
                haproxyStats[8] = haproxyStats[8].replace('"', '')
                haproxyStats[9] = haproxyStats[9].replace('"', '')
                transferredTextLine.color = "#000000"
                transferredTextLine.font.bold = false
                transferredTextLine.text = "Download: "+formatBytes(parseInt(haproxyStats[8]))+" / Upload: "+ formatBytes(parseInt(haproxyStats[9]))
            }else if(xmlhttp.readyState == 4){
                var host = applicationDirectory;
                var endpoint = ''
                var port = ''
                if(obj.proxy.length > 0){
                    endpoint = obj.proxy[0].endpoint
                    port = obj.proxy[0].port
                }else{
                    endpoint = obj.vpn[0].endpoint
                    port = obj.vpn[0].port
                }

                var certArray = decode64(obj.certArray[0].certContent); // "4pyTIMOgIGxhIG1vZGU="

                flag = 0
                transferredTextLine.text = "Proxy not running!"
                transferredTextLine.color = "#FF4500"
                transferredTextLine.font.bold = true
                callhaproxy.haproxyCert(host, certArray);
                console.log(Config.linuxPathHaproxy.length + "------------------")
                if(Qt.platform.os === "linux"){
                    console.log("call linux haproxy")
                    if(Config.linuxPathHaproxy.length > macHostFlag){
                        callhaproxy.haproxy(host, Config.haproxyIp, Config.haproxyPort, endpoint, port.slice(0,-4), Config.linuxPathHaproxy[macHostFlag])
                        if(Config.linuxPathHaproxy == macHostFlag){changeStatus();}
                    }

                }
                if(Qt.platform.os === "osx"){
                    console.log("call mac haproxy")
                    if(Config.macPathHaproxy.length > macHostFlag){
                        callhaproxy.haproxy(host, Config.haproxyIp, Config.haproxyPort, endpoint, port.slice(0,-4), Config.macPathHaproxy[macHostFlag])
                        if(Config.macPathHaproxy == macHostFlag){changeStatus();}
                    }
                }
                macHostFlag++;

            }
        }

        xmlhttp.open("GET", url, true);
        xmlhttp.setRequestHeader("Access-Control-Allow-Origin","*")
        xmlhttp.send();
    }

    function getMyFeedJson(){
        var myRank = 0
        var url = Config.url+Config.stage+Config.version+Config.feedback+Config.get+"/"+appWindow.currentWallet.address+"/"+idService
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange=function() {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                var mFeed = JSON.parse(xmlhttp.responseText)
                for(var i = 0; i < mFeed.length; i++){
                    if(mFeed[i].mStability == null){
                        mFeed[i].mStability = 0
                    }
                    if(mFeed[i].mSpeed == null){
                        mFeed[i].mSpeed = 0
                    }
                    myRank = (mFeed[i].mStability + mFeed[i].mSpeed)/2
                }
                myRank = parseFloat(myRank).toFixed(1)
                myRankText.text =  myRank
                getColor(myRank, myRankRectangle)

            }
        }

        xmlhttp.open("GET", url, true);
        xmlhttp.setRequestHeader("Access-Control-Allow-Origin","*")
        xmlhttp.send();

    }

    function changeStatus(){
        if (flag == 1){
            subButton.visible = true
            powerOn.source = "../images/power_on.png"
            if(type == "openvpn"){
                shield.source = "../images/shield_vpn_on.png"
            }else{
                shield.source = "../images/shield_proxy_on.png"
            }
            runningText.text = "Connected"
            subButtonText.text = "Disconnect"
            subConnectButton.visible = false
            timerHaproxy.restart()
            timerPayment.restart()
            timerHaproxy.running = true
            timerPayment.running = true

            startText.text = "Connected"
            paidTextLine.text = itnsStart.toFixed(8) + " ITNS"

        }else{
            subButton.visible = false
            shield.source = "../images/shield.png"
            runningText.text = "Not running"
            subConnectButton.visible = true
            //subButtonText.text = "Connect"
            timerHaproxy.stop()
            timerPayment.stop()
            timerHaproxy.running = false
            timerPayment.running = false
            bton = ""
            if(startText.text != "Disconnected"){
                startText.text = "Reconnect"
            }
        }

    }

    function buildTxConnectionString(data) {
        var trStart = '<tr><td width="145" style="padding-top:5px"><b>',
            trMiddle = '</b></td><td style="padding-left:10px;padding-top:5px;">',
            trEnd = "</td></tr>";

        return '<table border="0">'
            //+ (data.id ? trStart + qsTr("ID: ") + trMiddle + data.id + trEnd : "")
            + (data.providerName ? trStart + qsTr("Provider: ") + trMiddle + data.providerName  + trEnd : "")
            + (data.name ? trStart + qsTr("Plan: ") + trMiddle + data.name + trEnd : "")
            + (data.type ? trStart + qsTr("Type: ") + trMiddle + data.type  + trEnd : "")
            + (data.cost ? trStart + qsTr("Price:") + trMiddle + data.cost+" ITNS/min" + trEnd : "")
            + (data.firstPrePaidMinutes ? trStart + qsTr("First Pre Paid Minutes:") + trMiddle + data.firstPrePaidMinutes + trEnd : "")
            + "</table>"
            + translationManager.emptyString;
    }

    function decode64(input) {
        var keyStr = "ABCDEFGHIJKLMNOP" +
                       "QRSTUVWXYZabcdef" +
                       "ghijklmnopqrstuv" +
                       "wxyz0123456789+/" +
                       "=";
         var output = "";
         var chr1, chr2, chr3 = "";
         var enc1, enc2, enc3, enc4 = "";
         var i = 0;

         // remove all characters that are not A-Z, a-z, 0-9, +, /, or =
         var base64test = /[^A-Za-z0-9\+\/\=]/g;
         if (base64test.exec(input)) {
            alert("There were invalid base64 characters in the input text.\n" +
                  "Valid base64 characters are A-Z, a-z, 0-9, '+', '/',and '='\n" +
                  "Expect errors in decoding.");
         }
         input = input.replace(/[^A-Za-z0-9\+\/\=]/g, "");

         do {
            enc1 = keyStr.indexOf(input.charAt(i++));
            enc2 = keyStr.indexOf(input.charAt(i++));
            enc3 = keyStr.indexOf(input.charAt(i++));
            enc4 = keyStr.indexOf(input.charAt(i++));

            chr1 = (enc1 << 2) | (enc2 >> 4);
            chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
            chr3 = ((enc3 & 3) << 6) | enc4;

            output = output + String.fromCharCode(chr1);

            if (enc3 != 64) {
               output = output + String.fromCharCode(chr2);
            }
            if (enc4 != 64) {
               output = output + String.fromCharCode(chr3);
            }

            chr1 = chr2 = chr3 = "";
            enc1 = enc2 = enc3 = enc4 = "";

         } while (i < input.length);

         return unescape(output);
      }

    function createJsonFeedback(obj, rank){
        subButton.visible = true;
        var url = Config.url+Config.stage+Config.version+Config.feedback+Config.setup
        var xmlhttpPost = new XMLHttpRequest();
        xmlhttpPost.onreadystatechange=function() {
            if (xmlhttpPost.readyState == 4 && xmlhttpPost.status == 200) {
                var feed = JSON.parse(xmlhttpPost.responseText)
                var host = applicationDirectory;
                console.log(obj.certArray[0].certContent);

                var endpoint = ''
                var port = ''
                if(obj.proxy.length > 0){
                    endpoint = obj.proxy[0].endpoint
                    port = obj.proxy[0].port
                }else{
                    endpoint = obj.vpn[0].endpoint
                    port = obj.vpn[0].port
                }

                var certArray = decode64(obj.certArray[0].certContent); // "4pyTIMOgIGxhIG1vZGU="
                callhaproxy.haproxyCert(host, certArray);
                callhaproxy.haproxy(host, Config.haproxyIp, Config.haproxyPort, endpoint, port.slice(0,-4), 'haproxy')
                intenseDashboardView.idService = obj.id
                intenseDashboardView.feedback = feed.id
                intenseDashboardView.providerName = obj.providerName
                intenseDashboardView.name = obj.name
                intenseDashboardView.type = obj.type
                intenseDashboardView.cost = obj.cost
                intenseDashboardView.rank = rank
                intenseDashboardView.speed = formatBytes(obj.downloadSpeed)
                intenseDashboardView.firstPrePaidMinutes = obj.firstPrePaidMinutes
                intenseDashboardView.bton = "qrc:///images/power_off.png"
                intenseDashboardView.flag = 1
                intenseDashboardView.secs = 0
                intenseDashboardView.obj = obj
                intenseDashboardView.itnsStart = parseFloat(obj.cost)
                intenseDashboardView.macHostFlag = 0

                changeStatus()
            }
        }

        var data = {"id":obj.providerWallet, "provider":obj.provider, "services":obj.id, "client":appWindow.currentWallet.address}
        data = JSON.stringify(data)
        xmlhttpPost.open("POST", url, true);
        xmlhttpPost.setRequestHeader("Content-type", "application/json");
        xmlhttpPost.send(data);

    }

    function formatBytes(bytes,decimals) {
       if(bytes == 0) return '0 Bytes';
       var k = 1024,
           dm = decimals || 2,
           sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'],
           i = Math.floor(Math.log(bytes) / Math.log(k));
       return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
    }

    function getColor(id, idRank){

        if(id == 5){
            idRank.color = "#008b00"
        }else if(id < 5 && id > 4.5){
            idRank.color = "#66cd00"
        }else if(id <= 4.5 && id > 4){
            idRank.color = "#c0ff3e"
        }else if(id <= 4 && id > 3.5){
            idRank.color = "#caff70"
        }else if(id <= 3.5 && id > 3){
            idRank.color = "#ffff00"
        }else if(id <= 3 && id > 2.5){
            idRank.color = "#ffa500"
        }else if(id <= 2.5 && id > 2){
            idRank.color = "#ffa54f"
        }else if(id <= 2 && id > 1.5){
            idRank.color = "#ff7f24"
        }else if(id <= 1.5 && id > 1){
            idRank.color = "#ee6363"
        }else{
            idRank.color = "#ee2c2c"
        }

    }

    function timer() {
        secs++;
        var h = secs/60/60
        var m = (secs/60)%60
        var s = secs%60
        var array = [h,m,s].map(Math.floor)
        var value = ''
        for(x = 0; x < array.length; x++){
            if(array[x] < 10){
                array[x] = "0" + array[x]
            }else{
                array[x] = array[x]
            }
            function getCom(y){
                if(y < 2){return ":"}else{return ""}
            }
            var c = getCom(x)
            value = value + array[x] + c
        }
        timeonlineTextLine.text = value
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
              source: if(type == "openvpn"){"../images/shield_vpn_on.png"}else if(type == "proxy"){"../images/shield_proxy_on.png"}else{"../images/shield.png"}
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
                text: qsTr("Disconnected")+ translationManager.emptyString
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
              radius: 4

              Text {
                  text: rank
                  font.pixelSize: 13
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.verticalCenter: parent.verticalCenter
                  color: "#000000"
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
              radius: 4

              Text {
                  id: myRankText
                  font.pixelSize: 13
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.verticalCenter: parent.verticalCenter
                  color: "#000000"
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
                text: cost + " ITNS/min"
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
                text: qsTr(speed)+"/s"
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
              height: 380
              onAccepted:{
                  createJsonFeedback(obj, rank)

              }

              GroupBox {
                  anchors.top: parent.top
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.topMargin: 215
                  height: 70
                  ExclusiveGroup { id: tabPositionGroup }
                  Column {
                      anchors.top: parent.top
                      anchors.topMargin: 20
                      RadioButton {
                          id: radioRenew
                          text: "Auto Renew Connection"
                          checked: true
                          exclusiveGroup: tabPositionGroup
                      }
                      RadioButton {
                          id: radioClose
                          text: "Close after time expired"
                          exclusiveGroup: tabPositionGroup
                      }
                  }

              }
          }

          StandardButton {
              visible: if(obj.type == "proxy"){true}else{false}
              id: subConnectButton
              anchors.bottom: parent.bottom
              anchors.right: parent.right
              anchors.rightMargin: 17
              anchors.bottomMargin: 17
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

          StandardDialog {
              id: feedbackPopup
              cancelVisible: false
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
              visible: false
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
                  flag = 0
                  changeStatus()
                  callhaproxy.killHAproxy();
                  timerPayment.running = false
                  feedbackPopup.title = "Provider Feedback";
                  feedbackPopup.open();


              }

              Text {
                  id: subButtonText
                  anchors.verticalCenter: parent.verticalCenter
                  anchors.left: parent.left
                  anchors.leftMargin: 37
                  color: "#ffffff"
                  font.bold: true
                  text: qsTr("Disconnect") + translationManager.emptyString

              }

              Image {
                  id: powerOn
                  anchors.left: parent.left
                  anchors.top:  startText.top
                  anchors.verticalCenter: parent.verticalCenter
                  anchors.leftMargin: 10
                  width: 25; height: 25
                  fillMode: Image.PreserveAspectFit
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
              textFormat: Text.RichText
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
                      leftPanel.selectItem("ITNS Provider")
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
              id: timeonlineTextLine
              anchors.left: timeonlineText.right
              anchors.top:  detailsText.top
              anchors.topMargin: 47
              anchors.leftMargin: 20
              width: 180
              font.pixelSize: 14
              horizontalAlignment: Text.AlignLeft
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
              id: transferredTextLine
              anchors.left: transferredText.right
              anchors.top:  timeonlineTextLine.top
              anchors.topMargin: 27
              anchors.leftMargin: 20
              width: 180
              font.pixelSize: 14
              horizontalAlignment: Text.AlignLeft
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
              id: paidTextLine
              anchors.left: paiduntilnowText.right
              anchors.top:  transferredText.top
              anchors.topMargin: 27
              anchors.leftMargin: 20
              width: 180
              font.pixelSize: 14
              horizontalAlignment: Text.AlignLeft
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
              width: 140
              text: qsTr("Name:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }

        Text {
              visible: !isMobile
              id: providerNameText
              anchors.left: nameText.right
              anchors.top:  providerText.top
              anchors.topMargin: 47
              anchors.leftMargin: 20
              width: 180
              text: qsTr(providerName) + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignLeft
          }


        Text {
              visible: !isMobile
              id: planText
              anchors.left: parent.left
              anchors.top:  nameText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 140
              text: qsTr("Plan:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }


        Text {
              visible: !isMobile
              id: nameIntenseText
              anchors.left: planText.right
              anchors.top:  nameText.top
              anchors.topMargin: 27
              anchors.leftMargin: 20
              width: 180
              text: qsTr(name) + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignLeft
          }


        Text {
              visible: !isMobile
              id: costText
              anchors.left: parent.left
              anchors.top:  planText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 140
              text: qsTr("Price:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }


        Text {
              visible: !isMobile
              id: costIntenseText
              anchors.left: costText.right
              anchors.top:  planText.top
              anchors.topMargin: 27
              anchors.leftMargin: 20
              width: 180
              text: cost + (" ITNS/min")
              font.pixelSize: 14
              horizontalAlignment: Text.AlignLeft
          }


        Text {
              visible: !isMobile
              id: servercountryText
              anchors.left: parent.left
              anchors.top:  costText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 140
              text: qsTr("Country:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }

        Text {
              visible: !isMobile
              id: serverCountryTextLine
              anchors.left: servercountryText.right
              anchors.top:  costText.top
              anchors.topMargin: 27
              anchors.leftMargin: 20
              width: 180
              font.pixelSize: 14
              horizontalAlignment: Text.AlignLeft
          }


        Text {
              visible: !isMobile
              id: serveripText
              anchors.left: parent.left
              anchors.top:  servercountryText.top
              anchors.topMargin: 27
              anchors.leftMargin: 27
              width: 140
              text: qsTr("Server IP:") + translationManager.emptyString
              font.pixelSize: 14
              horizontalAlignment: Text.AlignRight
          }

        Text {
              visible: !isMobile
              id: serveripTextLine
              anchors.left: serveripText.right
              anchors.top:  servercountryText.top
              anchors.topMargin: 27
              anchors.leftMargin: 20
              width: 180
              text: obj.proxy[0].endpoint
              font.pixelSize: 14
              horizontalAlignment: Text.AlignLeft
          }


    }

    //onJsonService:console.debug(item + "------------------------------------")
    Timer {
        id: timerPayment
        interval: getTime()
        repeat: true
        running: false

        onTriggered:
        {
            getITNS()
            //setPayment()
        }
    }
    Timer {
        id: timerHaproxy
        interval: 1000
        repeat: true
        running: false

        onTriggered:
        {
            //setPayment()
            timer()
            getHaproxyStats(obj)
        }
    }




    function onPageCompleted() {
        console.log(flag)
        getColor(rank, rankRectangle)
        getMyFeedJson()
        changeStatus()
        if(providerName != ""){
            //timerHaproxy.running = true
            //timerPayment.running = true
            getGeoLocation()
            howToUseText.visible = false
            orText.visible = false
            searchForProviderText.visible = false
            historicalConnectionLabel.visible = false

            detailsText.visible = true
            timeonlineText.visible = true
            transferredText.visible = true
            paiduntilnowText.visible = true
            paidTextLine.visible = true
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

        }else{
            howToUseText.visible = true
            orText.visible = true
            searchForProviderText.visible = true
            historicalConnectionLabel.visible = true

            detailsText.visible = false
            timeonlineText.visible = false
            transferredText.visible = false
            paiduntilnowText.visible = false
            paidTextLine.visible = false
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
            subConnectButton.visible = false
        }
    }
}
