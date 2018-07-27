import QtQuick 2.0
import moneroComponents.TransactionInfo 1.0
import QtQuick.Controls 1.4
import QtQml 2.2
import moneroComponents.Wallet 1.0
import moneroComponents.WalletManager 1.0
import "../components"
import "../IntenseConfig.js" as Config

Rectangle {
    id: root
    property var model

    function buildTxDetailsString(data, rank) {
        console.log(data.subsequentVerificationsNeeded + "-------------------- ttt")
        var trStart = '<tr><td width="185" style="padding-top:5px"><b>',
            trMiddle = '</b></td><td style="padding-left:10px;padding-top:5px;">',
            trEnd = "</td></tr>";

        return '<table border="0">'
            + (data.id ? trStart + qsTr("ID: ") + trMiddle + data.id + trEnd : "")
            + (data.provider ? trStart + qsTr("Provider: ") + trMiddle + data.provider  + trEnd : "")
            + (data.name ? trStart + qsTr("Plan: ") + trMiddle + data.name + trEnd : "")
            + (data.type ? trStart + qsTr("Type: ") + trMiddle + data.type  + trEnd : "")
            + (data.cost ? trStart + qsTr("Price:") + trMiddle + data.cost + " ITNS/min" + trEnd : "")
            + (data.firstPrePaidMinutes ? trStart + qsTr("First Pre Paid Minutes:") + trMiddle + data.firstPrePaidMinutes + trEnd : "")
            + (data.firstVerificationsNeeded ? trStart + qsTr("First Verifications Needed:") + trMiddle + data.firstVerificationsNeeded + trEnd : "")
            + (data.subsequentPrePaidMinutes ? trStart + qsTr("Subsequent Pre Paid Minutes:") + trMiddle + data.subsequentPrePaidMinutes + trEnd : "")
            + (data.subsequentVerificationsNeeded >= 0 ? trStart + qsTr("Subsequent Verifications Needed:") + trMiddle + data.subsequentVerificationsNeeded + trEnd : "")
            + (data.allowRefunds ? trStart + qsTr("Allow Refunds:") + trMiddle + data.allowRefunds + trEnd : "")
            + (data.downloadSpeed ? trStart + qsTr("Download Speed:") + trMiddle + formatBytes(data.downloadSpeed)+"/s" + trEnd : "")
            + (data.uploadSpeed ? trStart + qsTr("Upload Speed:") + trMiddle + formatBytes(data.uploadSpeed)+"/s" + trEnd : "")
            + (rank ? trStart + qsTr("Rating:") + trMiddle + rank + " of 5" + trEnd : "")
            + "</table>"
            + translationManager.emptyString;
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
            return "#008b00"
        }else if(id < 5 && id > 4.5){
            return "#66cd00"
        }else if(id <= 4.5 && id > 4){
            return "#c0ff3e"
        }else if(id <= 4 && id > 3.5){
            return "#caff70"
        }else if(id <= 3.5 && id > 3){
            return "#ffff00"
        }else if(id <= 3 && id > 2.5){
            return "#ffa500"
        }else if(id <= 2.5 && id > 2){
            return "#ffa54f"
        }else if(id <= 2 && id > 1.5){
            return "#ff7f24"
        }else if(id <= 1.5 && id > 1){
            return "#ee6363"
        }else{
            return "#ee2c2c"
        }

    }

    function getBackgroundColor(id){
        if(id & 1){
            return "#f0f0f0"
        } else {
            return "#fafafa"
        }
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
                intenseDashboardView.obj = obj
                intenseDashboardView.secs = 0
                intenseDashboardView.itnsStart = parseFloat(obj.cost)

                middlePanel.state = "ITNS Dashboard"

                leftPanel.selectItem("ITNS Dashboard")
            }
        }

        var data = {"id":obj.providerWallet, "provider":obj.provider, "services":obj.id, "client":appWindow.currentWallet.address}
        data = JSON.stringify(data)
        xmlhttpPost.open("POST", url, true);
        xmlhttpPost.setRequestHeader("Content-type", "application/json");
        xmlhttpPost.send(data);

    }

    function getJson(speed, speedType, price, tp, favorite){

        if(speed != undefined){
            speed = speed * 1024
            if(speedType == "mb"){
                speed = speed * 1024
            }
            console.log(speed)
        }
        var url = Config.url+Config.stage+Config.version+Config.services+Config.search
        var xmlhttp = new XMLHttpRequest();
        listView.model.clear()
        xmlhttp.onreadystatechange=function() {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                getJsonFail.visible = false
                loading.visible = false
                var arr = JSON.parse(xmlhttp.responseText)
                for(var i = 0; i < arr.length; i++) {
                    if(arr[i].mStability == null){

                        arr[i].mStability = 0
                    }
                    if(arr[i].mSpeed == null){
                        arr[i].mSpeed = 0
                    }
                    if(arr[i].type == "proxy"){
                        var type = arr[i].proxy[0].endpoint +":"+arr[i].proxy[0].port
                    }else{
                        var type = arr[i].vpn[0].endpoint +":"+arr[i].vpn[0].port
                    }

                    var rank = (arr[i].mStability + arr[i].mSpeed)/2
                    rank = parseFloat(rank).toFixed(1)
                    if(speed == undefined && tp == undefined && price == undefined){
                        listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                    }else

                    // C1
                    if(speed == "" && isNaN(price) && tp == "all" && favorite == true){
                        for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                            if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                            }
                        }
                    }

                    // C1
                    if(speed == "" && isNaN(price) && tp == "all" && favorite == false){
                        listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                    }
                    // AC1
                    else if(speed <= arr[i].downloadSpeed && isNaN(price) && tp == "all" && favorite == false){
                        listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                    }

                    else if(speed <= arr[i].downloadSpeed && isNaN(price) && tp == "all" && favorite == true){
                        for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                            if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                            }
                        }
                    }


                    // BC1
                    else if(speed == "" && price >= arr[i].cost && tp == "all" && favorite == false){
                        listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                    }

                    else if(speed == "" && price >= arr[i].cost && tp == "all" && favorite == true){
                        for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                            if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                            }
                        }
                    }
                    // C2
                    else if(speed == "" && isNaN(price) && tp == "vpn" && favorite == false){
                        if(arr[i].vpn.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }

                    else if(speed == "" && isNaN(price) && tp == "vpn" && favorite == true){
                        if(arr[i].vpn.length > 0 ){
                            for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                                if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                    listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                                }
                            }
                        }
                    }
                    // C3
                    else if(speed == "" && price >= arr[i].cost && tp == "proxy" && favorite == false){
                        if(arr[i].proxy.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    else if(speed == "" && price >= arr[i].cost && tp == "proxy" && favorite == true){
                        if(arr[i].proxy.length > 0 ){
                            for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                                if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                    listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                                }
                            }
                        }
                    }
                    // ABC1
                    else if(speed <= arr[i].downloadSpeed && price >= arr[i].cost && tp == "all" && favorite == false){
                        listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                    }
                    else if(speed <= arr[i].downloadSpeed && price >= arr[i].cost && tp == "all" && favorite == true){
                        for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                            if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                            }
                        }
                    }
                    // ABC2
                    else if(speed <= arr[i].downloadSpeed && price >= arr[i].cost && tp == "vpn" && favorite == false){
                        if(arr[i].vpn.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    else if(speed <= arr[i].downloadSpeed && price >= arr[i].cost && tp == "vpn" && favorite == true){
                        if(arr[i].vpn.length > 0 ){
                            for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                                if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                    listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                                }
                            }
                        }
                    }
                    // ABC3
                    else if(speed <= arr[i].downloadSpeed && price >= arr[i].cost && tp == "proxy" && favorite == false){
                        if(arr[i].proxy.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    else if(speed <= arr[i].downloadSpeed && price >= arr[i].cost && tp == "proxy" && favorite == true){
                        if(arr[i].proxy.length > 0 ){
                            for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                                if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                    listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                                }
                            }
                        }
                    }
                    // BC2
                    else if(speed == "" && price >= arr[i].cost && tp == "vpn" && favorite == false ){
                        if(arr[i].vpn.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    else if(speed == "" && price >= arr[i].cost && tp == "vpn" && favorite == true ){
                        if(arr[i].vpn.length > 0 ){
                            for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                                if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                    listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                                }
                            }
                        }
                    }
                    // BC3
                    else if(speed == "" && price >= arr[i].cost && tp == "proxy" && favorite == false){
                        if(arr[i].proxy.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    else if(speed == "" && price >= arr[i].cost && tp == "proxy" && favorite == true){
                        if(arr[i].proxy.length > 0 ){
                            for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                                if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                    listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                                }
                            }
                        }
                    }
                    // AC2
                    else if(speed <= arr[i].downloadSpeed && isNaN(price) && tp == "vpn" && favorite == false){
                        if(arr[i].vpn.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    else if(speed <= arr[i].downloadSpeed && isNaN(price) && tp == "vpn" && favorite == true){
                        if(arr[i].vpn.length > 0 ){
                            for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                                if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                    listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                                }
                            }
                        }
                    }
                    // AC3
                    else if(speed <= arr[i].downloadSpeed && isNaN(price) && tp == "proxy" && favorite == false){
                        if(arr[i].proxy.length > 0 ){
                            listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                        }
                    }
                    else if(speed <= arr[i].downloadSpeed && isNaN(price) && tp == "proxy" && favorite == true){
                        if(arr[i].proxy.length > 0 ){
                            for(var showFav = 0; showFav < appWindow.persistentSettings.favorite.length-1; showFav++) {
                                if(appWindow.persistentSettings.favorite[showFav].id == arr[i].id && appWindow.persistentSettings.favorite[showFav].provider == arr[i].provider) {
                                    listView.model.append( {listdata:"<div style='font-size: 14px; font-weight: bold;'> " + arr[i].providerName + "</div><br />" + arr[i].name +"<br /> "+ type + " - "+ arr[i].type.toUpperCase() +"<br /><div style='font-weight: bold;'>"+ formatBytes(arr[i].downloadSpeed) +"/s </div>- "+ arr[i].cost + " ITNS/min", obj: arr[i], rank: rank, type: type, index: i})
                                }
                            }
                        }
                    }

                }
            }else if(xmlhttp.readyState == 4){
                var urlGEO = "https://geoip.nekudo.com/api/"
                var xmlGEOhttp = new XMLHttpRequest();

                xmlGEOhttp.onreadystatechange=function() {
                    getJsonFail.visible = true;
                    loading.visible = false
                    if (xmlGEOhttp.readyState == 4 && xmlGEOhttp.status == 200) {
                        getJsonFail.text = "Error status - SDP: " + xmlhttp.status + "<br />Error readyState - SDP: " + xmlhttp.readyState + "<br />" + xmlhttp.responseText + "<br /><br />" + "Status - GEO: " + xmlGEOhttp.status
                    }else if(xmlGEOhttp.readyState == 4){
                        getJsonFail.text = "Error status - SDP: " + xmlhttp.status + "<br />Error readyState - SDP: " + xmlhttp.readyState + "<br />" + xmlhttp.responseText + "<br /><br />" + "Error Status - GEO: " + xmlGEOhttp.status
                    }
                }
                xmlGEOhttp.open("GET", urlGEO, true);
                xmlGEOhttp.setRequestHeader("Access-Control-Allow-Origin","*")
                xmlGEOhttp.send();
            }
        }

        xmlhttp.open("GET", url, true);
        xmlhttp.setRequestHeader("Access-Control-Allow-Origin","*")
        xmlhttp.send();
    }

    function getCheckedFavorite(obj){
        console.log(appWindow.persistentSettings.favorite.length + "get fav total")
        for(var iCheckedFavorite = 0; iCheckedFavorite < appWindow.persistentSettings.favorite.length; iCheckedFavorite++) {
            if(appWindow.persistentSettings.favorite[iCheckedFavorite].id == obj.id && appWindow.persistentSettings.favorite[iCheckedFavorite].provider == obj.provider) {
                console.log(appWindow.persistentSettings.favorite[iCheckedFavorite].id)

                return true
            }
        }
        return false
    }

    function getFavorite(checked, obj){
        if(checked == true){
            appWindow.persistentSettings.favorite.push({id:obj.id, provider:obj.provider})
            console.log((appWindow.persistentSettings.favorite.length) + "---------- push Favorite")
        }else{
            for(var iFavorite = 0; iFavorite < appWindow.persistentSettings.favorite.length; iFavorite++) {
                if(appWindow.persistentSettings.favorite[iFavorite].id == obj.id && appWindow.persistentSettings.favorite[iFavorite].provider == obj.provider) {
                   appWindow.persistentSettings.favorite.splice(iFavorite, 1);
                   console.log(appWindow.persistentSettings.favorite.length + "my length atual")
                }
            }
        }
        //walletManager.persistentSettings(fav);
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
          width: 100
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
          width: 100
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
          width: 110
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
          width: 110

      }

      ListModel {
          id: typeSpeed
          ListElement { column1: "KB/s"; column2: ""; value: "kb" }
          ListElement { column1: "MB/s"; column2: ""; value: "mb" }
      }

      StandardDropdown {
          visible: !isMobile
          id: speedDrop
          anchors.left: minSpeedLine.right
          anchors.top: minSpeedText.bottom
          anchors.leftMargin: 0
          anchors.topMargin: 5
          width: 80
          shadowReleasedColor: "#4D0051"
          shadowPressedColor: "#2D002F"
          releasedColor: "#6B0072"
          pressedColor: "#4D0051"
          dataModel: typeSpeed
          z: 100
      }



      CheckBox {
          visible: !isMobile
          id: favoriteFilter
          text: qsTr("Favorite") + translationManager.emptyString
          anchors.left: speedDrop.right
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
              getJson(minSpeedLine.text, typeSpeed.get(speedDrop.currentIndex).value, parseFloat(maxPriceLine.text), typeTransaction.get(typeDrop.currentIndex).value, favoriteFilter.checked)
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


        Text {
          visible: false
          id: getJsonFail
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.top:  parent.top
          anchors.topMargin: 75
          font.pixelSize: 16
          width: 400
          height: 800
          color: "#535353"
          font.family: "Arial"
        }

        Text {
          id: loading
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.top:  parent.top
          anchors.topMargin: 75
          text: qsTr("Loading services ...") + translationManager.emptyString
          font.pixelSize: 20
          color: "#535353"
          font.family: "Arial"
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
                        textFormat: Text.RichText


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
                            checked: getCheckedFavorite(obj)
                            onClicked: {
                                getFavorite(this.checked, obj)
                            }
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



                        StandardDialog {
                            id: detailsPopup
                            cancelVisible: false
                            okVisible: true
                            width:720
                            height: 500

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
                            textColor: "#000000"

                        }

                        StandardButton {
                            visible: if(obj.type == "proxy"){true}else{false}
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
        loading.visible = true
        getJson()
    }
}
