import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    visible: true
    width: 800
    height: 480
    visibility: "FullScreen"
    title: qsTr("Hello World")

    /*menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }*/

    ColumnLayout
    {

        ToolButton
        {
            iconSource: "/settings.png"
        }

        ToolButton
        {
            iconSource: "/close.png"
            onClicked: Qt.quit();
        }
    }






}
