import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import "../components"



Rectangle {
    id: root
    color: "#F0EEEE"

    Item {
      id: pageRoot
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.topMargin: 20
      height: 400


    }

    Rectangle {
        id: getList
        x: 20
        y: 86
        width: 100
        height: 57
        color: "#000000"
        radius: 40
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        clip: false
        border.width: 0

        StandardButton {
            id: getAll
            width: 80
            shadowReleasedColor: "#983CFF"
            shadowPressedColor: "#B32D00"
            releasedColor: "#813CFF"
            pressedColor: "#983CFF"
            text: qsTr("Generate");
            onClicked: {
                console.log("wow")
                var xmlhttp = new XMLHttpRequest();
                var url = "https://jhx4eq5ijc.execute-api.us-east-1.amazonaws.com/dev/v1/services/search";

                xmlhttp.onreadystatechange=function() {
                    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                        myFunction(xmlhttp.responseText);
                    }
                }
                xmlhttp.open("GET", url, true);
                xmlhttp.send();

                function myFunction(json) {
                    var obj = JSON.parse(json);
                    console.log(json)
                }
            }
        }


    }


       /*
        onClicked: {
            var xmlhttp = new XMLHttpRequest();
            var url = "https://jhx4eq5ijc.execute-api.us-east-1.amazonaws.com/dev/v1/services/search";

            xmlhttp.onreadystatechange=function() {
                if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                    myFunction(xmlhttp.responseText);
                }
            }
            xmlhttp.open("GET", url, true);
            xmlhttp.send();

            function myFunction(json) {
                var obj = JSON.parse(json);
                listview.model.append( {jsondata: obj.first +" "+ obj.last })
            }
        }
        */

    // pageRoot




}
